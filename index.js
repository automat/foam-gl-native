var FoamGLNative_ = require('bindings')('foam_gl_native');
var glfw = FoamGLNative_.glfw;
var gl_  = FoamGLNative_.gl;

/*---------------------------------------------------------------------------------------------------------*/
// DEFINES
/*---------------------------------------------------------------------------------------------------------*/

var DEFAULT_WIDTH  = 800,
    DEFAULT_HEIGHT = 600,
    DEFAULT_TITLE  = ' ';

/*---------------------------------------------------------------------------------------------------------*/
// CONTEXT
/*---------------------------------------------------------------------------------------------------------*/

var contextRef = null;
var windowPtr  = null;

function ContextGLNative(){
    this.__secondsElapsed = -1;
}

ContextGLNative.testSetup = function(){
    glfw.testSetup();
};

ContextGLNative.prototype.initWindow = function(width,height,title){
    if (windowPtr !== null) {
        throw new Error('ContextGLNative already initialized.');
    }
    width = width === undefined ? DEFAULT_WIDTH : width;
    height = height === undefined ? DEFAULT_HEIGHT : height;
    title = title === undefined ? DEFAULT_TITLE : title;

    windowPtr = glfw.init(width, height, title);
};

ContextGLNative.prototype.getSecondsElpased = function () {
    return this.__secondsElapsed;
};
ContextGLNative.prototype.getScreenSize = function(){
    return glfw.getScreenSize();
};

ContextGLNative.prototype.getCursorPos = function(){
    return glfw.getCursorPos();
};

ContextGLNative.prototype.setCursorPos = function(x,y) {
    glfw.setCursorPos(x,y);
};

ContextGLNative.prototype.setWindowSize = function(width,height){
    glfw.setWindowSize(width,height);
};

ContextGLNative.prototype.getWindowSize = function(){
    return glfw.getWindowSize();
};

ContextGLNative.prototype.setWindowPos = function(x,y){
    x = x || 0; y = y || 0;
    glfw.setWindowPos(x,y);
};

ContextGLNative.prototype.getWindowPos = function(){
    return glfw.getWindowPos();
};

ContextGLNative.prototype.get = function () {
    return contextRef;
};

ContextGLNative.prototype.setup = function () {
    throw new Error('Context setup not implemented.');
};

ContextGLNative.prototype.update = function () {
    throw new Error('Context update not implemented');
};

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

var gl = ContextGLNative.gl = {};

/*--------------------------------------------------------------------------------------------*/
// EXPORT FOR IDE INSPECTION
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

//PROGRAMS AND SHADERS

gl.viewport =
gl.depthRange =
gl.scissor = null;

gl.VERTEX_SHADER =
gl.FRAGMENT_SHADER =
gl.COMPILE_STATUS =
gl.LINK_STATUS = null;

//fragment shaders

gl.bindFragDataLocation =
gl.bindFragDataLocationIndex =
gl.getFragDataLocation =
gl.getFragDataIndex = null;

gl.createShader =
gl.attachShader =
gl.bindAttribLocation =
gl.compileShader =
gl.createProgram =
gl.deleteProgram =
gl.deleteShader =
gl.detachShader = null;

gl.getProgramParameter =
gl.getProgramInfoLog =
gl.getShaderParameter=
gl.getShaderInfoLog =
gl.getShaderSource =
gl.isProgram =
gl.isShader =
gl.linkProgram =
gl.shaderSource =
gl.useProgram =
gl.validateProgram = null;

//UNIFORMS AND ATTRIBUTES

gl.disableVertexAttribArray =
gl.enableVertexAttribArray =
gl.getAttribLocation =
gl.getUniformLocation =
gl.uniform1f =
gl.uniform2f =
gl.uniform3f =
gl.uniform4f =
//gl.uniform1fv =
//gl.uniform2fv =
//gl.uniform3fv =
//gl.uniform4fv =
gl.uniform1i =
gl.uniform2i =
gl.uniform3i =
gl.uniform4i =
gl.uniformMatrix2fv =
gl.uniformMatrix3fv =
gl.uniformMatrix4fv =
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