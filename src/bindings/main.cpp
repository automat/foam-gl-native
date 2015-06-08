#include <iostream>
#include <node.h>

#include "utils/utils.h"
#include "gl/glfw.h"
#include "gl/gl.h"
#include "gl/glDraw.h"

using namespace v8;
using namespace std;

void init(Handle<Object> exports) {
    Local<Object> glfw = NanNew<Object>();
    glfw::init(glfw);
    exports->Set(NanNew("glfw"),glfw);

    Local<Object> gl = NanNew<Object>();
    gl::init(gl);
    exports->Set(NanNew("gl"),gl);

    Local<Object> glDraw = NanNew<Object>();
    glDraw::init(glDraw);
    exports->Set(NanNew("glDraw"),glDraw);
}

NODE_MODULE(foam_gl_native, init)