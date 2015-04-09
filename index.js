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

/*--------------------------------------------------------------------------------------------*/
// OPENGL ERROR
/*--------------------------------------------------------------------------------------------*/

gl.getError = function(){};

/*--------------------------------------------------------------------------------------------*/
// VERTEX ARRAYS
/*--------------------------------------------------------------------------------------------*/

gl.vertexPointer = function(size,type,stride,pointer){};
gl.normalPointer = function(type,stride,pointer){};
gl.colorPointer = function(size,type,stride,pointer){};
gl.secondaryColorPointer = function(size,type,stride,pointer){};
gl.indexPointer = function(type,stride,pointer){};
gl.edgeFlagPointer = function(stride,pointer){};
gl.fogCoordPointer = function(type,stride,pointer){};
gl.texCoordPointer = function(size,type,stride,pointer){};
gl.vertexAttribPointer = function(index,size,type,normalized,stride,pointer){};
gl.vertexAttribIPointer = function(index,size,type,stride,pointer){};
gl.vertexAttribLPointer = function(index,size,type,stride,pointer){};
gl.enableClientState = function(array){};
gl.disableClientState = function(array){};
gl.vertexAttribDivisor = function(index,divisor){};
gl.clientActiveTexture = function(texture){};
gl.arrayElement = function(i){};

//region DRAWING COMMANDS
gl.drawArrays = function(mode,first,count){};
gl.drawArraysInstanced = function(mode,first,count,primcount){};
gl.drawElements = function(mode,count,type,indices){};
gl.drawRangeElements = function(mode,start,end,count,type,indices){};
//endregion

/*--------------------------------------------------------------------------------------------*/
// VERTEX SPECIFICATIONS
/*--------------------------------------------------------------------------------------------*/


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
gl.DEPTH_BUFFER_BIT = null;

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

/*--------------------------------------------------------------------------------------------*/
// RASTERIZATION
/*--------------------------------------------------------------------------------------------*/

//region MULTISAMPLING
gl.getMultisamplefv = function(pname,index){};
gl.minSampleShading = function(value){};
//endregion

//region POINTS
gl.pointSize = function(size){};
gl.pointParameteri = function(pname,param){};
gl.pointParameterf = function(pname,param){};
//pointParameteriv
//pointParameterfv
//endregion

//region LINE SEGMENTS
gl.lineWidth = function(size){};
gl.lineStipple = function(factor,pattern){};
//endregion

//region POLYGONS
gl.frontFace = function(dir){};
gl.cullFace = function(mode){};
//endregion

//region STIPPLING
//endregion

//region POLYGON RASTERIZATION & DEPTH OFFSET
gl.polygonOffset = function(factor,units){};
//endregion

//region PIXEL STORAGE MODES
gl.pixelStorei = function(pname,param){};
gl.pixelStoref = function(pname,param){};
//endregion

//region COLOR TABLE SPECIFIATION
//endregion

//region RASTERIZATION OF PIXEL RECTANGLES
//endregion

//region PIXEL TRANSFER OPERATIONS
//endregion

//region BITMAPS
//endregion

/*--------------------------------------------------------------------------------------------*/
// WHOLE FRAMEBUFFER
/*--------------------------------------------------------------------------------------------*/

//region SELECTING A BUFFER FOR WRITING
gl.drawBuffer = function(buf){};
//endregion

//region FINE CONTROL OF BUFFER UPDATES
gl.indexMask = function(mask){};
gl.colorMask = function(r,g,b,a){};
gl.colorMaski = function(buf,r,g,b,a){};
gl.depthMask = function(mask){};
gl.stencilMask = function(mask){};
gl.stencilMaskSeparate = function(face,mask){};
//endregion

//region CLEARING THE BUFFERS
gl.clear = function(buf){};
gl.clearColor = function(r,g,b,a){};
gl.clearIndex = function(index){};
gl.clearDepth = function(d){};
gl.clearStencil = function(d){};
gl.clearAccum = function(r,g,b,a){};
gl.clearBufferfi = function(buffer,drawbuffer,depth,stencil){};
//endregion

//region ACCUMULATION BUFFER
gl.accum = function(op,value){};
//endregion

/*--------------------------------------------------------------------------------------------*/
// COLOR SUM, FOG AND HINTS
/*--------------------------------------------------------------------------------------------*/

gl.fogi = function(pname,param){};
gl.fogf = function(pname,param){};

/*--------------------------------------------------------------------------------------------*/
// TEXTURING
/*--------------------------------------------------------------------------------------------*/

gl.activeTexture = function(texture){};

//region TEXTURE_OBJECTS
gl.bindTexture = function(target,texture){};
gl.createTexture = function(){};
gl.deleteTexture = function(texture){};
//endregion

//region SAMPLER OBJECTS
gl.createSampler = function(){};
gl.bindSampler = function(unit,sampler){};
gl.deleteSampler = function(sampler){};
//endregion

//region TEXTURE IMAGE SPEC
//endregion

//region ALTERNATE TEXTURE IMAGE SPEC
gl.copyTexImage2D = function(target,level,internalformat,x,y,width,height,border){};
gl.copyTexImage1D = function(target,level,internalformat,x,y,width,border){};
gl.copyTexSubImage3D = function(target,level,xoffset,yoffset,zoffset,x,y,width,height){};
gl.copyTexSubImage2D = function(target,level,xoffset,yoffset,x,y,width,height){};
gl.copyTexSubImage1D = function(target,level,xoffset,x,y,width){};
//endregio

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




for(var p in gl){
    gl[p] = gl_[p];
}

/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

module.exports = ContextGLNative;