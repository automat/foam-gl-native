#ifndef _FOAM_GL_NATIVE_GLFW_H_
#define _FOAM_GL_NATIVE_GLFW_H_

#include "v8.h"

using namespace v8;

namespace glfw{
    void init(Handle<Object> exports);
};

//namespace glfw{
//    void init(Handle<Object> target);
//}

#endif //_FOAM_GL_NATIVE_GLFW_H_
