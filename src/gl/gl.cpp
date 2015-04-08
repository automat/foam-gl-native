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
// SHADERS AND PROGRAMS
/*--------------------------------------------------------------------------------------------*/

//region SHADER OBJECTS

NAN_METHOD(createShader) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint shader = glCreateShader(args[0]->Uint32Value());
    shaders.push_back(shader);
    NanReturnValue(V8_NUM(shader));
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

NAN_METHOD(compileShader) {
    NanScope();
    CHECK_ARGS_LEN(1);
    glCompileShader(args[0]->Uint32Value());
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
//endregion

//region PROGRAM OBJECTS

NAN_METHOD(createProgram) {
    NanScope();
    GLuint program = glCreateProgram();
    programs.push_back(program);
    NanReturnValue(V8_NUM(program));
}

NAN_METHOD(attachShader){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    GLuint shader = args[1]->Uint32Value();
    glAttachShader(program,shader);
    NanReturnUndefined();
}

NAN_METHOD(detachShader) {
    NanScope();
    CHECK_ARGS_LEN(2);
    glDetachShader(args[0]->Uint32Value(), args[1]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(linkProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    glLinkProgram(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(useProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    glUseProgram(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(deleteProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint program = args[0]->Uint32Value();
    glDeleteProgram(program);
    removeRef(&programs, program);
    NanReturnUndefined();
}
//endregion

//region PROGRAM PIPELINE OBJECTS

NAN_METHOD(createProgramPipeline){
    NanScope();
    GLuint pipeline;
    glGenProgramPipelines(1,&pipeline);
    NanReturnValue(V8_INT(pipeline));
}

NAN_METHOD(deleteProgramPipeline){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint pipeline = args[0]->Uint32Value();
    glDeleteProgramPipelines(1,&pipeline);
    NanReturnUndefined();
}

NAN_METHOD(bindProgramPipeline){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint pipeline = args[0]->Uint32Value();
    glBindProgramPipeline(pipeline);
    NanReturnUndefined();
}

NAN_METHOD(useProgramStages){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLuint pipeline = args[0]->Uint32Value();
    GLbitfield stages = args[1]->Uint32Value();
    GLuint  program = args[2]->Uint32Value();
    glUseProgramStages(pipeline,stages,program);
    NanReturnUndefined();
}

NAN_METHOD(activeShaderProgram){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint pipeline = args[0]->Uint32Value();
    GLuint program = args[1]->Uint32Value();
    glActiveShaderProgram(pipeline,program);
    NanReturnUndefined();
}

//endregion

//region VERTEX ATTRIBUTES

NAN_METHOD(getAttribLocation){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value name(args[1]);
    NanReturnValue(V8_INT(glGetAttribLocation(program,*name)));
}

//getActiveAttrib

NAN_METHOD(bindAttribLocation) {
    NanScope();
    CHECK_ARGS_LEN(3);
    glBindAttribLocation(
            args[0]->Uint32Value(),
            args[1]->Uint32Value(),
            *String::Utf8Value(args[2]));
    NanReturnUndefined();
}

//endregion

//region UNIFORM VARIABLES

const GLfloat* getUniformFloat32ArrayData(Local<Object> value, int lengthExp){
    if(value->GetIndexedPropertiesExternalArrayDataType() != kExternalFloatArray){
        NanThrowTypeError("Array arg not of type Float32Array.");
    } else if(value->GetIndexedPropertiesExternalArrayDataLength() != lengthExp){
        stringstream ss;
        ss << "Float32Array length must be " << lengthExp << ".";
        NanThrowError(ss.str().c_str());
    }
    return static_cast<const GLfloat*>(value->GetIndexedPropertiesExternalArrayData());
}

NAN_METHOD(getUniformLocation){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value name(args[1]);
    NanReturnValue(V8_INT(glGetUniformLocation(program,*name)));
}

//getUniformBlockIndex
//getActiveUniformBlockName
//getActiveUniformBlockiv
//getUniformIndices
//getActiveUniformName
//getActiveUniform
//getActiveUniformsiv

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

//uniform1fv
//uniform2fv
//uniform3fv
//uniform4fv

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

//glProgramUniform{1234}{ifd}
//glProgramUniform{1234}{ifd}v
//glProgramUniform{1234}ui
//glProgramUniform{1234}uiv
//glProgramUniformMatrix{234}{fd}v
//glProgramUniformMatrixf{2x3,3x2,2x4, 4x2,3x4,4x3}{fd}v
//endregion

//region UNIFORM BUFFER OBJECTS BINDINGS
//uniformBlockBinding
//endregion

//region SUBROUTINE UNIFORM VARIABLES
//getSubroutineUniformLocation
//getSubroutineIndex
//getActiveSubroutineUniformiv
//getActiveSubroutineUniformName
//getActiveSubroutineName
//uniformSubroutinesuiv
//endregion

//region VARYING VARIABLES
//transformFeedbackVaryings
//getTransformFeedbackVarying
//endregion

//region SHADER EXECUTION
NAN_METHOD(validateProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint program = args[0]->Uint32Value();
    glValidateProgram(program);
    NanReturnUndefined();
}

NAN_METHOD(validateProgramPipeline){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint program = args[0]->Uint32Value();
    glValidateProgramPipeline(program);
    NanReturnUndefined();
}
//endregion

//region TESSELATION CONTROL SHADERS
//patchParamerterfv
//endregion

//region FRAGMENT SHADERS
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
//endregion

/*--------------------------------------------------------------------------------------------*/
// SHADER AND PROGRAM QUERIES
/*--------------------------------------------------------------------------------------------*/

//region SHADER QUERIES
NAN_METHOD(isShader) {
    NanScope();
    NanReturnValue(V8_BOOL(glIsShader(args[0]->Uint32Value())));
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

//getShaderPrecisionFormat
//getProgramStageiv
//endregion

//region PROGRAM QUERIES
//getAttachedShaders
//getVertexAttrib{d f i}v
//getVertexAttribl{i ui}v
//getVertexAttribLdv
//getVertexAttribPointerv
//getUniform{f d i ui}v
//getUniformSubroutineuiv

NAN_METHOD(isProgram) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLboolean value = glIsProgram(args[0]->Uint32Value());
    NanReturnValue(V8_BOOL(value));
}

//getProgramiv

NAN_METHOD(isProgramPipeline){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLboolean value = glIsProgramPipeline(args[0]->Uint32Value());
    NanReturnValue(V8_BOOL(value));
}

//GetProgramPipelineiv

NAN_METHOD(getProgramInfoLog) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint program = args[0]->Uint32Value();
    GLsizei len = 1024;
    GLchar infoLog[len];
    glGetProgramInfoLog(program, len, &len, infoLog);
    NanReturnValue(V8_STR(infoLog));
}

NAN_METHOD(getProgramPipelineInfoLog){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint pipeline = args[0]->Uint32Value();
    GLsizei len = 1024;
    GLchar infoLog[len];
    glGetProgramPipelineInfoLog(pipeline,len,&len,infoLog);
    NanReturnValue(V8_STR(infoLog));
}

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

//getProgramPipelineParameter

//endregion

/*--------------------------------------------------------------------------------------------*/
// RASTERIZATION
/*--------------------------------------------------------------------------------------------*/

//region MULTISAMPLING
NAN_METHOD(getMultisamplefv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum pname = args[0]->Uint32Value();
    GLuint index = args[1]->Uint32Value();
    GLfloat val[2] = {0.0f, 0.0f};
    glGetMultisamplefv(pname,index,val);
    Local<Array> out = Array::New(2);
    out->Set(0,V8_NUM(val[0]));
    out->Set(1,V8_NUM(val[1]));
    NanReturnValue(out);
}

NAN_METHOD(minSampleShading){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLclampf value = static_cast<GLclampf>(args[0]->NumberValue());
    glMinSampleShading(value);
    NanReturnUndefined();
}
//endregion

//region POINTS
NAN_METHOD(pointSize){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLfloat size = static_cast<GLfloat>(args[0]->NumberValue());
    glPointSize(size);
    NanReturnUndefined();
}

NAN_METHOD(pointParameteri){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum pname = args[0]->Uint32Value();
    GLint  param = args[1]->Int32Value();
    glPointParameteri(pname,param);
    NanReturnUndefined();
}

NAN_METHOD(pointParameterf){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum  pname = args[0]->Uint32Value();
    GLfloat param = static_cast<GLfloat>(args[1]->NumberValue());
    glPointParameterf(pname,param);
    NanReturnUndefined();
}

//pointParameteriv
//pointParameterfv

//endregion

//region LINE SEGMENTS
NAN_METHOD(lineWidth){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLfloat value = static_cast<GLfloat>(args[0]->NumberValue());
    glLineWidth(value);
    NanReturnUndefined();
}

NAN_METHOD(lineStipple){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLint factor = args[0]->Int32Value();
    GLushort pattern = static_cast<GLushort>(args[1]->Uint32Value());
    glLineStipple(factor,pattern);
    NanReturnUndefined();
}
//endregion

//region POLYGONS
NAN_METHOD(frontFace){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum dir = args[0]->Uint32Value();
    glFrontFace(dir);
    NanReturnUndefined();
}

NAN_METHOD(cullFace){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum mode = args[0]->Uint32Value();
    glFrontFace(mode);
    NanReturnUndefined();
}
//endregion

//region STIPPLING
//polygonStipple
//getPolygonStipple
//endregion

//region POLYGON RASTERIZATION & DEPTH OFFSET
//polygonMode
NAN_METHOD(polygonOffset){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLfloat factor = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat units = static_cast<GLfloat>(args[1]->NumberValue());
    glPolygonOffset(factor,units);
    NanReturnUndefined();
}
//endregion

//region PIXEL STORAGE MODES
NAN_METHOD(pixelStorei){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum pname = args[0]->Uint32Value();
    GLint  param = args[1]->Int32Value();
    glPixelStorei(pname,param);
    NanReturnUndefined();
}

NAN_METHOD(pixelStoref){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum pname = args[0]->Uint32Value();
    GLfloat param = static_cast<GLfloat>(args[1]->Int32Value());
    glPixelStoref(pname,param);
    NanReturnUndefined();
}
//endregion

//region PIXEL TRANSFER MODES
//pixelTransfer{if}
//pixelMap{ui us f}v
//getPixelMap{ui us f}v
//endregion

//region COLOR TABLE SPECIFIATION
//colorTable
//colorTableParameter{if}v
//copyColorTable
//colorSubTable
//copyColorSubTable
//getColorTable
//getColorTableParameter{if}v
//convolutionFilter2D
//convolutionFilter1D
//convolutionParameter{if}v
//seperateFilter2D
//copyConvolutionFilter2D
//copyConvolutionFilter1D
//getConvolutionFilter
//getSeparableFilter
//getConvolutionParameter{if}v
//histogram
//getHistogram
//resetHistogram
//getHistogramParameter{if}v
//minmax
//getMinmax
//resetMinmax
//getMinmaxParameter{if}v
//endregion

//region RASTERIZATION OF PIXEL RECTANGLES
//drawPixels
//clampColor
//endregion

//region PIXEL TRANSFER OPERATIONS
//convolutionParameter
//endregion

//region BITMAPS
//Bitmap
//endregion


/*--------------------------------------------------------------------------------------------*/
// WHOLE FRAMEBUFFER
/*--------------------------------------------------------------------------------------------*/

//region SELECTING A BUFFER FOR WRITING
NAN_METHOD(drawBuffer){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum buf = args[0]->Uint32Value();
    glDrawBuffer(buf);
    NanReturnUndefined();
}

//drawBuffers
//endregion

//region FINE CONTROL OF BUFFER UPDATES
NAN_METHOD(indexMask){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint mask = args[0]->Uint32Value();
    glIndexMask(mask);
    NanReturnUndefined();
}

NAN_METHOD(colorMask){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLboolean r = static_cast<GLboolean>(args[0]->BooleanValue());
    GLboolean g = static_cast<GLboolean>(args[1]->BooleanValue());
    GLboolean b = static_cast<GLboolean>(args[2]->BooleanValue());
    GLboolean a = static_cast<GLboolean>(args[3]->BooleanValue());
    glColorMask(r,g,b,a);
    NanReturnUndefined();
}

NAN_METHOD(colorMaski){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLuint buf = args[0]->Uint32Value();
    GLboolean r = static_cast<GLboolean>(args[0]->BooleanValue());
    GLboolean g = static_cast<GLboolean>(args[1]->BooleanValue());
    GLboolean b = static_cast<GLboolean>(args[2]->BooleanValue());
    GLboolean a = static_cast<GLboolean>(args[3]->BooleanValue());
    glColorMaski(buf,r,g,b,a);
    NanReturnUndefined();
}

NAN_METHOD(depthMask){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLboolean mask = static_cast<GLboolean>(args[0]->BooleanValue());
    glDepthMask(mask);
    NanReturnUndefined();
}

NAN_METHOD(stencilMask){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLboolean mask = static_cast<GLboolean>(args[0]->BooleanValue());
    glStencilMask(mask);
    NanReturnUndefined();
}

NAN_METHOD(stencilMaskSeparate){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum face = args[0]->Uint32Value();
    GLuint mask = args[1]->Uint32Value();
    glStencilMaskSeparate(face,mask);
    NanReturnUndefined();
}
//endregion

//region CLEARING THE BUFFERS
NAN_METHOD(clear){
    NanScope();
    CHECK_ARGS_LEN(1);
    glClear(args[0]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(clearColor){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLfloat r = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat g = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat b = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat a = static_cast<GLfloat>(args[3]->NumberValue());
    glClearColor(r,g,b,a);
    NanReturnUndefined();
}

NAN_METHOD(clearIndex){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLfloat index = static_cast<GLfloat>(args[0]->NumberValue());
    glClearIndex(index);
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

NAN_METHOD(clearAccum){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLfloat r = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat g = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat b = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat a = static_cast<GLfloat>(args[3]->NumberValue());
    glClearAccum(r,g,b,a);
    NanReturnUndefined();
}

//clearBuffer{if ui}v

NAN_METHOD(clearBufferfi){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLenum buf = args[0]->Uint32Value();
    GLint drawbuffer = args[1]->Int32Value();
    GLfloat depth = static_cast<GLfloat>(args[2]->NumberValue());
    GLint stencil = args[3]->Int32Value();
    glClearBufferfi(buf,drawbuffer,depth,stencil);
    NanReturnUndefined();
}

//endregion

//region ACCUMULATION BUFFER
NAN_METHOD(accum){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum op = args[0]->Uint32Value();
    GLfloat value = static_cast<GLfloat>(args[1]->NumberValue());
    glAccum(op,value);
    NanReturnUndefined();
}
//endregion

/*--------------------------------------------------------------------------------------------*/
// COLOR SUM, FOG AND HINTS
/*--------------------------------------------------------------------------------------------*/


//    NAN_METHOD(GetAttachedShaders){}

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
// RASTERIZATION
/*--------------------------------------------------------------------------------------------*/




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

NAN_METHOD(copyBufferSubData){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLenum readTarget = args[0]->Uint32Value();
    GLenum writeTarget = args[1]->Uint32Value();
    GLintptr readoffset = args[2]->Uint32Value();
    GLintptr writeoffset = args[3]->Uint32Value();
    GLsizeiptr size = args[4]->Uint32Value();
    glCopyBufferSubData(readTarget,writeTarget,readoffset,writeoffset,size);
    NanReturnUndefined();
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
    // WHOLE FRAMEBUFFER
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(NONE);
    EXPORT_SET_GL_CONST(FRONT_LEFT);
    EXPORT_SET_GL_CONST(FRONT_RIGHT);
    EXPORT_SET_GL_CONST(LEFT);
    EXPORT_SET_GL_CONST(RIGHT);
    EXPORT_SET_GL_CONST(BACK_LEFT);
    EXPORT_SET_GL_CONST(BACK_RIGHT);
    EXPORT_SET_GL_CONST(FRONT);
    EXPORT_SET_GL_CONST(BACK);
    EXPORT_SET_GL_CONST(FRONT_AND_BACK);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT0);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT1);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT2);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT3);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT4);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT5);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT6);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT7);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT8);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT9);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT10);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT11);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT12);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT13);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT14);
    EXPORT_SET_GL_CONST(COLOR_ATTACHMENT15);
    EXPORT_SET_GL_CONST(AUX0);
    EXPORT_SET_GL_CONST(AUX1);
    EXPORT_SET_GL_CONST(AUX2);
    EXPORT_SET_GL_CONST(AUX3);

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

    //region SHADER OBJECTS
    EXPORT_SET_METHOD(createShader);
    EXPORT_SET_METHOD(shaderSource);
    EXPORT_SET_METHOD(compileShader);
    EXPORT_SET_METHOD(deleteShader);
    //endregion

    //region PROGRAM OBJECTS
    EXPORT_SET_METHOD(createProgram);
    EXPORT_SET_METHOD(attachShader);
    EXPORT_SET_METHOD(detachShader);
    EXPORT_SET_METHOD(linkProgram);
    EXPORT_SET_METHOD(useProgram);
    EXPORT_SET_METHOD(deleteProgram);
    //endregion

    //region PROGRAM PIPELINE OBJECTS
    EXPORT_SET_METHOD(createProgramPipeline);
    EXPORT_SET_METHOD(deleteProgramPipeline);
    EXPORT_SET_METHOD(bindProgramPipeline);
    EXPORT_SET_METHOD(useProgramStages);
    EXPORT_SET_METHOD(activeShaderProgram);
    //endregion

    //region VERTEX ATTRIBUTES
    EXPORT_SET_METHOD(getAttribLocation);
    //getActiveAttrib
    EXPORT_SET_METHOD(bindAttribLocation);
    //endregion

    //region UNIFORM VARIABLES
    EXPORT_SET_METHOD(getUniformLocation);
    //getUniformBlockIndex
    //getActiveUniformBlockName
    //getActiveUniformBlockiv
    //getUniformIndices
    //getActiveUniformName
    //getActiveUniform
    //getActiveUniformsiv
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
    //endregion

    //region UNIFORM BUFFER OBJECTS BINDINGS
    //uniformBlockBinding
    //endregion

    //region SUBROUTINE UNIFORM VARIABLES
    //getSubroutineUniformLocation
    //getSubroutineIndex
    //getActiveSubroutineUniformiv
    //getActiveSubroutineUniformName
    //getActiveSubroutineName
    //uniformSubroutinesuiv
    //endregion

    //region VARYING VARIABLES
    //transformFeedbackVaryings
    //getTransformFeedbackVarying
    //endregion

    //region SHADER EXECUTION
    EXPORT_SET_METHOD(validateProgram);
    EXPORT_SET_METHOD(validateProgramPipeline);
    //endregion

    //region FRAGMENT SHADERS
    EXPORT_SET_METHOD(bindFragDataLocation);
    EXPORT_SET_METHOD(bindFragDataLocationIndexed);
    EXPORT_SET_METHOD(getFragDataLocation);
    EXPORT_SET_METHOD(getFrageDataIndex);
    //endregion

    /*----------------------------------------------------------------------------------------*/
    // SHADER AND PROGRAM QUERIES
    /*----------------------------------------------------------------------------------------*/

    //region SHADER QUERIES
    EXPORT_SET_METHOD(isShader);
    EXPORT_SET_METHOD(getShaderParameter);
    EXPORT_SET_METHOD(getShaderInfoLog);
    EXPORT_SET_METHOD(getShaderSource);
    //getShaderPrecisionFormat
    //getProgramStageiv
    //endregion

    //region PROGRAM QUERIES
    //getAttachedShaders
    //getVertexAttrib{d f i}v
    //getVertexAttribl{i ui}v
    //getVertexAttribLdv
    //getVertexAttribPointerv
    //getUniform{f d i ui}v
    //getUniformSubroutineuiv
    //getProgramiv
    EXPORT_SET_METHOD(isProgramPipeline);
    EXPORT_SET_METHOD(isProgram);
    //GetProgramPipelineiv
    EXPORT_SET_METHOD(getProgramInfoLog);
    EXPORT_SET_METHOD(getProgramPipelineInfoLog);
    EXPORT_SET_METHOD(getProgramParameter);
    //getProgramPipelineParameter
    //endregion

    /*----------------------------------------------------------------------------------------*/
    // RASTERIZATION
    /*----------------------------------------------------------------------------------------*/

    //region MULTISAMPLING
    EXPORT_SET_METHOD(getMultisamplefv);
    EXPORT_SET_METHOD(minSampleShading);
    //endregion

    //region POINTS
    EXPORT_SET_METHOD(pointSize);
    EXPORT_SET_METHOD(pointParameteri);
    EXPORT_SET_METHOD(pointParameterf);
    //pointParameteriv
    //pointParameterfv
    //endregion

    //region LINE SEGMENTS
    EXPORT_SET_METHOD(lineWidth);
    EXPORT_SET_METHOD(lineStipple);
    //endregion

    //region POLYGONS
    EXPORT_SET_METHOD(frontFace);
    EXPORT_SET_METHOD(cullFace);
    //endregion

    //region STIPPLING
    //polygonStipple
    //getPolygonStipple
    //endregion

    //region POLYGON RASTERIZATION & DEPTH OFFSET
    //polygonMode
    EXPORT_SET_METHOD(polygonOffset);
    //endregion

    //region PIXEL STORAGE MODES
    EXPORT_SET_METHOD(pixelStorei);
    EXPORT_SET_METHOD(pixelStoref);
    //endregion

    //region PIXEL TRANSFER MODES
    //pixelTransfer{if}
    //pixelMap{ui us f}v
    //getPixelMap{ui us f}v
    //endregion

    //region COLOR TABLE SPECIFIATION
    //colorTable
    //colorTableParameter{if}v
    //copyColorTable
    //colorSubTable
    //copyColorSubTable
    //getColorTable
    //getColorTableParameter{if}v
    //convolutionFilter2D
    //convolutionFilter1D
    //convolutionParameter{if}v
    //seperateFilter2D
    //copyConvolutionFilter2D
    //copyConvolutionFilter1D
    //getConvolutionFilter
    //getSeparableFilter
    //getConvolutionParameter{if}v
    //histogram
    //getHistogram
    //resetHistogram
    //getHistogramParameter{if}v
    //minmax
    //getMinmax
    //resetMinmax
    //getMinmaxParameter{if}v
    //endregion

    //region RASTERIZATION OF PIXEL RECTANGLES
    //drawPixels
    //clampColor
    //endregion

    //region PIXEL TRANSFER OPERATIONS
    //convolutionParameter
    //endregion

    //region BITMAPS
    //Bitmap
    //endregion

    /*----------------------------------------------------------------------------------------*/
    // WHOLE FRAMEBUFFER
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_GL_CONST(DEPTH_BUFFER_BIT);
    EXPORT_SET_GL_CONST(COLOR_BUFFER_BIT);
    EXPORT_SET_GL_CONST(STENCIL_BUFFER_BIT);
    EXPORT_SET_GL_CONST(ACCUM_BUFFER_BIT);

    //region SELECTING A BUFFER FOR WRITING
    EXPORT_SET_METHOD(drawBuffer);
    //drawBuffers
    //endregion

    //region FINE CONTROL OF BUFFER UPDATES
    EXPORT_SET_METHOD(indexMask);
    EXPORT_SET_METHOD(colorMask);
    EXPORT_SET_METHOD(colorMaski);
    EXPORT_SET_METHOD(depthMask);
    EXPORT_SET_METHOD(stencilMask);
    EXPORT_SET_METHOD(stencilMaskSeparate);
    //endregion

    //region CLEARING THE BUFFERS
    EXPORT_SET_METHOD(clear);
    EXPORT_SET_METHOD(clearColor);
    EXPORT_SET_METHOD(clearIndex);
    EXPORT_SET_METHOD(clearDepth);
    EXPORT_SET_METHOD(clearStencil);
    EXPORT_SET_METHOD(clearAccum);
    //clearBuffer{if ui}v
    EXPORT_SET_METHOD(clearBufferfi);
    //endregion

    //region ACCUMULATION BUFFER
    EXPORT_SET_METHOD(accum);
    //endregion

    /*----------------------------------------------------------------------------------------*/
    // COLOR SUM, FOG AND HINTS
    /*----------------------------------------------------------------------------------------*/


    /*----------------------------------------------------------------------------------------*/
    // UNIFORMS AND ATTRIBUTES
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(enableVertexAttribArray);
    EXPORT_SET_METHOD(disableVertexAttribArray);





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
