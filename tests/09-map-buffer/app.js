var fs   = require('fs');
var path = require('path');

var Utils   = require('../_common/Utils');
var Context = require('../../index.js');
var gl = Context.gl;


function setup(){
    this.initWindow(800,600);

    var vbo = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER,vbo);

    var num = 100;
    var data = new Float32Array(num);
    for(var i = 0; i < num; ++i){
        data[i] = i;
    }

    gl.bufferData(gl.ARRAY_BUFFER, data, gl.STATIC_DRAW);

    data = gl.mapBuffer(gl.ARRAY_BUFFER,gl.WRITE_ONLY,gl.FLOAT,num);
    for(var i = 0; i < num; ++i){
        data[i] = num - i;
    }
    gl.unmapBuffer(gl.ARRAY_BUFFER);

    data = gl.mapBuffer(gl.ARRAY_BUFFER,gl.READ_ONLY,gl.FLOAT,num);
    console.log(data);
    gl.unmapBuffer(gl.ARRAY_BUFFER);

    data = gl.mapBufferRange(gl.ARRAY_BUFFER,0,10,gl.MAP_READ_BIT | gl.MAP_WRITE_BIT,gl.FLOAT);
    data[0] = data[4] = data[8] = 1234.567;
    console.log(data);
    gl.unmapBuffer(gl.ARRAY_BUFFER);

    gl.deleteBuffer(vbo);
}

function update(){}

Context.new({setup:setup,update:update});



























