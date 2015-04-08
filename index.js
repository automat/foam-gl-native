var ContextGLNative_ = require('bindings')('foam_gl_native');
var glfw = ContextGLNative_.glfw;
var gl_  = ContextGLNative_.gl;

/*---------------------------------------------------------------------------------------------------------*/
// DEFINES
/*---------------------------------------------------------------------------------------------------------*/

var DEFAULT_WIDTH  = 800,
    DEFAULT_HEIGHT = 600,
    DEFAULT_TITLE  = ' ',
    DEFAULT_WINDOW_RESIZABLE = false,
    DEFAULT_WINDOW_NUM_SAMPLES = 2;

/*---------------------------------------------------------------------------------------------------------*/
// CONTEXT SETUP & WINDOW
/*---------------------------------------------------------------------------------------------------------*/

var contextRef = null;
var windowPtr  = null;

/**
 * ContextGLNative – GLFW & OpenGL bindings wrapper
 * @constructor
 */

function ContextGLNative(){
    this.__secondsElapsed = -1;
}

ContextGLNative.testSetup = function(){
    glfw.testSetup();
};

/**
 * Creates a new GLFW Window and sets up the OpenGL context.
 * @param {Number} [width]
 * @param {Number} [height]
 * @param {String} [title]
 * @param {Boolean} [resizable]
 * @param {Number} [numSamples]
 */

ContextGLNative.prototype.initWindow = function(width,height,title,resizable,numSamples){
    if (windowPtr !== null) {
        throw new Error('ContextGLNative already initialized.');
    }
    width      = width === undefined ? DEFAULT_WIDTH : width;
    height     = height === undefined ? DEFAULT_HEIGHT : height;
    title      = title === undefined ? DEFAULT_TITLE : title;
    resizable  = resizable === undefined ? DEFAULT_WINDOW_RESIZABLE : resizable;
    numSamples = numSamples === undefined ? DEFAULT_WINDOW_NUM_SAMPLES : numSamples;

    windowPtr = glfw.init(width, height, title, resizable, numSamples);
};

/**
 * Returns the seconds elapsed since context start.
 * @returns {Number}
 */

ContextGLNative.prototype.getSecondsElapsed= function () {
    return this.__secondsElapsed;
};

/**
 * Returns the screen size the window is positioned in.
 * @returns {Array}
 */

ContextGLNative.prototype.getScreenSize = function(){
    return glfw.getScreenSize();
};

/**
 * Returns the current cursor position within the window.
 * @returns {Array}
 */

ContextGLNative.prototype.getCursorPos = function(){
    return glfw.getCursorPos();
};

/**
 * Sets the current cursor position within the window.
 * @param {Number} x
 * @param {Number} y
 */

ContextGLNative.prototype.setCursorPos = function(x,y) {
    glfw.setCursorPos(x,y);
};

/**
 * Sets the created windows dimensions.
 * @param {Number} width
 * @param {Number} height
 */

ContextGLNative.prototype.setWindowSize = function(width,height){
    glfw.setWindowSize(width,height);
};

/**
 * Returns the windows dimensions.
 * @returns {Array}
 */

ContextGLNative.prototype.getWindowSize = function(){
    return glfw.getWindowSize();
};

/**
 * Sets the windows position on screen.
 * @param {Number} x
 * @param {Number} y
 */

ContextGLNative.prototype.setWindowPos = function(x,y){
    x = x || 0; y = y || 0;
    glfw.setWindowPos(x,y);
};

/**
 * Returns the windows current position on screen.
 * @returns {Array}
 */

ContextGLNative.prototype.getWindowPos = function(){
    return glfw.getWindowPos();
};

/**
 * Minimizes the window.
 */

ContextGLNative.prototype.iconifyWindow = function(){
    glfw.iconifyWindow();
};

/**
 * Restores the window if minimized.
 */

ContextGLNative.prototype.restoreWindow = function() {
    glfw.restoreWindow();
};

