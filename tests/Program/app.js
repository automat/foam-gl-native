var FoamGLFW = require('../../index.js');
var gl = FoamGLFW.gl;
var fs = require('fs');
var path = require('path');

function setup(){
    this.setWindow(800,600);

    var vertexShader, fragmentShader;

    vertexShader  = gl.createShader(gl.VERTEX_SHADER);
    fragmentShader  = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(
        vertexShader,
        fs.readFileSync(path.join(__dirname,'vert.glsl'),{encoding:'utf8'})
    );
    gl.compileShader(vertexShader);

    if(!gl.getShaderParameter(vertexShader,gl.COMPILE_STATUS)){
        console.log(gl.getShaderInfoLog(vertexShader));
    }

    gl.shaderSource(
        fragmentShader,
        fs.readFileSync(path.join(__dirname,'frag.glsl'),{encoding:'utf8'})
    );
    gl.compileShader(fragmentShader);

    if(!gl.getShaderParameter(fragmentShader,gl.COMPILE_STATUS)){
        console.log(gl.getShaderInfoLog(fragmentShader));
    }

    var program, buffer;

    program = gl.createProgram();
    gl.attachShader(program,vertexShader);
    gl.attachShader(program,fragmentShader);
    gl.linkProgram(program);

    buffer = gl.createBuffer(gl.ARRAY_BUFFER);
    gl.bindBuffer(gl.ARRAY_BUFFER,buffer);
    gl.bufferData(gl.ARRAY_BUFFER,new Float32Array([0,0,0,1,0,0,1,1,0]),gl.STATIC_DRAW);


}

function update(){
    gl.clearColor(1,0,0,1);
    gl.clear();
}

FoamGLFW.newWithResources({setup:setup,update:update});