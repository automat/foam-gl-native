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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
    glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(windowPtr));
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

/*--------------------------------------------------------------------------------------------*/
// INIT
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(init_) {
    atexit(glfwTerminate);

    NanScope();

    if(initialized){
        NanReturnValue(V8_NUM(windowPtr));
    }

    if(!glfwInit()){
        return NanThrowError("Failed to initialize GLFW.");
    }

    glfwSetErrorCallback(errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(
            args[0]->Uint32Value(),
            args[1]->Uint32Value(),
            *String::Utf8Value(args[2]->ToString()),
            NULL, NULL);

    if(!window){
        glfwTerminate();
        return NanThrowError("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

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
    if(initialized){
        NanThrowError("glfw test setup already initialized.");
    }
    atexit(glfwTerminate);

    GLFWwindow* window;
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        NanThrowError("Cant initialize glfw.");
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window) {
        NanThrowError("Cant create glfw window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

void glfw::init(Handle<Object> exports){
    NODE_SET_METHOD(exports,"init", init_);
    EXPORT_SET_METHOD(terminate);
    EXPORT_SET_METHOD(destroyWindow);

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

    EXPORT_SET_METHOD(getTime);

    EXPORT_SET_METHOD(testSetup);
}