/**
 * Returns a reference to the ContextGLNative instance.
 * @returns {ContextGLNative}
 */

ContextGLNative.prototype.get = function () {
    return contextRef;
};

/**
 * Returns the major, minor, rev information about the created OpenGL context.
 * @returns {Array}
 */

ContextGLNative.prototype.getVersion = function(){
    return glfw.getVersion();
};

/**
 * Initializes ContextNativeGL
 * @param {Object} obj - The App object, must implement setup & draw
 */

ContextGLNative.new = function(obj){
    function Context(){
        ContextGLNative.call(this);
    }
    Context.prototype = Object.create(ContextGLNative.prototype);

    for(var p in obj){
        if(obj.hasOwnProperty(p)){
            Context.prototype[p] = obj[p];
        }
    }
    contextRef = new Context();
    contextRef.__secondsElapsed = 0;

    contextRef.setup();

    while(!glfw.windowShouldClose()){
        contextRef.__secondsElapsed = glfw.getTime();
        contextRef.update();

        glfw.pollEvents();
        glfw.swapBuffers();
    }

    glfw.terminate();
};

/*---------------------------------------------------------------------------------------------------------*/
// CONTEXT SETUP & WINDOW
/*---------------------------------------------------------------------------------------------------------*/

ContextGLNative.prototype.setup = function () {
    throw new Error('ContextGLNative setup not implemented.');
};

ContextGLNative.prototype.update = function () {
    throw new Error('ContextGLNative update not implemented');
};

/*--------------------------------------------------------------------------------------------*/
// GL BINDINGS
/*--------------------------------------------------------------------------------------------*/

var gl = ContextGLNative.gl = {};


gl.FLOAT =
gl.VERTEX_PROGRAM_POINT_SIZE = null;

//PER FRAGMENT OPERATIONS

gl.NEVER =
gl.ALWAYS =
gl.LESS =
gl.EQUAL =
gl.LEQUAL =
gl.GREATER =
gl.GEQUAL =
gl.NOTEQUAL = null;

gl.depthFunc = null;

//SPECIAL FUNCTIONS

gl.ARRAY_BUFFER_BINDING =
gl.CURRENT_PROGRAM = null;

gl.disable =
gl.enable =
gl.finish =
gl.flush =
gl.getParameter =
gl.hint =
gl.isEnabled =
gl.pixelStorei = null;

//FRAMEBUFFER

gl.COLOR_BUFFER_BIT =
gl.DEPTH_BUFFER_BIT =
gl.clearColor =
gl.clear =
gl.clearDepth =
gl.clearColor =
gl.clearStencil =
gl.colorMask =
gl.depthMask =
gl.stencilMask =
gl.stencilMaskSeparate = null;

gl.DEPTH_TEST = null;

/*--------------------------------------------------------------------------------------------*/
// PROGRAMS AND SHADERS
/*--------------------------------------------------------------------------------------------*/

//region SHADER OBJECTS
gl.createShader = function(type){};
gl.shaderSource = function(shader,string){};
gl.compileShader = function(shader){};
gl.deleteShader = function(shader){};
//endregion

//region PROGRAM OBJECTS
gl.createProgram = function(){};
gl.attachShader = function(program,shader){};
gl.detachShader = function(program,shader){};
gl.linkProgram = function(program){};
gl.useProgram = function(program){};
gl.getProgramParameter = function(program,pname){};
gl.deleteProgram = function(program){};
//endregion

//region PROGRAM PIPELINE OBJECTS
gl.createProgramPipeline = function(){};
gl.deleteProgramPipeline = function(pipeline){};
gl.bindProgramPipeline = function(pipeline){};
gl.useProgramStages = function(pipeline,stages,program){};
gl.activeShaderProgram = function(pipeline,program){};
//endregion

//region VERTEX ATTRIBUTES
gl.getAttribLocation = function(program,name){};
gl.bindAttribLocation = function(program,name){};
//endregion

