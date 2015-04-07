var ContextGLNative = require('../../index');

var gl = ContextGLNative.gl;

function setup(){
    this.initWindow(800,600,'Test Setup');

    console.log('OpenGL version',this.getVersion());

    var vertexSource =
        "#version 140\n" +
        "in vec2 position;\n" +
        "void main() {\n" +
        "   gl_Position = vec4(position, 0.0, 1.0);\n" +
        "}";

    var fragmentSource =
        "#version 140\n" +
        "out vec4 outColor;\n" +
        "uniform vec3 triangleColor;\n" +
        "void main() {\n" +
        "   outColor = vec4(triangleColor, 1.0);\n" +
        "}";

    var vao = gl.createVertexArray();
    gl.bindVertexArray(vao);

    var vbo = gl.createBuffer();

    var vertices = new Float32Array([
        0.0,0.5,
        0.5,-0.5,
        -0.5,-0.5
    ]);

    gl.bindBuffer(gl.ARRAY_BUFFER,vbo);
    gl.bufferData(gl.ARRAY_BUFFER,vertices,gl.STATIC_DRAW);

    var vertexShader = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vertexShader,vertexSource);
    gl.compileShader(vertexShader);

    if(!gl.getShaderParameter(vertexShader,gl.COMPILE_STATUS)){
        throw new Error('VERTEX SHADER: ' + gl.getShaderInfoLog(vertexShader))
    }

    var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fragmentShader,fragmentSource);
    gl.compileShader(fragmentShader);

    if(!gl.getShaderParameter(fragmentShader,gl.COMPILE_STATUS)){
        throw new Error('FRAGMENT SHADER: ' + gl.getShaderInfoLog(fragmentShader));
    }

    var program = gl.createProgram();
    gl.attachShader(program,vertexShader);
    gl.attachShader(program,fragmentShader);
    gl.bindFragDataLocation(program,0,'outColor');
    gl.linkProgram(program);
    gl.useProgram(program);

    if(!gl.getProgramParameter(program,gl.LINK_STATUS)){
        throw new Error('PROGRAM ' + gl.getProgramInfoLog(program));
    }

    var posAttrib = gl.getAttribLocation(program,'position');
    gl.enableVertexAttribArray(posAttrib);
    gl.vertexAttribPointer(posAttrib,2,gl.FLOAT,false,0,0);

    this._uniColor = gl.getUniformLocation(program,'triangleColor');
}

function update(){
    gl.uniform3f(this._uniColor,1,1,0.5 + Math.sin(this.getSecondsElapsed()) * 0.5);
    gl.clearColor(0,0,0,1);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.drawArrays(gl.TRIANGLES,0,3);
}

ContextGLNative.new({setup:setup,update:update});