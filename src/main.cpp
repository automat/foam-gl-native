#include <iostream>
#include <node.h>
#include <v8.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils/utils.h"
#include "gl/glfw.h"
#include "gl/gl.h"

using namespace v8;
using namespace std;

bool     initialized = false;
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

NAN_METHOD(Clear){
    NanScope();
//    glClear((GLbitfield)args[0]->Int32Value());
    glClear(GL_COLOR_BUFFER_BIT);
    NanReturnUndefined();
}

NAN_METHOD(ClearColor){
    NanScope();
    glClearColor(
            (GLfloat)args[0]->NumberValue(),
            (GLfloat)args[1]->NumberValue(),
            (GLfloat)args[2]->NumberValue(),
            (GLfloat)args[3]->NumberValue());
    NanReturnUndefined();
}

NAN_METHOD(GetTime){
    NanScope();
    NanReturnValue(V8_NUM(glfwGetTime()));
}

NAN_METHOD(PollEvents){
    NanScope();
    glfwPollEvents();
    NanReturnUndefined();
}

NAN_METHOD(SwapInterval){
    NanScope();
    glfwSwapInterval(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(SwapBuffers){
    NanScope();
    glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(windowPtr));
    NanReturnUndefined();
}

NAN_METHOD(Terminate){
    NanScope();
    glfwTerminate();
    NanReturnUndefined();
}

NAN_METHOD(Dispose){
    NanScope();
    NanReturnUndefined();
}

/*---------------------------------------------------------------------------------------------------------*/
// INPUT
/*---------------------------------------------------------------------------------------------------------*/

NAN_METHOD(SetCursorPos){
    NanScope();
    glfwSetCursorPos(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            args[0]->NumberValue(),
            args[1]->NumberValue());
    NanReturnUndefined();
}

NAN_METHOD(GetCursorPos){
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

NAN_METHOD(WindowShouldClose){
    NanScope();
    NanReturnValue(V8_BOOL((bool)glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(windowPtr))));
}

NAN_METHOD(GetScreenSize){
    NanScope();
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    Local<Array> out = Array::New(2);
    out->Set(0, V8_INT(mode->width));
    out->Set(1, V8_INT(mode->height));
    NanReturnValue(out);
}

NAN_METHOD(SetWindowSize){
    NanScope();
    glfwSetWindowSize(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            args[0]->Uint32Value(),
            args[1]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(GetWindowSize){
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

NAN_METHOD(SetWindowPos){
    NanScope();
    glfwSetWindowPos(
            reinterpret_cast<GLFWwindow*>(windowPtr),
            args[0]->Uint32Value(),
            args[1]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(GetWindowPos){
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

NAN_METHOD(Init) {
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

/*---------------------------------------------------------------------------------------------------------*/
// EXPORT
/*---------------------------------------------------------------------------------------------------------*/

void init(Handle<Object> exports) {

    Local<Object> obj = Object::New();
    NODE_SET_METHOD(obj,"init",Init);

    exports->Set(V8_STR("glfw"),obj);


    NODE_SET_METHOD(exports, "init", Init);
    NODE_SET_METHOD(exports,"terminate", Terminate);

    NODE_SET_METHOD(exports,"pollEvents", PollEvents);
    NODE_SET_METHOD(exports,"swapBuffers", SwapBuffers);
    NODE_SET_METHOD(exports,"swapInterval", SwapInterval);

    NODE_SET_METHOD(exports,"setCursorPos", SetCursorPos);
    NODE_SET_METHOD(exports,"getCursorPos", GetCursorPos);

    NODE_SET_METHOD(exports,"windowShouldClose", WindowShouldClose);
    NODE_SET_METHOD(exports,"getScreenSize", GetScreenSize);
    NODE_SET_METHOD(exports,"setWindowSize", SetWindowSize);
    NODE_SET_METHOD(exports,"getWindowSize", GetWindowSize);
    NODE_SET_METHOD(exports,"setWindowPos", SetWindowPos);
    NODE_SET_METHOD(exports,"getWindowPos", GetWindowPos);

    NODE_SET_METHOD(exports,"getTime", GetTime);

    ngl::init(exports);



    NODE_SET_METHOD(exports,"clear", Clear);
    NODE_SET_METHOD(exports,"clearColor", ClearColor);

    NODE_SET_METHOD(exports,"dispose", Dispose);
}

NODE_MODULE(foam_gl_native, init)