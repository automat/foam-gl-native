var FoamGLFW_ = require('bindings')('foam_gl_native');

console.log(FoamGLFW_);

/*---------------------------------------------------------------------------------------------------------*/
// DEFINES
/*---------------------------------------------------------------------------------------------------------*/

var DEFAULT_WIDTH  = 800,
    DEFAULT_HEIGHT = 600,
    DEFAULT_TITLE  = ' ';

/*---------------------------------------------------------------------------------------------------------*/
// VARIABLES
/*---------------------------------------------------------------------------------------------------------*/
var windowPtr;

/*---------------------------------------------------------------------------------------------------------*/
// CONTEXT
/*---------------------------------------------------------------------------------------------------------*/

var appInstance = null;

var FoamGLFW = {};



function App(){
    this.__secondsElapsed = -1;

}

App.prototype.setWindow = function(width,height,title){
    if(windowPtr !== undefined){
        throw new Error('FoamGLFW already initialized.');
    }
    width  = width  === undefined ? DEFAULT_WIDTH : width;
    height = height === undefined ? DEFAULT_HEIGHT : height;
    title  = title  === undefined ? DEFAULT_TITLE : title;
    windowPtr = FoamGLFW_.init(width,height,title);
};

App.prototype.getSecondsElpased = function () {
    return this.__secondsElapsed;
};
App.prototype.getScreenSize = function(){
    return FoamGLFW_.getScreenSize();
};

App.prototype.getCursorPos = function(){
    return FoamGLFW_.getCursorPos();
};

App.prototype.setCursorPos = function(x,y) {
    FoamGLFW_.setCursorPos(x,y);
};

App.prototype.setWindowSize = function(width,height){
    FoamGLFW_.setWindowSize(width,height);
};

App.prototype.getWindowSize = function(){
    return FoamGLFW_.getWindowSize();
};

App.prototype.setWindowPos = function(x,y){
    x = x || 0; y = y || 0;
    FoamGLFW_.setWindowPos(x,y);
};

App.prototype.getWindowPos = function(){
    return FoamGLFW_.getWindowPos();
};

App.prototype.get = function () {
    return appInstance;
};

App.prototype.setup = function () {
    throw new Error('App setup not implemented.');
};
App.prototype.update = function () {
    throw new Error('App update not implemented');
};
App.prototype.updateFixed = function(){};



/*---------------------------------------------------------------------------------------------------------*/
// EXPORT
/*---------------------------------------------------------------------------------------------------------*/

FoamGLFW.newWithResources = function(obj){
    function App_(){
        App.call(this);
    }
    App_.prototype = Object.create(App.prototype);

    for(var p in obj){
        if(obj.hasOwnProperty(p)){
            App_.prototype[p] = obj[p];
        }
    }
    appInstance = new App_();
    appInstance.__secondsElapsed = 0;

    appInstance.setup();

    while(!FoamGLFW_.windowShouldClose()){
        appInstance.__secondsElapsed = FoamGLFW_.getTime();
        appInstance.update();

        FoamGLFW_.pollEvents();
        FoamGLFW_.swapBuffers();
    }

    FoamGLFW_.terminate();
};

// FOR IDE INSPECTION
var gl = {};

gl.clearColor = FoamGLFW_.clearColor;
gl.clear      = FoamGLFW_.clear;

gl.attachShader = FoamGLFW_.attachShader;
gl.bindAttribLocation = FoamGLFW_.bindAttribLocation;
gl.compileShader = FoamGLFW_.compileShader;
gl.createProgram = FoamGLFW_.createProgram;
gl.createShader  = FoamGLFW_.createShader;
gl.deleteProgram = FoamGLFW_.deleteProgram;
gl.deleteShader  = FoamGLFW_.deleteShader;
gl.detachShader  = FoamGLFW_.detachShader;
//gl.getAttachedShaders = FoamGLFW_.getAttachedShaders;
gl.getProgramParameter = FoamGLFW_.getProgramParameter;
gl.getProgramInfoLog   = FoamGLFW_.getProgramInfoLog;
gl.getShaderParameter  = FoamGLFW_.getShaderParameter;
gl.getShaderInfoLog    = FoamGLFW_.getShaderInfoLog;
gl.getShaderSource     = FoamGLFW_.getShaderSource;

gl.isProgram      = FoamGLFW_.isProgram;
gl.isShader       = FoamGLFW_.isShader;
gl.linkProgram    = FoamGLFW_.linkProgram;
gl.shaderSource   = FoamGLFW_.shaderSource;
gl.useProgram     = FoamGLFW_.useProgram;
gl.validateShader = FoamGLFW_.validateShader;

gl.createBuffer  = FoamGLFW_.createBuffer;
gl.bindBuffer    = FoamGLFW_.bindBuffer;
gl.bufferData    = FoamGLFW_.bufferData;
gl.bufferSubData = FoamGLFW_.bufferSubData;
gl.deleteBuffer  = FoamGLFW_.deleteBuffer;
//gl.getBufferParameter = FoamGLFW_.getBufferParameter;
gl.isBuffer      = FoamGLFW_.isBuffer;

gl.VERTEX_SHADER = FoamGLFW_.VERTEX_SHADER;
gl.FRAGMENT_SHADER = FoamGLFW_.FRAGMENT_SHADER;
gl.COMPILE_STATUS = FoamGLFW_.COMPILE_STATUS;

gl.ARRAY_BUFFER = FoamGLFW_.ARRAY_BUFFER;
gl.ELEMENT_ARRAY_BUFFER = FoamGLFW.ELEMENT_ARRAY_BUFFER;
gl.STATIC_DRAW  = FoamGLFW_.STATIC_DRAW;
gl.DYNAMIC_DRAW = FoamGLFW_.DYNAMIC_DRAW;



FoamGLFW.gl = gl;
module.exports = FoamGLFW;