//region UNIFORM VARIABLES
gl.getUniformLocation = function(program,name){};
gl.uniform1f = function(location,x){};
gl.uniform2f = function(location,x,y){};
gl.uniform3f = function(location,x,y,z){};
gl.uniform4f = function(location,x,y,z,w){};
gl.uniform1i = function(location,x){};
gl.uniform2i = function(location,x,y){};
gl.uniform3i = function(location,x,y,z){};
gl.uniform4i = function(location,x,y,z,w){};
gl.uniformMatrix2fv = function(location,transpose,value){};
gl.uniformMatrix3fv = function(location,transpose,value){};
gl.uniformMatrix4fv = function(location,transpose,value){};
//endregion

//region UNIFORM BUFFER OBJECTS BINDING
//uniformBlockBinding
//endregion

//region SUBROUTINE UNIFORM VARIABLES
//getSubroutineUniformLocation
//getSubroutineIndex
//getActiveSubroutineUniformiv
//getActiveSubroutineUniformName
//getActiveSubroutineName
//uniformSubroutinesuiv
//endregion

//region VARYING VARIABLES
//transformFeedbackVaryings
//getTransformFeedbackVarying
//endregion

//region SHADER EXECUTION
gl.validateProgram = function(program){};
gl.validateProgramPipeline = function(pipeline){};
//endregion

//region FRAGMENT SHADERS
gl.bindFragDataLocation = function(program,colorNumber,name){};
gl.bindFragDataLocationIndex = function(program,colorNumber,index,name){};
gl.getFragDataLocation = function(program,name){};
gl.getFragDataIndex = function(program,name){};
//endregion

/*--------------------------------------------------------------------------------------------*/
// SHADER AND PROGRAM QUERIES
/*--------------------------------------------------------------------------------------------*/

//region SHADER QUERIES
gl.isShader = function(shader){};
gl.getShaderInfoLog = function(shader){};
gl.getShaderSource = function(shader){};
//endregion

//region PROGRAM QUERIES
gl.isProgramPipeline = function(pipeline){};
gl.isProgram = function(program){};
gl.getProgramInfoLog = function(program){};
gl.getProgramPipelineInfoLog = function(program){};
gl.getShaderParameter = function(shader,pname){};
//endregion

gl.viewport =
gl.depthRange =
gl.scissor = null;

gl.VERTEX_SHADER =
gl.FRAGMENT_SHADER =
gl.COMPILE_STATUS =
gl.LINK_STATUS = null;

//fragment shaders

//UNIFORMS AND ATTRIBUTES

gl.disableVertexAttribArray =
gl.enableVertexAttribArray =
//gl.uniform1fv =
//gl.uniform2fv =
//gl.uniform3fv =
//gl.uniform4fv =
gl.vertexAttribPointer = null;

//BUFFER

gl.ARRAY_BUFFER =
gl.ELEMENT_ARRAY_BUFFER =
gl.STATIC_DRAW =
gl.DYNAMIC_DRAW =
gl.BUFFER_SIZE =
gl.BUFFER_USAGE = null;

gl.createBuffer =
gl.bindBuffer =
gl.bufferData =
gl.bufferSubData =
gl.deleteBuffer =
gl.getBufferParameter =
gl.isBuffer = null;


//VERTEX ARRAY OBJECTS

gl.createVertexArray =
gl.deleteVertexArray =
gl.bindVertexArray =
gl.isVertexArray = null;

//DRAW BUFFER WRITING
gl.POINTS =
gl.LINE_STRIP =
gl.LINE_LOOP =
gl.LINES =
gl.TRIANGLES =
gl.TRIANGLE_STRIP =
gl.TRIANGLE_FAN = null;

gl.drawArrays =
gl.drawElements =
gl.drawRangeElements = null;


for(var p in gl){
    gl[p] = gl_[p];
}

/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

module.exports = ContextGLNative;