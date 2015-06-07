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
    glfw.init();
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

    glfw.createWindow(width,height,title,resizable,numSamples);
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
 * Returns true if the key passed is pressed.
 * @param char
 */

ContextGLNative.prototype.isKeyDown = function(key){
    return glfw.isKeyDown(key);
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
 * Returns the windows frame buffer size,
 */

ContextGLNative.prototype.getFrameBufferSize = function(){
    return glfw.getFrameBufferSize();
};

/**
 * Returns the windows current monitor dpi.
 */

ContextGLNative.prototype.getDPI = function(){
    return glfw.getDPI();
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

ContextGLNative.KEY_UNKNOWN =
ContextGLNative.KEY_SPACE =
ContextGLNative.KEY_APOSTROPHE =
ContextGLNative.KEY_COMMA =
ContextGLNative.KEY_MINUS =
ContextGLNative.KEY_PERIOD =
ContextGLNative.KEY_SLASH =
ContextGLNative.KEY_0 =
ContextGLNative.KEY_1 =
ContextGLNative.KEY_2 =
ContextGLNative.KEY_3 =
ContextGLNative.KEY_4 =
ContextGLNative.KEY_5 =
ContextGLNative.KEY_6 =
ContextGLNative.KEY_7 =
ContextGLNative.KEY_8 =
ContextGLNative.KEY_9 =
ContextGLNative.KEY_SEMICOLON =
ContextGLNative.KEY_EQUAL =
ContextGLNative.KEY_A =
ContextGLNative.KEY_B =
ContextGLNative.KEY_C =
ContextGLNative.KEY_D =
ContextGLNative.KEY_E =
ContextGLNative.KEY_F =
ContextGLNative.KEY_G =
ContextGLNative.KEY_H =
ContextGLNative.KEY_I =
ContextGLNative.KEY_J =
ContextGLNative.KEY_K =
ContextGLNative.KEY_L =
ContextGLNative.KEY_M =
ContextGLNative.KEY_N =
ContextGLNative.KEY_O =
ContextGLNative.KEY_P =
ContextGLNative.KEY_Q =
ContextGLNative.KEY_R =
ContextGLNative.KEY_S =
ContextGLNative.KEY_T =
ContextGLNative.KEY_U =
ContextGLNative.KEY_V =
ContextGLNative.KEY_W =
ContextGLNative.KEY_X =
ContextGLNative.KEY_Y =
ContextGLNative.KEY_Z =
ContextGLNative.KEY_LEFT_BRACKET =
ContextGLNative.KEY_BACKSLASH =
ContextGLNative.KEY_RIGHT_BRACKET =
ContextGLNative.KEY_GRAVE_ACCENT =
ContextGLNative.KEY_WORLD_1 =
ContextGLNative.KEY_WORLD_2 =
ContextGLNative.KEY_ESCAPE =
ContextGLNative.KEY_ENTER =
ContextGLNative.KEY_TAB =
ContextGLNative.KEY_BACKSPACE =
ContextGLNative.KEY_INSERT =
ContextGLNative.KEY_DELETE =
ContextGLNative.KEY_RIGHT =
ContextGLNative.KEY_LEFT =
ContextGLNative.KEY_DOWN =
ContextGLNative.KEY_UP =
ContextGLNative.KEY_PAGE_UP =
ContextGLNative.KEY_PAGE_DOWN =
ContextGLNative.KEY_HOME =
ContextGLNative.KEY_END =
ContextGLNative.KEY_CAPS_LOCK =
ContextGLNative.KEY_SCROLL_LOCK =
ContextGLNative.KEY_NUM_LOCK =
ContextGLNative.KEY_PRINT_SCREEN =
ContextGLNative.KEY_PAUSE =
ContextGLNative.KEY_F1 =
ContextGLNative.KEY_F2 =
ContextGLNative.KEY_F3 =
ContextGLNative.KEY_F4 =
ContextGLNative.KEY_F5 =
ContextGLNative.KEY_F6 =
ContextGLNative.KEY_F7 =
ContextGLNative.KEY_F8 =
ContextGLNative.KEY_F9 =
ContextGLNative.KEY_F10 =
ContextGLNative.KEY_F11 =
ContextGLNative.KEY_F12 =
ContextGLNative.KEY_F13 =
ContextGLNative.KEY_F14 =
ContextGLNative.KEY_F15 =
ContextGLNative.KEY_F16 =
ContextGLNative.KEY_F17 =
ContextGLNative.KEY_F18 =
ContextGLNative.KEY_F19 =
ContextGLNative.KEY_F20 =
ContextGLNative.KEY_F21 =
ContextGLNative.KEY_F22 =
ContextGLNative.KEY_F23 =
ContextGLNative.KEY_F24 =
ContextGLNative.KEY_F25 =
ContextGLNative.KEY_KP_0 =
ContextGLNative.KEY_KP_1 =
ContextGLNative.KEY_KP_2 =
ContextGLNative.KEY_KP_3 =
ContextGLNative.KEY_KP_4 =
ContextGLNative.KEY_KP_5 =
ContextGLNative.KEY_KP_6 =
ContextGLNative.KEY_KP_7 =
ContextGLNative.KEY_KP_8 =
ContextGLNative.KEY_KP_9 =
ContextGLNative.KEY_KP_DECIMAL =
ContextGLNative.KEY_KP_DIVIDE =
ContextGLNative.KEY_KP_MULTIPLY =
ContextGLNative.KEY_KP_SUBTRACT =
ContextGLNative.KEY_KP_ADD =
ContextGLNative.KEY_KP_ENTER =
ContextGLNative.KEY_KP_EQUAL =
ContextGLNative.KEY_LEFT_SHIFT =
ContextGLNative.KEY_LEFT_CONTROL =
ContextGLNative.KEY_LEFT_ALT =
ContextGLNative.KEY_LEFT_SUPER =
ContextGLNative.KEY_RIGHT_SHIFT =
ContextGLNative.KEY_RIGHT_CONTROL =
ContextGLNative.KEY_RIGHT_ALT =
ContextGLNative.KEY_RIGHT_SUPER =
ContextGLNative.KEY_MENU =
ContextGLNative.KEY_LAST =
ContextGLNative.MOD_SHIFT =
ContextGLNative.MOD_CONTROL =
ContextGLNative.MOD_ALT =
ContextGLNative.MOD_SUPER = null;

for(var p in glfw){
    if(typeof glfw[p] === 'function'){
        continue;
    }
    ContextGLNative[p] = glfw[p];
}

/*--------------------------------------------------------------------------------------------*/
// GL BINDINGS
/*--------------------------------------------------------------------------------------------*/

ContextGLNative.gl = gl_;

var writeImage = ContextGLNative.gl.writeImage;
ContextGLNative.gl.writeImage = function(path,xoffset,yoffset,width,height){
    xoffset = xoffset || 0;
    yoffset = yoffset || 0;

    //TODO: Fix me
    var size = glfw.getWindowSize();

    width  = width  || size[0];
    height = height || size[1];

    writeImage.call(this,path,xoffset,yoffset,width,height);
};

/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

module.exports = ContextGLNative;