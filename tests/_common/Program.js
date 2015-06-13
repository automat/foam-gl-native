var gl = require('../../index.js').gl;

var currProgram, prevProgram;

currProgram = null;
prevProgram = null;

function Program(vertexShader, fragmentShader) {
    this._obj = null;
    this._attributes = this._uniforms = null;
}

Program.prototype.dispose = function(){
    if(!this._obj){
        return this;
    }
    this._gl.deleteProgram(this._obj);
    this._obj = null;
    return this;
}

Program.prototype.load = function(vertexShader,fragmentShader){
    if(!vertexShader){
        throw new Error('No shader passed.');
    }
    this.dispose();

    var program;
    var prefixVertexShader, prefixFragmentShader;
    var vertShader,fragShader;

    program = gl.createProgram();
    vertShader = gl.createShader(gl.VERTEX_SHADER);
    fragShader = gl.createShader(gl.FRAGMENT_SHADER);

    if(!fragmentShader){
        prefixVertexShader = '#define VERTEX_SHADER\n';
        prefixFragmentShader = '#define FRAGMENT_SHADER\n';
        fragmentShader = vertexShader;
    } else {
        prefixVertexShader = prefixFragmentShader = '';
    }

    gl.shaderSource(vertShader, prefixVertexShader + vertexShader);
    gl.compileShader(vertShader);

    if (!gl.getShaderParameter(vertShader, gl.COMPILE_STATUS)) {
        throw new Error('VERTEX: ' + gl.getShaderInfoLog(vertShader));
    }

    gl.shaderSource(fragShader, prefixFragmentShader + fragmentShader);
    gl.compileShader(fragShader);

    if (!gl.getShaderParameter(fragShader, gl.COMPILE_STATUS)) {
        throw new Error('FRAGMENT: ' + gl.getShaderInfoLog(fragShader));
    }

    gl.attachShader(program, vertShader);
    gl.attachShader(program, fragShader);
    gl.linkProgram(program);

    var paramName;
    var objects, numObjects;

    numObjects = this._numUniforms = gl.getProgramParameter(program,gl.ACTIVE_UNIFORMS);
    objects = this._uniforms = {};
    for(var i = 0;  i < numObjects; ++i){
        paramName = gl.getActiveUniform(program,i).name;
        console.log(paramName);
    }

    console.log(numObjects);

    numObjects = this._numAttributes = gl.getProgramParameter(program,gl.ACTIVE_ATTRIBUTES);
    objects = this._attributes = {};
    for(var i = 0; i < numObjects; ++i){
        paramName = gl.getActiveAttrib(program,i).name;
        console.log(paramName);
    }

    console.log(numObjects);

}

module.exports = Program;