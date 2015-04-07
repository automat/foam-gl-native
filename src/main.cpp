#include <iostream>
#include <node.h>

#include "utils/utils.h"
#include "gl/glfw.h"
#include "gl/gl.h"

using namespace v8;
using namespace std;

void init(Handle<Object> exports) {
    Local<Object> glfw = Object::New();
    glfw::init(glfw);
    exports->Set(V8_STR("glfw"),glfw);

    Local<Object> gl = Object::New();
    gl::init(gl);
    exports->Set(V8_STR("gl"),gl);
}

NODE_MODULE(foam_gl_native, init)