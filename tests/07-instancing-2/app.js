var fs   = require('fs');
var path = require('path');

var Utils    = require('../_common/Utils');
var glu      = require('../_common/glu');
var Matrix44 = require('../_common/Matrix44');
var Context  = require('../../index.js');
var gl       = Context.gl;

var VERT_SRC =
    "#version 330\n" +
    "layout(std140) uniform Matrices {\n" +
    "   mat4 ViewProjection;\n" +
    "   mat4 Model[8];\n" +
    "};\n" +
    "layout(location = 0) in vec4 vposition;\n" +
    "layout(location = 1) in vec4 vcolor;\n" +
    "out vec4 fcolor;\n" +
    "void main() {\n" +
    "   fcolor = vcolor;\n" +
    "   gl_Position = ViewProjection*Model[gl_InstanceID]*vposition;\n" +
    "}\n";

var FRAG_SRC =
    "#version 330\n" +
    "in vec4 fcolor;\n" +
    "layout(location = 0) out vec4 FragColor;\n" +
    "void main() {\n" +
    "   FragColor = fcolor;\n" +
    "}\n";


function setup(){
    this.initWindow(800,600);

    this._program = Utils.createProgram(gl,VERT_SRC, FRAG_SRC);
    this._uniformLocationViewProjectionMatrix = gl.getUniformLocation(this._program, 'ViewProjection');

    this._matricesBinding   = 0;
    this._uniformBlockIndex = gl.getUniformBlockIndex(this._program,'Matrices');
    gl.uniformBlockBinding(this._program, this._uniformBlockIndex, this._matricesBinding);

    this._ubo = gl.createBuffer();
    gl.bindBuffer(gl.UNIFORM_BUFFER, this._ubo);
    gl.bufferData(gl.UNIFORM_BUFFER, 8 * 4 * 4 * 4, 0, gl.STREAM_DRAW);


    function createTranslationMatrixF32(x,y,z){
        return new Matrix44().translatef(x,y,z).m;
    }

    var modelMatrices = new Float32Array([
        createTranslationMatrixF32( 2, 2,-2),
        createTranslationMatrixF32( 2, 2,-2),
        createTranslationMatrixF32( 2,-2, 2),
        createTranslationMatrixF32( 2,-2,-2),
        createTranslationMatrixF32(-2, 2, 2),
        createTranslationMatrixF32(-2, 2,-2),
        createTranslationMatrixF32(-2,-2, 2),
        createTranslationMatrixF32(-2,-2,-2)
    ]);


    gl.bufferSubData(gl.UNIFORM_BUFFER, 0, modelMatrices);

    this._vao = gl.createVertexArray();
    gl.bindVertexArray(this._vao);

    this._vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER,this._vbo);

    this._ibo = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this._ibo);

    var vertexData = new Float32Array([
        // face 0:
        1.0, 1.0, 1.0,       1.0, 0.0, 0.0, // vertex 0
        -1.0, 1.0, 1.0,       1.0, 0.0, 0.0, // vertex 1
        1.0,-1.0, 1.0,       1.0, 0.0, 0.0, // vertex 2
        -1.0,-1.0, 1.0,       1.0, 0.0, 0.0, // vertex 3

        // face 1:
        1.0, 1.0, 1.0,       0.0, 1.0, 0.0, // vertex 0
        1.0,-1.0, 1.0,       0.0, 1.0, 0.0, // vertex 1
        1.0, 1.0,-1.0,       0.0, 1.0, 0.0, // vertex 2
        1.0,-1.0,-1.0,       0.0, 1.0, 0.0, // vertex 3

        // face 2:
        1.0, 1.0, 1.0,       0.0, 0.0, 1.0, // vertex 0
        1.0, 1.0,-1.0,       0.0, 0.0, 1.0, // vertex 1
        -1.0, 1.0, 1.0,       0.0, 0.0, 1.0, // vertex 2
        -1.0, 1.0,-1.0,       0.0, 0.0, 1.0, // vertex 3

        // face 3:
        1.0, 1.0,-1.0,       1.0, 1.0, 0.0, // vertex 0
        1.0,-1.0,-1.0,       1.0, 1.0, 0.0, // vertex 1
        -1.0, 1.0,-1.0,       1.0, 1.0, 0.0, // vertex 2
        -1.0,-1.0,-1.0,       1.0, 1.0, 0.0, // vertex 3

        // face 4:
        -1.0, 1.0, 1.0,       0.0, 1.0, 1.0, // vertex 0
        -1.0, 1.0,-1.0,       0.0, 1.0, 1.0, // vertex 1
        -1.0,-1.0, 1.0,       0.0, 1.0, 1.0, // vertex 2
        -1.0,-1.0,-1.0,       0.0, 1.0, 1.0, // vertex 3

        // face 5:
        1.0,-1.0, 1.0,       1.0, 0.0, 1.0, // vertex 0
        -1.0,-1.0, 1.0,       1.0, 0.0, 1.0, // vertex 1
        1.0,-1.0,-1.0,       1.0, 0.0, 1.0, // vertex 2
        -1.0,-1.0,-1.0,       1.0, 0.0, 1.0, // vertex 3
    ]);

    gl.bufferData(gl.ARRAY_BUFFER, vertexData, gl.STATIC_DRAW);

    gl.enableVertexAttribArray(0);
    gl.vertexAttribPointer(0,3, gl.FLOAT, false, 6 * 4, 0);

    gl.enableVertexAttribArray(1);
    gl.vertexAttribPointer(1,3, gl.FLOAT, false, 6 * 4, 3 * 4);

    var indexData = new Uint16Array([
        // face 0:
        0,1,2,      // first triangle
        2,1,3,      // second triangle
        // face 1:
        4,5,6,      // first triangle
        6,5,7,      // second triangle
        // face 2:
        8,9,10,     // first triangle
        10,9,11,    // second triangle
        // face 3:
        12,13,14,   // first triangle
        14,13,15,   // second triangle
        // face 4:
        16,17,18,   // first triangle
        18,17,19,   // second triangle
        // face 5:
        20,21,22,   // first triangle
        22,21,23,   // second triangle
    ]);

    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, indexData, gl.STATIC_DRAW);

    this._tbo = gl.createBuffer(gl.ARRAY_BUFFER);
    gl.bindBuffer(gl.ARRAY_BUFFER,this._tbo);

    var translationData = new Float32Array([
        2.0, 2.0, 2.0,  // cube 0
        2.0, 2.0,-2.0,  // cube 1
        2.0,-2.0, 2.0,  // cube 2
        2.0,-2.0,-2.0,  // cube 3
        -2.0, 2.0, 2.0,  // cube 4
        -2.0, 2.0,-2.0,  // cube 5
        -2.0,-2.0, 2.0,  // cube 6
        -2.0,-2.0,-2.0,  // cube 7
    ]);

    gl.bufferData(gl.ARRAY_BUFFER, translationData, gl.STATIC_DRAW);

    gl.enableVertexAttribArray(2);
    gl.vertexAttribPointer(2,3,gl.FLOAT, false, 0, 0);
    gl.vertexAttribDivisor(2,1);

    gl.useProgram(this._program);
    gl.bindVertexArray(this._vao);
    gl.clearColor(0,0,0,1);
    gl.enable(gl.DEPTH_TEST);

    this._matrixProjection = new Matrix44();

    glu.perspective(this._matrixProjection.m,45, this.getWindowAspectRatio(), 0.01, 100);

    this._matrixView = new Matrix44();
    glu.lookAt(this._matrixView.m,0,-1,0,0,0,0,0,1,0);

    this._matrixProjectionView = new Matrix44();
    this._matrixTemp44 = new Float32Array(16);
}

function update(){
    gl.clear(gl.DEPTH_BUFFER_BIT | gl.COLOR_BUFFER_BIT);

    var matrixProjection     = this._matrixProjection;
    var matrixView           = this._matrixView;
    var matrixProjectionView = this._matrixProjectionView;
    var matrixTemp           = this._matrixTemp44;

    var t = this.getSecondsElapsed();
    var d = 2.25 + (0.5 + Math.sin(t) * 0.5) * 7.75;

    glu.lookAt(matrixView.m,Math.cos(t) * d, Math.sin(t * 0.125) * 1.25,Math.sin(t) * d, 0,0,0, 0,1,0);
    matrixView.multiplied(matrixProjection,matrixProjectionView);
    matrixTemp.set(matrixProjectionView.m);

    gl.uniformMatrix4fv(this._uniformLocationViewProjectionMatrix,false,matrixTemp);



    //gl.drawElements(gl.TRIANGLES, 6 * 6, gl.UNSIGNED_SHORT, 0);

    gl.drawElementsInstanced(gl.TRIANGLES, 6 * 6, gl.UNSIGNED_SHORT, 0, 8);
}

Context.new({setup:setup,update:update});



























