var Utils    = require('../_common/Utils');
var glu      = require('../_common/glu');
var Matrix44 = require('../_common/Matrix44');
var Context  = require('../../index.js');
var gl       = Context.gl;

var VERT_SRC =
    "#version 330\n" +
    "layout(location = 0) in vec4 vposition;\n" +
    "void main() {\n" +
    "   gl_Position = vposition;\n" +
    "}\n";

var GEOM_SRC =
    "#version 330\n" +
    "uniform mat4 View;\n" +
    "uniform mat4 Projection;\n" +
    "layout (points) in;\n" +
    "layout (triangle_strip, max_vertices = 4) out;\n" +
    "out vec2 txcoord;\n" +
    "void main() {\n" +
    "   vec4 pos = View*gl_in[0].gl_Position;\n" +
    "   txcoord = vec2(-1,-1);\n" +
    "   gl_Position = Projection*(pos+vec4(txcoord,0,0));\n" +
    "   EmitVertex();\n" +
    "   txcoord = vec2( 1,-1);\n" +
    "   gl_Position = Projection*(pos+vec4(txcoord,0,0));\n" +
    "   EmitVertex();\n" +
    "   txcoord = vec2(-1, 1);\n" +
    "   gl_Position = Projection*(pos+vec4(txcoord,0,0));\n" +
    "   EmitVertex();\n" +
    "   txcoord = vec2( 1, 1);\n" +
    "   gl_Position = Projection*(pos+vec4(txcoord,0,0));\n" +
    "   EmitVertex();\n" +
    "}\n";


var FRAG_SRC =
    "#version 330\n" +
    "in vec2 txcoord;\n" +
    "layout(location = 0) out vec4 FragColor;\n" +
    "void main() {\n" +
    "   float s = 0.2*(1/(1+15.*dot(txcoord, txcoord))-1/16.);\n" +
    "   FragColor = s*vec4(0.9,0.29,0.6,1);\n" +
    "}\n";

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

function setup(){
    this.initWindow(800,600);

    var shaderVert = Utils.compileShader(gl,gl.VERTEX_SHADER,   VERT_SRC);
    var shaderGeom = Utils.compileShader(gl,gl.GEOMETRY_SHADER, GEOM_SRC);
    var shaderFrag = Utils.compileShader(gl,gl.FRAGMENT_SHADER, FRAG_SRC);

    var program = this._program = gl.createProgram();
    gl.attachShader(program, shaderVert);
    gl.attachShader(program, shaderGeom);
    gl.attachShader(program, shaderFrag);

    gl.linkProgram(program);
    gl.useProgram(program);

    if(!gl.getProgramParameter(program,gl.LINK_STATUS)){
        throw new Error('PROGRAM ' + gl.getProgramInfoLog(program));
    }

    this._uniformLocationView       = gl.getUniformLocation(program,'View');
    this._uniformLocationProjection = gl.getUniformLocation(program,'Projection');

    console.log(this._uniformLocationView,this._uniformLocationProjection);

    this._vao = gl.createVertexArray();
    gl.bindVertexArray(this._vao);

    var vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER,vbo);

    var particles = this._particles = 256 * 1024;
    var vertexData = new Float32Array(particles * 3);

    for(var i = 0; i < particles; ++i){
        var arm   = Math.floor(3 * Math.random());
        var alpha = 1.0 / (0.1 + Math.pow(Math.random(),0.7)-1/1.1);
        var r     = 4.0 * alpha;

        alpha += arm * 2.0 * Math.PI / 3.0;

        vertexData[i * 3 + 0] = r * Math.sin(alpha);
        vertexData[i * 3 + 1] = 0;
        vertexData[i * 3 + 2] = r * Math.cos(alpha);

        vertexData[i * 3 + 0] += (4.0 - 0.2 * alpha)*(2-(Math.random() + Math.random() + Math.random() + Math.random()));
        vertexData[i * 3 + 1] += (2.0 - 0.1 * alpha)*(2-(Math.random() + Math.random() + Math.random() + Math.random()));
        vertexData[i * 3 + 2] += (4.0 - 0.2 * alpha)*(2-(Math.random() + Math.random() + Math.random() + Math.random()));
    }

    gl.bufferData(gl.ARRAY_BUFFER,vertexData,gl.STATIC_DRAW);

    gl.enableVertexAttribArray(0);
    gl.vertexAttribPointer(0,3,gl.FLOAT,gl.FALSE,0,0);

    gl.disable(gl.DEPTH_TEST);
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.ONE,gl.ONE);
}


function update(){
    var t = this.getSecondsElapsed();

    gl.clearColor(0,0,0,1);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    var projection = new Float32Array(16);
    glu.perspective(projection,90,this.getWindowAspectRatio(),0.1,100.0);

    var view = new Float32Array(16);
    var distance = 20 + (0.5 + Math.sin(t * 0.01) * 0.5) * 20;
    glu.lookAt(view,Math.cos(t * 0.1) * distance,Math.sin(t) * 10,Math.sin(t * 0.1) * distance,0,0,0,0,1,0);

    gl.uniformMatrix4fv(this._uniformLocationView, gl.FALSE, view);
    gl.uniformMatrix4fv(this._uniformLocationProjection, gl.FALSE, projection);

    gl.bindVertexArray(this._vao);
    gl.drawArrays(gl.POINTS, 0, this._particles);

    getGLError();
}

Context.new({setup:setup,update:update})
