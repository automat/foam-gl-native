var fs   = require('fs');
var path = require('path');

var Utils   = require('../_common/Utils');
var Context = require('../../index.js');
var gl = Context.gl;

var VERT_SRC =
    "#version 330\n" +
    "layout(location = 0) in vec4 vposition;\n" +
    "layout(location = 1) in vec4 vcolor;\n" +
    "out vec4 fcolor;\n" +
    "void main() {\n" +
    "   fcolor = vcolor;\n" +
    "   gl_Position = vposition;\n" +
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

    this._vao = gl.createVertexArray();
    gl.bindVertexArray(this._vao);

    this._vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER,this._vbo);

    this._ibo = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this._ibo);

    var vertexData = new Float32Array([
        //  X     Y     Z           R     G     B
         1.0, 1.0, 0.0,       1.0, 0.0, 0.0, // vertex 0
        -1.0, 1.0, 0.0,       0.0, 1.0, 0.0, // vertex 1
         1.0,-1.0, 0.0,       0.0, 0.0, 1.0, // vertex 2
        -1.0,-1.0, 0.0,       1.0, 0.0, 0.0  // vertex 3
    ]); 

    gl.bufferData(gl.ARRAY_BUFFER, vertexData, gl.STATIC_DRAW);

    gl.enableVertexAttribArray(0);
    gl.vertexAttribPointer(0,3, gl.FLOAT, false, 6 * 4, 0);

    gl.enableVertexAttribArray(1);
    gl.vertexAttribPointer(1,3, gl.FLOAT, false, 6 * 4, 3 * 4);

    var indexData = new Uint16Array([
        0,1,2,
        2,1,3
    ]);

    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, indexData, gl.STATIC_DRAW);

    gl.useProgram(this._program);
    gl.bindVertexArray(this._vao);
    gl.clearColor(0,0,1,1);
}

function update(){
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.drawElements(gl.TRIANGLES, 6, gl.UNSIGNED_SHORT, 0);
}

Context.new({setup:setup,update:update});



























