#include "gl.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <node.h>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <ostream>
#include <iostream>
#include <sstream>

using namespace std;

/*--------------------------------------------------------------------------------------------*/
// REFS & UTILITIES
/*--------------------------------------------------------------------------------------------*/

vector<GLuint> programs;
vector<GLuint> shaders;
vector<GLuint> buffers;

map<v8::ExternalArrayType, GLuint> ELEMENT_SIZES_FOR_ARRAY_TYPES{
        {v8::kExternalByteArray, 1},
        {v8::kExternalUnsignedByteArray, 1},
        {v8::kExternalShortArray, 2},
        {v8::kExternalUnsignedShortArray, 2},
        {v8::kExternalIntArray, 4},
        {v8::kExternalUnsignedIntArray, 4},
        {v8::kExternalFloatArray, 4}
};

void removeRef(vector<GLuint> *collection, GLuint obj){
    collection->erase(find(begin(*collection),end(*collection),obj));
}

/*--------------------------------------------------------------------------------------------*/
// PER-FRAGMENT OPERATIONS
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(depthFunc){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum func = args[0]->Uint32Value();
    glDepthFunc(func);
    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// SPECIAL FUNCTIONS
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(disable){
    NanScope();
    CHECK_ARGS_LEN(1);
    glDisable(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(enable){
    NanScope();
    CHECK_ARGS_LEN(1);
    glEnable(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(finish){
    NanScope();
    glFinish();
    NanReturnUndefined();
}

NAN_METHOD(flush){
    NanScope();
    glFlush();
    NanReturnUndefined();
}

NAN_METHOD(getParameter){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum pname = args[0]->Uint32Value();
    switch(pname){
        case GL_CURRENT_PROGRAM:
        case GL_ARRAY_BUFFER_BINDING:
            GLint params;
            glGetIntegerv(pname,&params);
            NanReturnValue(V8_INT(params));
        default:
            break;
    }
    NanReturnUndefined();
}

NAN_METHOD(hint){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum target = args[0]->Uint32Value();
    GLenum mode   = args[1]->Uint32Value();
    glHint(target,mode);
    NanReturnUndefined();
}

NAN_METHOD(isEnabled){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLboolean enabled = glIsEnabled(args[0]->Uint32Value());
    NanReturnValue(V8_BOOL(enabled));
}

NAN_METHOD(pixelStorei){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum pname = args[0]->Uint32Value();
    GLint  param = args[0]->Int32Value();
    glPixelStorei(pname,param);
    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// FRAMEBUFFER
/*--------------------------------------------------------------------------------------------*/

//#define THROW_ARGS_LEN_ERROR(methodName,expectedSize)

NAN_METHOD(clear){
    NanScope();
    CHECK_ARGS_LEN(1);
    glClear(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(clearColor){
    NanScope();
    CHECK_ARGS_LEN(4);
    glClearColor(
            static_cast<GLfloat>(args[0]->NumberValue()),
            static_cast<GLfloat>(args[1]->NumberValue()),
            static_cast<GLfloat>(args[2]->NumberValue()),
            static_cast<GLfloat>(args[3]->NumberValue())
    );
    NanReturnUndefined();
}

NAN_METHOD(clearDepth){
    NanScope();
    CHECK_ARGS_LEN(1);
    glClearDepth(static_cast<GLfloat>(args[0]->NumberValue()));
    NanReturnUndefined();
}

NAN_METHOD(clearStencil){
    NanScope();
    CHECK_ARGS_LEN(1);
    glClearStencil(args[0]->Int32Value());
    NanReturnUndefined();
}

NAN_METHOD(colorMask){
    NanScope();
    CHECK_ARGS_LEN(4);
    glColorMask(
            static_cast<GLboolean>(args[0]->BooleanValue()),
            static_cast<GLboolean>(args[1]->BooleanValue()),
            static_cast<GLboolean>(args[2]->BooleanValue()),
            static_cast<GLboolean>(args[3]->BooleanValue())
    );
    NanReturnUndefined();
}

NAN_METHOD(depthMask){
    NanScope();
    CHECK_ARGS_LEN(1);
    glDepthMask(static_cast<GLboolean>(args[0]->BooleanValue()));
    NanReturnUndefined();
}

NAN_METHOD(stencilMask){
    NanScope();
    CHECK_ARGS_LEN(1);
    glStencilMask(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(stencilMaskSeparate){
    NanScope();
    CHECK_ARGS_LEN(2);
    glStencilMaskSeparate(
            args[0]->Uint32Value(),
            args[1]->Uint32Value()
    );
    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// VIEW & CLIP
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(viewport) {
    NanScope();
    CHECK_ARGS_LEN(4);
    glViewport(
            args[0]->Uint32Value(),
            args[1]->Uint32Value(),
            args[2]->Uint32Value(),
            args[3]->Uint32Value()
    );
    NanReturnUndefined();
}

NAN_METHOD(depthRange) {
    NanScope();
    CHECK_ARGS_LEN(2);
    glDepthRange(
            args[0]->NumberValue(),
            args[1]->NumberValue()
    );
    NanReturnUndefined();
}

NAN_METHOD(scissor) {
    NanScope();
    CHECK_ARGS_LEN(4);
    glScissor(
            args[0]->Int32Value(),
            args[1]->Int32Value(),
            args[2]->Int32Value(),
            args[3]->Int32Value()
    );
    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// PROGRAM
/*--------------------------------------------------------------------------------------------*/

//FRAGMENT SHADERS

NAN_METHOD(bindFragDataLocation){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLuint program = args[0]->Uint32Value();
    GLuint colorNumber = args[1]->Uint32Value();
    String::Utf8Value name(args[2]);
    glBindFragDataLocation(program,colorNumber,*name);
    NanReturnUndefined();
}

NAN_METHOD(bindFragDataLocationIndexed){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLuint program = args[0]->Uint32Value();
    GLuint colorNumber = args[1]->Uint32Value();
    GLuint index = args[2]->Uint32Value();
    String::Utf8Value name(args[3]);
    glBindFragDataLocationIndexed(program,colorNumber,index,*name);
    NanReturnUndefined();
}

NAN_METHOD(getFragDataLocation){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value name(args[1]);
    GLint location = glGetFragDataLocation(program,*name);
    NanReturnValue(V8_INT(location));
}

NAN_METHOD(getFrageDataIndex){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value name(args[1]);
    GLint index = glGetFragDataIndex(program,*name);
    NanReturnValue(V8_INT(index));
}


NAN_METHOD(attachShader){
    NanScope();
    CHECK_ARGS_LEN(2);
    glAttachShader(
            args[0]->Uint32Value(),
            args[1]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(bindAttribLocation) {
    NanScope();
    CHECK_ARGS_LEN(3);
    glBindAttribLocation(
            args[0]->Uint32Value(),
            args[1]->Uint32Value(),
            *String::Utf8Value(args[2]));
    NanReturnUndefined();
}

NAN_METHOD(compileShader) {
    NanScope();
    CHECK_ARGS_LEN(1);
    glCompileShader(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(createProgram) {
    NanScope();
    GLuint program = glCreateProgram();
    programs.push_back(program);
    NanReturnValue(V8_NUM(program));
}

NAN_METHOD(createShader) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint shader = glCreateShader(args[0]->Uint32Value());
    shaders.push_back(shader);
    NanReturnValue(V8_NUM(shader));
}

NAN_METHOD(deleteProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint program = args[0]->Uint32Value();
    glDeleteProgram(program);
    removeRef(&programs, program);
    NanReturnUndefined();
}

NAN_METHOD(deleteShader) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint shader = args[0]->Uint32Value();
    glDeleteShader(shader);
    removeRef(&shaders, shader);
    NanReturnUndefined();
}

NAN_METHOD(detachShader) {
    NanScope();
    CHECK_ARGS_LEN(2);
    glDetachShader(args[0]->Uint32Value(), args[1]->Uint32Value());
    NanReturnUndefined();
}

//    NAN_METHOD(GetAttachedShaders){}

NAN_METHOD(getProgramParameter) {
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
    GLint params;

    switch (pname) {
        case GL_COMPILE_STATUS:
        case GL_LINK_STATUS:
            glGetProgramiv(program,pname,&params);
            NanReturnValue(V8_BOOL(static_cast<bool>(params)));
        default:
            break;
    }

    NanReturnUndefined();
}

NAN_METHOD(getProgramInfoLog) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLsizei len = 1024;
    GLchar infoLog[len];
    glGetProgramInfoLog(args[0]->Uint32Value(), len, &len, infoLog);
    NanReturnValue(V8_STR(infoLog));
}

NAN_METHOD(getShaderParameter) {
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint shader, paramName;
    GLint value;
    shader = args[0]->Uint32Value();
    paramName = args[1]->Uint32Value();
    value = 0;
    switch (paramName) {
        case GL_COMPILE_STATUS:
        case GL_DELETE_STATUS:
            glGetShaderiv(shader, paramName, &value);
            NanReturnValue(V8_BOOL(static_cast<bool>(value)));
        default:
            break;
    }

    NanReturnUndefined();
}

NAN_METHOD(getShaderInfoLog) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLsizei len = 1024;
    GLchar infoLog[len];
    glGetShaderInfoLog(args[0]->Uint32Value(), len, &len, infoLog);
    NanReturnValue(V8_STR(infoLog));
}

NAN_METHOD(getShaderSource) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint shader = args[0]->Uint32Value();
    GLint len;
    glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &len);
    GLchar *src = new GLchar[len];
    glGetShaderSource(shader, len, NULL, src);
    Local<String> str = V8_STR(src);
    delete src;
    NanReturnValue(str);
}

NAN_METHOD(isProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    NanReturnValue(V8_BOOL(glIsProgram(args[0]->Uint32Value())));
}

NAN_METHOD(isShader) {
    NanScope();
    NanReturnValue(V8_BOOL(glIsShader(args[0]->Uint32Value())));
}

NAN_METHOD(linkProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    glLinkProgram(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(shaderSource) {
    NanScope();
    CHECK_ARGS_LEN(2);
    String::Utf8Value src(args[1]);
    const char *srcs[1];
    srcs[0] = *src;
    GLint len = src.length();
    glShaderSource(args[0]->Uint32Value(), 1, srcs, &len);
    NanReturnUndefined();
}

NAN_METHOD(useProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    glUseProgram(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(validateProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    glValidateProgram(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(disableVertexAttribArray){
    NanScope();
    CHECK_ARGS_LEN(1);
    glDisableVertexAttribArray(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(enableVertexAttribArray){
    NanScope();
    CHECK_ARGS_LEN(1);
    glEnableVertexAttribArray(args[0]->Uint32Value());
    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// UNIFORMS AND ATTRIBUTES
/*--------------------------------------------------------------------------------------------*/

//glActiveAttrib
//getActiveUniform

NAN_METHOD(getAttribLocation){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value name(args[1]);
    NanReturnValue(V8_INT(glGetAttribLocation(program,*name)));
}

//getUniform

NAN_METHOD(getUniformLocation){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value name(args[1]);
    NanReturnValue(V8_INT(glGetUniformLocation(program,*name)));
}

//getVertexAttrib
//getVertexAttribOffset

const GLfloat* getUniformFloat32ArrayData(Local<Object> value, int lengthExp){
    if(value->GetIndexedPropertiesExternalArrayDataType() != kExternalFloatArray){
        NanThrowTypeError("Array arg not of type Float32Array.");
    } else if(value->GetIndexedPropertiesExternalArrayDataLength() != lengthExp){
        stringstream ss;
        ss << "Float32Array lengt must be " << lengthExp << ".";
        NanThrowError(ss.str().c_str());
    }
    return static_cast<const GLfloat*>(value->GetIndexedPropertiesExternalArrayData());
}

NAN_METHOD(uniform1f) {
    NanScope();
    CHECK_ARGS_LEN(2);
    glUniform1f(
            args[0]->Uint32Value(),
            static_cast<float>(args[1]->NumberValue())
    );
    NanReturnUndefined();
}

NAN_METHOD(uniform2f) {
    NanScope();
    CHECK_ARGS_LEN(3);
    glUniform2f(
            args[0]->Uint32Value(),
            static_cast<float>(args[1]->NumberValue()),
            static_cast<float>(args[2]->NumberValue())
    );
    NanReturnUndefined();
}

NAN_METHOD(uniform3f) {
    NanScope();
    CHECK_ARGS_LEN(4);
    glUniform3f(
            args[0]->Uint32Value(),
            static_cast<float>(args[1]->NumberValue()),
            static_cast<float>(args[2]->NumberValue()),
            static_cast<float>(args[3]->NumberValue())
    );
    NanReturnUndefined();
}

NAN_METHOD(uniform4f) {
    NanScope();
    CHECK_ARGS_LEN(5);
    glUniform4f(
            args[0]->Uint32Value(),
            static_cast<float>(args[1]->NumberValue()),
            static_cast<float>(args[2]->NumberValue()),
            static_cast<float>(args[3]->NumberValue()),
            static_cast<float>(args[4]->NumberValue())
    );
    NanReturnUndefined();
}

//NAN_METHOD(uniform1fv) {
//    NanScope();
//    CHECK_ARGS_LEN(2);
//    GLint location = args[0]->Int32Value();
//
//
//    glUniform1fv()
////    int count = 0;
////    GLfloat *value = getArrayData<GLfloat>(args[1], &count);
////
////    glUniform1fv(
////            args[0]->Uint32Value(),
////            count,
////            value
////    );
//    NanReturnUndefined();
//}
//
//NAN_METHOD(uniform2fv) {
//    NanScope();
//    CHECK_ARGS_LEN(2);
//    int count = 0;
//    GLfloat *value = getArrayData<GLfloat>(args[1], &count);
//
//    glUniform2fv(
//            args[0]->Uint32Value(),
//            count / 2,
//            value
//    );
//    NanReturnUndefined();
//}
//
//NAN_METHOD(uniform3fv) {
//    NanScope();
//    CHECK_ARGS_LEN(2);
//    int count = 0;
//    GLfloat *value = getArrayData<GLfloat>(args[1], &count);
//
//    glUniform3fv(
//            args[0]->Uint32Value(),
//            count / 3,
//            value
//    );
//    NanReturnUndefined();
//}
//
//NAN_METHOD(uniform4fv) {
//    NanScope();
//    CHECK_ARGS_LEN(2);
//    int count = 0;
//    GLfloat *value = getArrayData<GLfloat>(args[1], &count);
//
//    glUniform3fv(
//            args[0]->Uint32Value(),
//            count / 4,
//            value
//    );
//    NanReturnUndefined();
//}

NAN_METHOD(uniform1i) {
    NanScope();
    CHECK_ARGS_LEN(2);
    glUniform1i(
            args[0]->Uint32Value(),
            args[1]->Int32Value()
    );
    NanReturnUndefined();
}

NAN_METHOD(uniform2i) {
    NanScope();
    CHECK_ARGS_LEN(3);
    glUniform2i(
            args[0]->Uint32Value(),
            args[1]->Int32Value(),
            args[2]->Int32Value()
    );
    NanReturnUndefined();
}

NAN_METHOD(uniform3i) {
    NanScope();
    CHECK_ARGS_LEN(4);
    glUniform3i(
            args[0]->Uint32Value(),
            args[1]->Int32Value(),
            args[2]->Int32Value(),
            args[3]->Int32Value()
    );
    NanReturnUndefined();
}

NAN_METHOD(uniform4i) {
    NanScope();
    CHECK_ARGS_LEN(5);
    glUniform4i(
            args[0]->Uint32Value(),
            args[1]->Int32Value(),
            args[2]->Int32Value(),
            args[3]->Int32Value(),
            args[4]->Int32Value()
    );
    NanReturnUndefined();
}

NAN_METHOD(uniformMatrix2fv) {
    NanScope();
    CHECK_ARGS_LEN(3);
    GLint     location  = args[0]->Int32Value();
    GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());

    Local<Object> obj = args[2]->ToObject();
    const GLfloat *value = getUniformFloat32ArrayData(obj, 4);

    glUniformMatrix2fv(location,1,transpose,value);
    NanReturnUndefined();
}

NAN_METHOD(uniformMatrix3fv) {
    NanScope();
    CHECK_ARGS_LEN(3);
    GLint     location  = args[0]->Int32Value();
    GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());

    Local<Object> obj = args[2]->ToObject();
    const GLfloat *value = getUniformFloat32ArrayData(obj, 9);

    glUniformMatrix3fv(location,1,transpose,value);
    NanReturnUndefined();
}

NAN_METHOD(uniformMatrix4fv) {
    NanScope();
    CHECK_ARGS_LEN(3);

    GLint     location  = args[0]->Int32Value();
    GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());

    Local<Object> obj = args[2]->ToObject();
    const GLfloat *value = getUniformFloat32ArrayData(obj, 16);

    glUniformMatrix4fv(location,1,transpose,value);
    NanReturnUndefined();
}

NAN_METHOD(vertexAttribPointer){
    NanScope();
    CHECK_ARGS_LEN(6);
    GLuint index = args[0]->Uint32Value();
    GLint size = args[1]->Int32Value();
    GLenum type = args[2]->Uint32Value();
    GLboolean normalized = static_cast<GLboolean>(args[3]->BooleanValue());
    GLsizei stride = args[4]->Uint32Value();
    intptr_t offset = args[5]->Uint32Value();

    glVertexAttribPointer(index,size,type,normalized,stride, reinterpret_cast<const GLvoid *>(offset));
    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// BUFFER
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(createBuffer){
    NanScope();
    GLuint buffer;
    glGenBuffers(1, &buffer);
    buffers.push_back(buffer);
    NanReturnValue(V8_INT(buffer));
}

NAN_METHOD(bindBuffer) {
    NanScope();
    CHECK_ARGS_LEN(2);
    glBindBuffer(args[0]->Uint32Value(), args[1]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(bufferData) {
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum usage = args[2]->Uint32Value();

    if(args[1]->IsNumber()){
        glBufferData(target,args[1]->Uint32Value(),NULL,usage);
    } else if(args[1]->IsObject()){
        Local<Object> obj = args[1]->ToObject();
        ExternalArrayType type = obj->GetIndexedPropertiesExternalArrayDataType();
        GLsizeiptr size = 0;
        switch (type){
            case kExternalByteArray:
            case kExternalUnsignedByteArray:
//                size = 1;
//                size = obj->GetIndexedPropertiesExternalArrayDataLength() * size;
//                glBufferData(target, size, static_cast<const GLbyte*>(obj->GetIndexedPropertiesExternalArrayData()), usage);
                break;
            case kExternalShortArray:
            case kExternalUnsignedShortArray:
//                size = 2;
//                size = obj->GetIndexedPropertiesExternalArrayDataLength() * size;
//                glBufferData(target, size, static_cast<const GLSh*>(obj->GetIndexedPropertiesExternalArrayData()), usage);
                break;
            case kExternalIntArray:
                size = 4;
                size = obj->GetIndexedPropertiesExternalArrayDataLength() * size;
                glBufferData(target, size, static_cast<const GLint *>(obj->GetIndexedPropertiesExternalArrayData()), usage);
                break;
            case kExternalUnsignedIntArray:
                size = 4;
                size = obj->GetIndexedPropertiesExternalArrayDataLength() * size;
                glBufferData(target, size, static_cast<const GLuint *>(obj->GetIndexedPropertiesExternalArrayData()), usage);
                break;
            case kExternalFloatArray:
                size = 4;
                size = obj->GetIndexedPropertiesExternalArrayDataLength() * size;
                glBufferData(target, size, static_cast<const GLfloat*>(obj->GetIndexedPropertiesExternalArrayData()), usage);
                break;
            default:
                NanThrowError("Array data type not supported.");
                break;
        }
    }
    NanReturnUndefined();
}

NAN_METHOD(bufferSubData) {
    NanScope();
    GLenum target = args[0]->Uint32Value();
    GLintptr offset = args[1]->Uint32Value();
    Local<Object> obj = Local<Object>::Cast(args[2]);

    int elementSize = ELEMENT_SIZES_FOR_ARRAY_TYPES[obj->GetIndexedPropertiesExternalArrayDataType()];
    GLsizeiptr size = obj->GetIndexedPropertiesExternalArrayDataLength() * elementSize;
    void *data = obj->GetIndexedPropertiesExternalArrayData();

    glBufferSubData(target, offset, size, data);
    NanReturnUndefined();
}

NAN_METHOD(deleteBuffer) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint buffer = args[0]->Uint32Value();
    glDeleteBuffers(1, &buffer);
    removeRef(&buffers, buffer);
    NanReturnUndefined();
}

NAN_METHOD(getBufferParameter) {
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum target = args[0]->Uint32Value();
    GLenum pname  = args[1]->Uint32Value();

    switch(pname){
        case GL_BUFFER_SIZE:
        case GL_BUFFER_USAGE:
            GLint params;
            glGetBufferParameteriv(target,pname,&params);
            NanReturnValue(V8_INT(params));
        default:
            break;
    }
    NanReturnUndefined();
}

NAN_METHOD(isBuffer) {
    NanScope();
    CHECK_ARGS_LEN(1);
    NanReturnValue(V8_BOOL(glIsBuffer(args[0]->Uint32Value())));
}

/*--------------------------------------------------------------------------------------------*/
// VERTEX ARRAY OBJECTS
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(createVertexArray){
    NanScope();
    GLuint vao;
    glGenVertexArrays(1,&vao);
    NanReturnValue(V8_INT(vao));
}

NAN_METHOD(deleteVertexArray){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint vao = args[0]->Uint32Value();
    glDeleteVertexArrays(1,&vao);
    NanReturnUndefined();
}

NAN_METHOD(bindVertexArray){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint vao = args[0]->Uint32Value();
    glBindVertexArray(vao);
    NanReturnUndefined();
}

NAN_METHOD(isVertexArray){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLboolean value = glIsVertexArray(args[0]->Uint32Value());
    NanReturnValue(V8_BOOL(value));
}


/*--------------------------------------------------------------------------------------------*/
// DRAW BUFFER WRITING
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(drawArrays){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum  mode  = args[0]->Uint32Value();
    GLint   first = args[1]->Int32Value();
    GLsizei count = args[2]->Uint32Value();
    glDrawArrays(mode,first,count);
    NanReturnUndefined();
}

NAN_METHOD(drawElements){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLenum  mode   = args[0]->Uint32Value();
    GLsizei count  = args[1]->Uint32Value();
    GLenum  type   = args[2]->Uint32Value();
    GLvoid *offset = reinterpret_cast<GLvoid*>(args[3]->Uint32Value());

    glDrawElements(mode,count,type,offset);
    NanReturnUndefined();
}

NAN_METHOD(drawRangeElements){
    NanScope();
    CHECK_ARGS_LEN(6);
    GLenum  mode   = args[0]->Uint32Value();
    GLuint  start  = args[1]->Uint32Value();
    GLuint  end    = args[2]->Uint32Value();
    GLsizei count  = args[3]->Uint32Value();
    GLenum  type   = args[4]->Uint32Value();
    GLvoid *offset = reinterpret_cast<GLvoid*>(args[5]->Uint32Value());

    glDrawRangeElements(mode,start,end,count,type,offset);
    NanReturnUndefined();
}


/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

#define EXPORT_SET_GL_CONST(name) EXPORT_SET_CONST(#name,GL_ ## name)

void gl::init(Handle<Object> exports) {
    EXPORT_SET_GL_CONST(FLOAT);
    EXPORT_SET_GL_CONST(VERTEX_PROGRAM_POINT_SIZE);

    /*----------------------------------------------------------------------------------------*/
    // PER FRAGMENT OPERATIONS
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(NEVER);
    EXPORT_SET_GL_CONST(ALWAYS);
    EXPORT_SET_GL_CONST(LESS);
    EXPORT_SET_GL_CONST(EQUAL);
    EXPORT_SET_GL_CONST(LEQUAL);
    EXPORT_SET_GL_CONST(GREATER);
    EXPORT_SET_GL_CONST(GEQUAL);
    EXPORT_SET_GL_CONST(NOTEQUAL);

    EXPORT_SET_METHOD(depthFunc);

    /*----------------------------------------------------------------------------------------*/
    // SPECIAL FUNCTIONS
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(ARRAY_BUFFER_BINDING);
    EXPORT_SET_GL_CONST(CURRENT_PROGRAM);

    EXPORT_SET_METHOD(disable);
    EXPORT_SET_METHOD(enable);
    EXPORT_SET_METHOD(finish);
    EXPORT_SET_METHOD(flush);
    EXPORT_SET_METHOD(getParameter);
    EXPORT_SET_METHOD(hint);
    EXPORT_SET_METHOD(isEnabled);
    EXPORT_SET_METHOD(pixelStorei);

    /*----------------------------------------------------------------------------------------*/
    // FRAMEBUFFER
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(DEPTH_BUFFER_BIT);
    EXPORT_SET_GL_CONST(COLOR_BUFFER_BIT);

    EXPORT_SET_METHOD(clear);
    EXPORT_SET_METHOD(clearColor);
    EXPORT_SET_METHOD(clearDepth);
    EXPORT_SET_METHOD(clearStencil);
    EXPORT_SET_METHOD(colorMask);
    EXPORT_SET_METHOD(depthMask);
    EXPORT_SET_METHOD(stencilMask);
    EXPORT_SET_METHOD(stencilMaskSeparate);

    /*----------------------------------------------------------------------------------------*/
    // VIEW & CLIP
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(DEPTH_TEST);

    EXPORT_SET_METHOD(viewport);
    EXPORT_SET_METHOD(depthRange);
    EXPORT_SET_METHOD(scissor);

    /*----------------------------------------------------------------------------------------*/
    // PROGRAMS AND SHADERS
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(VERTEX_SHADER);
    EXPORT_SET_GL_CONST(FRAGMENT_SHADER);
    EXPORT_SET_GL_CONST(COMPILE_STATUS);
    EXPORT_SET_GL_CONST(LINK_STATUS);

    //FRAGMENT SHADERS

    EXPORT_SET_METHOD(bindFragDataLocation);
    EXPORT_SET_METHOD(bindFragDataLocationIndexed);
    EXPORT_SET_METHOD(getFragDataLocation);
    EXPORT_SET_METHOD(getFrageDataIndex);

    EXPORT_SET_METHOD(createShader);
    EXPORT_SET_METHOD(attachShader);
    EXPORT_SET_METHOD(bindAttribLocation);
    EXPORT_SET_METHOD(compileShader);
    EXPORT_SET_METHOD(createProgram);
    EXPORT_SET_METHOD(createShader);
    EXPORT_SET_METHOD(deleteProgram);
    EXPORT_SET_METHOD(deleteShader);
    EXPORT_SET_METHOD(detachShader);
    EXPORT_SET_METHOD(getProgramParameter);
    EXPORT_SET_METHOD(getProgramInfoLog);
    EXPORT_SET_METHOD(getShaderParameter);
    EXPORT_SET_METHOD(getShaderInfoLog);
    EXPORT_SET_METHOD(getShaderSource);
    EXPORT_SET_METHOD(isProgram);
    EXPORT_SET_METHOD(isShader);
    EXPORT_SET_METHOD(linkProgram);
    EXPORT_SET_METHOD(shaderSource);
    EXPORT_SET_METHOD(useProgram);
    EXPORT_SET_METHOD(validateProgram);

    /*----------------------------------------------------------------------------------------*/
    // UNIFORMS AND ATTRIBUTES
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(enableVertexAttribArray);
    EXPORT_SET_METHOD(disableVertexAttribArray);
    EXPORT_SET_METHOD(getUniformLocation);
    EXPORT_SET_METHOD(getAttribLocation);

    EXPORT_SET_METHOD(uniform1f);
    EXPORT_SET_METHOD(uniform2f);
    EXPORT_SET_METHOD(uniform3f);
    EXPORT_SET_METHOD(uniform4f);
    EXPORT_SET_METHOD(uniform1i);
    EXPORT_SET_METHOD(uniform2i);
    EXPORT_SET_METHOD(uniform3i);
    EXPORT_SET_METHOD(uniform4i);
    EXPORT_SET_METHOD(uniformMatrix2fv);
    EXPORT_SET_METHOD(uniformMatrix3fv);
    EXPORT_SET_METHOD(uniformMatrix4fv);

    EXPORT_SET_METHOD(vertexAttribPointer);

    /*----------------------------------------------------------------------------------------*/
    // BUFFER
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(ARRAY_BUFFER);
    EXPORT_SET_GL_CONST(ELEMENT_ARRAY_BUFFER);
    EXPORT_SET_GL_CONST(STATIC_DRAW);
    EXPORT_SET_GL_CONST(DYNAMIC_DRAW);

    EXPORT_SET_GL_CONST(BUFFER_SIZE);
    EXPORT_SET_GL_CONST(BUFFER_USAGE);

    EXPORT_SET_METHOD(createBuffer);
    EXPORT_SET_METHOD(bindBuffer);
    EXPORT_SET_METHOD(bufferData);
    EXPORT_SET_METHOD(bufferSubData);
    EXPORT_SET_METHOD(deleteBuffer);
    EXPORT_SET_METHOD(getBufferParameter);
    EXPORT_SET_METHOD(isBuffer);

    /*----------------------------------------------------------------------------------------*/
    // VERTEX ARRAY OBJECTS
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(createVertexArray);
    EXPORT_SET_METHOD(deleteVertexArray);
    EXPORT_SET_METHOD(bindVertexArray);
    EXPORT_SET_METHOD(isVertexArray);

    /*----------------------------------------------------------------------------------------*/
    // DRAW BUFFER WRITING
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(POINTS);
    EXPORT_SET_GL_CONST(LINE_STRIP);
    EXPORT_SET_GL_CONST(LINE_LOOP);
    EXPORT_SET_GL_CONST(LINES);
    EXPORT_SET_GL_CONST(TRIANGLES);
    EXPORT_SET_GL_CONST(TRIANGLE_STRIP);
    EXPORT_SET_GL_CONST(TRIANGLE_FAN);

    EXPORT_SET_METHOD(drawArrays);
    EXPORT_SET_METHOD(drawElements);
    EXPORT_SET_METHOD(drawRangeElements);
}
