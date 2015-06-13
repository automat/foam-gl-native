var fs   = require('fs');
var path = require('path');

var Utils     = require('../_common/Utils');
var glu       = require('../_common/glu');
var Vec3      = require('../_common/Vec3');
var Matrix44  = require('../_common/Matrix44');
var Geoemetry = require('../_common/Geometry');
var Context = require('../../index.js');
var gl = Context.gl;


var ERROR_STR = {};
ERROR_STR[gl.INVALID_OPERATION] = 'INVALID_OPERATION';
ERROR_STR[gl.INVALID_ENUM] = 'INVALID_ENUM';
ERROR_STR[gl.INVALID_VALUE] = 'INVALID_VALUE';
ERROR_STR[gl.OUT_OF_MEMORY] = 'OUT_OF_MEMORY';
ERROR_STR[gl.INVALID_FRAMEBUFFER_OPERATION] = 'INVALID_FRAMEBUFFER_OPERATION';

function getGLError(){
    var error = gl.getError();
    if(error != gl.NO_ERROR){
        error = ERROR_STR[error] === undefined ? error : ERROR_STR[error];
        console.log("GL_ERROR",error);
    }
}

var VERT_SRC =
    "#version 330\n" +
    "layout(location = 0) in vec4 vposition;\n" +
    "layout(location = 1) in vec2 vtexcoord;\n" +
    "out vec2 ftexcoord;\n" +
    "void main() {\n" +
    "   ftexcoord = vtexcoord;\n" +
    "   gl_Position = vposition;\n" +
    "}\n";

var FRAG_SRC =
    "#version 330\n" +
    "uniform sampler2D tex;\n" + // texture uniform
    "in vec2 ftexcoord;\n" +
    "layout(location = 0) out vec4 FragColor;\n" +
    "void main() {\n" +
    "   FragColor = texture(tex, ftexcoord);\n" +
    "}\n";

function setup(){
    this.initWindow(800,600);

    console.log(this.getDPI());

    var program = this._program = Utils.createProgramv(gl,[
        {type : gl.VERTEX_SHADER,   src : VERT_SRC},
        {type : gl.FRAGMENT_SHADER, src : FRAG_SRC}
    ]);

    this._uniformLocationTex = gl.getUniformLocation(program,'tex');

    this._vao = gl.genVertexArrays(1)[0];
    gl.bindVertexArray(this._vao);

    this._vbo = gl.genBuffers(1)[0];
    gl.bindBuffer(gl.ARRAY_BUFFER, this._vbo);

    var vertexData = new Float32Array([
        1.0, 1.0, 0.0, 1.0, 1.0,
        -1.0, 1.0, 0.0, 0.0, 1.0,
        1.0, -1.0, 0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0, 0.0, 0.0
    ]);

    gl.bufferData(gl.ARRAY_BUFFER, vertexData, gl.STATIC_DRAW);

    gl.enableVertexAttribArray(0);
    gl.vertexAttribPointer(0,3,gl.FLOAT,gl.FALSE,5 * 4,0);
    //
    gl.enableVertexAttribArray(1);
    gl.vertexAttribPointer(1,2,gl.FLOAT,gl.FALSE,5 * 4,3 * 4);

    this._ibo = gl.genBuffers(1)[0];
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this._ibo);

    var indexData = new Uint16Array([
        0,1,2,
        2,1,3
    ]);

    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,indexData,gl.STATIC_DRAW);

    gl.bindVertexArray(0);

    this._texture = gl.genTextures(1)[0];
    gl.bindTexture(gl.TEXTURE_2D,this._texture);

    var imageData = gl.readImageData(__dirname + '/texture.png');

    console.log(imageData.data.length,imageData.data.byteLength);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, imageData.width, imageData.height, 0, gl.RGB, gl.UNSIGNED_BYTE, imageData.data);

    this._writeTestImage = false;
    this._imageWritten = false;
}

function update(){
    gl.clearColor(0,0,1,1);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.useProgram(this._program);

    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, this._texture);

    gl.uniform1i(this._uniformLocationTex, 0);

    gl.bindVertexArray(this._vao);
    gl.drawElements(gl.TRIANGLES,6,gl.UNSIGNED_SHORT, 0);

    getGLError();

    if(!this._imageWritten){
        var windowSize = this.getWindowSize();
        gl.writeImage(__dirname + '/out.png',0,0,windowSize[0],windowSize[1]);
        this._imageWritten = true;
    }
}

Context.new({setup:setup,update:update});