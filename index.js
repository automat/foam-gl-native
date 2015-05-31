var ContextGLNative_ = require('bindings')('foam_gl_native');
var glfw = ContextGLNative_.glfw;
var gl_  = require('./src/context/gl');
var glDraw_ = require('./src/context/glDraw');

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

ContextGLNative.prototype.getWindowAspectRatio = function(){
    var windowSize = this.getWindowSize();
    return windowSize[0] / windowSize[1];
}

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

    //will change
    while(!glfw.windowShouldClose()){
        contextRef.__secondsElapsed = glfw.getTime();
        contextRef.update();

        glfw.pollEvents();
        glfw.swapBuffers();
    }

    contextRef.destroy();

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

ContextGLNative.prototype.destroy = function(){};

/*--------------------------------------------------------------------------------------------*/
// GL BINDINGS
/*--------------------------------------------------------------------------------------------*/

ContextGLNative.gl = gl_;

/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

module.exports = ContextGLNative;