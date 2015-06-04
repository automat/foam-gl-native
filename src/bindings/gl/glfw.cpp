#include <nan.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glfw.h"
#include "../utils/utils.h"

using namespace std;
using namespace glfw;

/*---------------------------------------------------------------------------------------------------------*/
// STATES
/*---------------------------------------------------------------------------------------------------------*/

bool initialized = false;
//optional window ref from js, multiple windows, for later...
uint64_t windowPtr = 0;


void errorCallback(int error, const char* description){
    fputs(description, stderr);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
//
/*---------------------------------------------------------------------------------------------------------*/

NAN_METHOD(getTime){
    NanScope();
    NanReturnValue(V8_NUM(glfwGetTime()));
}

NAN_METHOD(pollEvents){
    NanScope();
    glfwPollEvents();
    NanReturnUndefined();
}

NAN_METHOD(swapInterval){
    NanScope();
    glfwSwapInterval(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(swapBuffers){
    NanScope();
    glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(windowPtr));
    NanReturnUndefined();
}

NAN_METHOD(terminate){
    NanScope();
    glfwTerminate();
    NanReturnUndefined();
}

NAN_METHOD(destroyWindow){
    NanScope();
    NanReturnUndefined();
}

/*---------------------------------------------------------------------------------------------------------*/
// INPUT
/*---------------------------------------------------------------------------------------------------------*/

NAN_METHOD(setCursorPos){
    NanScope();
    glfwSetCursorPos(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            args[0]->NumberValue(),
            args[1]->NumberValue());
    NanReturnUndefined();
}

NAN_METHOD(getCursorPos){
    NanScope();
    double x,y;
    glfwGetCursorPos(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            &x, &y);
    Local<Array> out = Array::New(2);
    out->Set(0, V8_NUM(x));
    out->Set(1, V8_NUM(y));
    NanReturnValue(out);
}

/*---------------------------------------------------------------------------------------------------------*/
// WINDOW & SCREEN
/*---------------------------------------------------------------------------------------------------------*/

NAN_METHOD(windowShouldClose){
    NanScope();
    NanReturnValue(V8_BOOL((bool)glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(windowPtr))));
}

NAN_METHOD(getScreenSize){
    NanScope();
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    Local<Array> out = Array::New(2);
    out->Set(0, V8_INT(mode->width));
    out->Set(1, V8_INT(mode->height));
    NanReturnValue(out);
}

NAN_METHOD(setWindowSize){
    NanScope();
    glfwSetWindowSize(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            args[0]->Uint32Value(),
            args[1]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(getWindowSize){
    NanScope();
    int width, height;
    glfwGetWindowSize(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            &width,
            &height
    );
    Local<Array> out = Array::New(2);
    out->Set(0, V8_NUM(width));
    out->Set(1, V8_NUM(height));
    NanReturnValue(out);
}

NAN_METHOD(setWindowPos){
    NanScope();
    glfwSetWindowPos(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            args[0]->Uint32Value(),
            args[1]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(getWindowPos){
    NanScope();
    int x,y;
    glfwGetWindowPos(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            &x, &y);
    Local<Array> out = Array::New(2);
    out->Set(0, V8_NUM(x));
    out->Set(1, V8_NUM(y));
    NanReturnValue(out);
}

NAN_METHOD(getVersion){
    NanScope();
    int major, minor, rev;
    glfwGetVersion(&major,&minor,&rev);
    Local<Array> out = Array::New(3);
    out->Set(0, V8_INT(major));
    out->Set(1, V8_INT(major));
    out->Set(2, V8_INT(major));
    NanReturnValue(out);
}

NAN_METHOD(iconifyWindow){
    NanScope();
    glfwIconifyWindow(reinterpret_cast<GLFWwindow*>(windowPtr));
    NanReturnUndefined();
}

NAN_METHOD(restoreWindow){
    NanScope();
    glfwRestoreWindow(reinterpret_cast<GLFWwindow*>(windowPtr));
    NanReturnUndefined();
}

NAN_METHOD(getFrameBufferSize){
    NanScope();
    int width, height;
    glfwGetFramebufferSize(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            &width,
            &height
    );
    Local<Array> out = Array::New(2);
    out->Set(0, V8_NUM(width));
    out->Set(1, V8_NUM(height));
    NanReturnValue(out);
}

NAN_METHOD(getDPI){
    NanScope();
    int windowWidth, windowHeight;
    int frameBufferWidth, frameBufferHeight;

    GLFWwindow* window = reinterpret_cast<GLFWwindow *>(windowPtr);
    glfwGetWindowSize(window,&windowWidth,&windowHeight);
    glfwGetFramebufferSize(window,&frameBufferWidth,&frameBufferHeight);

    float dpi = (float)frameBufferWidth / (float)windowWidth;

    NanReturnValue(V8_NUM(dpi));
}

/*--------------------------------------------------------------------------------------------*/
// INIT
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(init_) {
    NanScope();

    if(initialized){
        NanThrowError("GLFW already initialized.");
    }

    glfwInit();
    initialized = true;

    NanReturnUndefined();
}

NAN_METHOD(createWindow){
    NanScope();

    CHECK_ARGS_LEN(5);

    int windowWidth = args[0]->Int32Value();
    int windowHeight = args[1]->Int32Value();
    string windowTitle = *String::Utf8Value(args[2]->ToString());
    GLboolean windowIsResizable = static_cast<GLboolean>(args[3]->BooleanValue());
    int numSamples = args[4]->Int32Value();

    atexit(glfwTerminate);

    glfwSetErrorCallback(errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, windowIsResizable);
    glfwWindowHint(GLFW_SAMPLES, numSamples);

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);

    if(!window){
        glfwTerminate();
        return NanThrowError("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    glewExperimental = GL_TRUE;
    glewInit();

    initialized = true;
    windowPtr   = (uint64_t) window;

    NanReturnValue(V8_NUM(windowPtr));
}

/*--------------------------------------------------------------------------------------------*/
// TEST SETUP
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(testSetup){
    NanScope();
    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

void glfw::init(Handle<Object> exports){
    NODE_SET_METHOD(exports,"init", init_);
    EXPORT_SET_METHOD(terminate);
    EXPORT_SET_METHOD(createWindow);

    EXPORT_SET_METHOD(pollEvents);
    EXPORT_SET_METHOD(swapBuffers);
    EXPORT_SET_METHOD(swapInterval);

    EXPORT_SET_METHOD(setCursorPos);
    EXPORT_SET_METHOD(getCursorPos);

    EXPORT_SET_METHOD(windowShouldClose);
    EXPORT_SET_METHOD(getScreenSize);
    EXPORT_SET_METHOD(setWindowSize);
    EXPORT_SET_METHOD(getWindowSize);
    EXPORT_SET_METHOD(setWindowPos);
    EXPORT_SET_METHOD(getWindowPos);
    EXPORT_SET_METHOD(iconifyWindow);
    EXPORT_SET_METHOD(restoreWindow);

    EXPORT_SET_METHOD(getFrameBufferSize);
    EXPORT_SET_METHOD(getDPI);

    EXPORT_SET_METHOD(getTime);

    EXPORT_SET_METHOD(getVersion);

    EXPORT_SET_METHOD(testSetup);
}
