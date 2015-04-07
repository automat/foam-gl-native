var fs   = require('fs');
var path = require('path');

var Context = require('../../index.js');
var gl = Context.gl;

var glu = require('../_common/glu');
var Matrix44 = require('../_common/Matrix44');

function initProgram(){
    var vertexShader, fragmentShader, program;
    vertexShader    = gl.createShader(gl.VERTEX_SHADER);
    fragmentShader  = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(
        vertexShader,
        fs.readFileSync(path.join(__dirname,'vert.glsl'),{encoding:'utf8'})
    );
    gl.compileShader(vertexShader);

    if(!gl.getShaderParameter(vertexShader,gl.COMPILE_STATUS)){
        throw new Error('VERTEX ' + gl.getShaderInfoLog(vertexShader));
    }

    gl.shaderSource(
        fragmentShader,
        fs.readFileSync(path.join(__dirname,'frag.glsl'),{encoding:'utf8'})
    );
    gl.compileShader(fragmentShader);

    if(!gl.getShaderParameter(fragmentShader,gl.COMPILE_STATUS)){
        throw new Error('FRAGMENT ' + gl.getShaderInfoLog(fragmentShader));
    }

    program = gl.createProgram();
    gl.attachShader(program,vertexShader);
    gl.attachShader(program,fragmentShader);
    gl.linkProgram(program);

    if(!gl.getProgramParameter(program,gl.LINK_STATUS)){
        throw new Error('PROGRAM ' + gl.getProgramInfoLog(program));
    }

    return program;
}

function setup(){
    this.initWindow(800,600,'ContextGLNative');

    var width, height;
    var program, buffer;

    width  = this.getWindowSize()[0];
    height = this.getWindowSize()[1];

    program = this._program = initProgram();
    gl.useProgram(program);
    gl.enable(gl.VERTEX_PROGRAM_POINT_SIZE);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LESS);


    this._locationAttribPosition = gl.getAttribLocation(program,'vp');
    gl.enableVertexAttribArray(this._locationAttribPosition);

    buffer = this._buffer =  gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER,buffer);
    gl.bufferData(gl.ARRAY_BUFFER,
        new Float32Array([
            0.0,  0.5,  0.0,
            0.5, -0.5,  0.0,
            -0.5,-0.5,  0.0
        ]),
        gl.STATIC_DRAW
    );

    gl.viewport(0,0,width,height);
}

function update(){
    gl.clearColor(0,0,0,1);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.useProgram(this._program);
    gl.bindBuffer(gl.ARRAY_BUFFER,this._buffer);
    gl.vertexAttribPointer(this._locationAttribPosition,3,gl.FLOAT,false,0,0);
    gl.drawArrays(gl.TRIANGLES,0,3);
}

Context.new({setup:setup,update:update});