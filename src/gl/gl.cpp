#include "gl.h"
#include <GLFW/glfw3.h>
#include <node.h>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <ostream>
#include <iostream>

using namespace std;

void removeGLObject(vector<GLuint> *collection,GLuint object){
//    collection->erase(find(begin(*collection),end(*collection),object);
}

#define GL_CONST(name) exports->Set(V8_STR(#name),V8_INT(name))

#define GL_CONSTANT_EX(name,glName) exports->Set(V8_STR(name),V8_INT(glName))

//https://github.com/mikeseven/node-webgl/blob/master/src/webgl.cc#L72
template<typename Type>
inline Type* getArrayData(Local<Value> arg, int* num = NULL) {
    Type *data=NULL;
    if(num){
        *num=0;
    }

    if(!arg->IsNull()) {
        if(arg->IsArray()) {
            Local<Array> arr = Local<Array>::Cast(arg);
            if(num){
                *num=arr->Length();
            }
            data = reinterpret_cast<Type*>(arr->GetIndexedPropertiesExternalArrayData());
        }
        else if(arg->IsObject()) {
            if(num){
                *num = arg->ToObject()->GetIndexedPropertiesExternalArrayDataLength();
            }
            data = reinterpret_cast<Type*>(arg->ToObject()->GetIndexedPropertiesExternalArrayData());
        }
        else
            ThrowException(V8_STR("Bad array argument"));
    }

    return data;
}

namespace ngl{
    vector<GLuint> programs;
    vector<GLuint> shaders;
    vector<GLuint> buffers;

    map<v8::ExternalArrayType,GLuint> ELEMENT_SIZES_FOR_ARRAY_TYPES {
            {v8::kExternalByteArray,1},
            {v8::kExternalUnsignedByteArray,1},
            {v8::kExternalShortArray,2},
            {v8::kExternalUnsignedShortArray,2},
            {v8::kExternalIntArray,4},
            {v8::kExternalUnsignedIntArray,4},
            {v8::kExternalFloatArray,4}
    };

    /*--------------------------------------------------------------------------------------------*/
    //region VIEW & CLIP
    /*--------------------------------------------------------------------------------------------*/

    NAN_METHOD(Viewport){
        NanScope();
        glViewport(
                args[0]->Int32Value(),
                args[1]->Int32Value(),
                args[2]->Int32Value(),
                args[3]->Int32Value()
        );
        NanReturnUndefined();
    }

    NAN_METHOD(DepthRange){
        NanScope();
        glDepthRange(
                args[0]->NumberValue(),
                args[1]->NumberValue()
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Scissor){
        NanScope();
        glScissor(
                args[0]->Int32Value(),
                args[1]->Int32Value(),
                args[2]->Int32Value(),
                args[3]->Int32Value()
        );
        NanReturnUndefined();
    }

    //endregion

    /*--------------------------------------------------------------------------------------------*/
    //region PROGRAM
    /*--------------------------------------------------------------------------------------------*/

    NAN_METHOD(AttachShader){
        NanScope();
        glAttachShader(
                args[0]->Uint32Value(),
                args[1]->Uint32Value());
        NanReturnUndefined();
    }

    NAN_METHOD(BindAttribLocation){
        NanScope();
        glBindAttribLocation(
                args[0]->Uint32Value(),
                args[1]->Uint32Value(),
                *String::Utf8Value(args[2]));
        NanReturnUndefined();
    }

    NAN_METHOD(CompileShader){
        NanScope();
        glCompileShader(args[0]->Uint32Value());
        NanReturnUndefined();
    }

    NAN_METHOD(CreateProgram){
        NanScope();
        GLuint program = glCreateProgram();
        programs.push_back(program);
        NanReturnValue(V8_NUM(program));
    }

    NAN_METHOD(CreateShader){
        NanScope();
        GLuint shader = glCreateShader(args[0]->Uint32Value());
        shaders.push_back(shader);
        NanReturnValue(V8_NUM(shader));
    }

    NAN_METHOD(DeleteProgram){
        NanScope();
        GLuint program = args[0]->Uint32Value();
        glDeleteProgram(program);
        removeGLObject(&programs, program);
        NanReturnUndefined();
    }

    NAN_METHOD(DeleteShader){
        NanScope();
        GLuint shader = args[0]->Uint32Value();
        glDeleteShader(shader);
        removeGLObject(&shaders, shader);
        NanReturnUndefined();
    }

    NAN_METHOD(DetachShader){
        NanScope();
        glDetachShader(args[0]->Uint32Value(), args[1]->Uint32Value());
        NanReturnUndefined();
    }

//    NAN_METHOD(GetAttachedShaders){}

    NAN_METHOD(GetProgramParameter){
        NanScope();
        GLuint program, paramName, value;
        program   = args[0]->Uint32Value();
        paramName = args[1]->Uint32Value();
//
//        value = 0;
//        switch (paramName){
//        }

        switch (paramName){
            case GL_COMPILE_STATUS:
//                glGetProgramiv(program, paramName, )
                break;
            default:
                break;
        }

        NanReturnUndefined();
    }

    NAN_METHOD(GetProgramInfoLog){
        NanScope();
        GLsizei len = 1024;
        GLchar infoLog[len];
        glGetProgramInfoLog(args[0]->Uint32Value(), len, &len, infoLog);
        NanReturnValue(V8_STR(infoLog));
    }

    NAN_METHOD(GetShaderParameter){
        NanScope();
        GLuint shader, paramName;
        GLint value;
        shader    = args[0]->Uint32Value();
        paramName = args[1]->Uint32Value();
        value     = 0;
        switch (paramName){
            case GL_COMPILE_STATUS:
            case GL_DELETE_STATUS:
                glGetShaderiv(shader, paramName, &value);
                NanReturnValue(V8_BOOL(!value));
            default:
                break;
        }

        NanReturnUndefined();
    }

    NAN_METHOD(GetShaderInfoLog){
        NanScope();
        GLsizei len = 1024;
        GLchar infoLog[len];
        glGetShaderInfoLog(args[0]->Uint32Value(), len, &len, infoLog);
        NanReturnValue(V8_STR(infoLog));
    }

    NAN_METHOD(GetShaderSource){
        NanScope();
        GLuint shader = args[0]->Uint32Value();
        GLint len;
        glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &len);
        GLchar *src = new GLchar[len];
        glGetShaderSource(shader, len, NULL, src);
        Local<String> str = V8_STR(src);
        delete src;
        NanReturnValue(str);
    }

    NAN_METHOD(IsProgram){
        NanScope();
        NanReturnValue(V8_BOOL(glIsProgram(args[0]->Uint32Value())));
    }

    NAN_METHOD(IsShader){
        NanScope();
        NanReturnValue(V8_BOOL(glIsShader(args[0]->Uint32Value())));
    }

    NAN_METHOD(LinkProgram){
        NanScope();
        glLinkProgram(args[0]->Uint32Value());
        NanReturnUndefined();
    }

    NAN_METHOD(ShaderSource){
        NanScope();
        String::Utf8Value src(args[1]);
        const char* srcs[1];
        srcs[0] = *src;
        GLint len = src.length();
        glShaderSource(args[0]->Uint32Value(), 1, srcs, &len);
        NanReturnUndefined();
    }

    NAN_METHOD(UseProgram){
        NanScope();
        glUseProgram(args[0]->Uint32Value());
        NanReturnUndefined();
    }

    NAN_METHOD(ValidateShader){
        NanScope();
        glValidateProgram(args[0]->Uint32Value());
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform1f){
        NanScope();
        glUniform1f(
                args[0]->Uint32Value(),
                static_cast<float>(args[1]->NumberValue())
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform2f){
        NanScope();
        glUniform2f(
                args[0]->Uint32Value(),
                static_cast<float>(args[1]->NumberValue()),
                static_cast<float>(args[2]->NumberValue())
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform3f){
        NanScope();
        glUniform3f(
                args[0]->Uint32Value(),
                static_cast<float>(args[1]->NumberValue()),
                static_cast<float>(args[2]->NumberValue()),
                static_cast<float>(args[3]->NumberValue())
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform4f){
        NanScope();
        glUniform4f(
                args[0]->Uint32Value(),
                static_cast<float>(args[1]->NumberValue()),
                static_cast<float>(args[2]->NumberValue()),
                static_cast<float>(args[3]->NumberValue()),
                static_cast<float>(args[4]->NumberValue())
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform1fv){
        NanScope();
        int count = 0;
        GLfloat *value = getArrayData<GLfloat>(args[1],&count);

        glUniform1fv(
                args[0]->Uint32Value(),
                count,
                value
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform2fv){
        NanScope();
        int count = 0;
        GLfloat *value = getArrayData<GLfloat>(args[1],&count);

        glUniform2fv(
                args[0]->Uint32Value(),
                count / 2,
                value
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform3fv){
        NanScope();
        int count = 0;
        GLfloat *value = getArrayData<GLfloat>(args[1],&count);

        glUniform3fv(
                args[0]->Uint32Value(),
                count / 3,
                value
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform4fv){
        NanScope();
        int count = 0;
        GLfloat *value = getArrayData<GLfloat>(args[1],&count);

        glUniform3fv(
                args[0]->Uint32Value(),
                count / 4,
                value
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform1i){
        NanScope();
        glUniform1i(
                args[0]->Uint32Value(),
                args[1]->Int32Value()
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform2i){
        NanScope();
        glUniform2i(
                args[0]->Uint32Value(),
                args[1]->Int32Value(),
                args[2]->Int32Value()
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform3i){
        NanScope();
        glUniform3i(
                args[0]->Uint32Value(),
                args[1]->Int32Value(),
                args[2]->Int32Value(),
                args[3]->Int32Value()
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform4i){
        NanScope();
        glUniform4i(
                args[0]->Uint32Value(),
                args[1]->Int32Value(),
                args[2]->Int32Value(),
                args[3]->Int32Value(),
                args[4]->Int32Value()
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform1iv){
        NanScope();
        int count = 0;
        GLint *value = getArrayData<GLint>(args[1],&count);

        glUniform1iv(
                args[0]->Uint32Value(),
                count,
                value
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform2iv){
        NanScope();
        int count = 0;
        GLint *value = getArrayData<GLint>(args[1],&count);

        glUniform2iv(
                args[0]->Uint32Value(),
                count / 2,
                value
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform3iv){
        NanScope();
        int count = 0;
        GLint *value = getArrayData<GLint>(args[1],&count);

        glUniform3iv(
                args[0]->Uint32Value(),
                count / 3,
                value
        );
        NanReturnUndefined();
    }

    NAN_METHOD(Uniform4iv){
        NanScope();
        int count = 0;
        GLint *value = getArrayData<GLint>(args[1],&count);

        glUniform4iv(
                args[0]->Uint32Value(),
                count / 4,
                value
        );
        NanReturnUndefined();
    }

    NAN_METHOD(UniformMatrix2fv){
        NanScope();
        GLint location = args[0]->Int32Value();
        GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());

        GLsizei count = 0;
        GLfloat *data = getArrayData<GLfloat>(args[2],&count);

        glUniformMatrix2fv(
                location,
                count / 4,
                transpose,
                data
        );
        NanReturnUndefined();
    }

    NAN_METHOD(UniformMatrix3fv){
        NanScope();
        GLint location = args[0]->Int32Value();
        GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());

        GLsizei count = 0;
        GLfloat *data = getArrayData<GLfloat>(args[2],&count);

        glUniformMatrix3fv(
                location,
                count / 9,
                transpose,
                data
        );
        NanReturnUndefined();
    }

    NAN_METHOD(UniformMatrix4fv){
        NanScope();
        GLint location = args[0]->Int32Value();
        GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());

        GLsizei count = 0;
        GLfloat *data = getArrayData<GLfloat>(args[2],&count);

        glUniformMatrix4fv(
                location,
                count / 16,
                transpose,
                data
        );
        NanReturnUndefined();
    }

    //endregion

    /*--------------------------------------------------------------------------------------------*/
    //region BUFFER
    /*--------------------------------------------------------------------------------------------*/

    NAN_METHOD(CreateBuffer){
        NanScope();
        GLuint buffer;
        glGenBuffers(1,&buffer);
        buffers.push_back(buffer);
        NanReturnValue(V8_INT(buffer));
    }

    NAN_METHOD(BindBuffer){
        NanScope();
        glBindBuffer(args[0]->Uint32Value(),args[1]->Uint32Value());
        NanReturnUndefined();
    }

    NAN_METHOD(BufferData){
        NanScope();

        GLenum target = args[0]->Uint32Value();
        GLenum usage  = args[2]->Uint32Value();

        if(args[1]->IsObject()){
            Local<Object> obj = Local<Object>::Cast(args[1]);

            int elementSize = ELEMENT_SIZES_FOR_ARRAY_TYPES[obj->GetIndexedPropertiesExternalArrayDataType()];
            GLsizeiptr size = obj->GetIndexedPropertiesExternalArrayDataLength() * elementSize;
            void *data      = obj->GetIndexedPropertiesExternalArrayData();

            glBufferData(target,size,data,usage);
        } else if(args[1]->IsNumber()){
            GLsizeiptr size = args[1]->Uint32Value();
            glBufferData(target,size,NULL,usage);
        }
        NanReturnUndefined();
    }

    NAN_METHOD(BufferSubData){
        NanScope();
        GLenum target = args[0]->Uint32Value();
        GLintptr offset = args[1]->Uint32Value();
        Local<Object> obj = Local<Object>::Cast(args[2]);

        int elementSize = ELEMENT_SIZES_FOR_ARRAY_TYPES[obj->GetIndexedPropertiesExternalArrayDataType()];
        GLsizeiptr size = obj->GetIndexedPropertiesExternalArrayDataLength() * elementSize;
        void *data      = obj->GetIndexedPropertiesExternalArrayData();

        glBufferSubData(target,offset,size,data);
        NanReturnUndefined();
    }

    NAN_METHOD(DeleteBuffer){
        NanScope();
        GLuint buffer = args[0]->Uint32Value();
        glDeleteBuffers(1,&buffer);
        removeGLObject(&buffers,buffer);
        NanReturnUndefined();
    }

    NAN_METHOD(GetBufferParameter){
        NanScope();
//        glGetBufferParameteriv()
        NanReturnUndefined();
    }

    NAN_METHOD(IsBuffer){
        NanScope();
        NanReturnValue(V8_BOOL(glIsBuffer(args[0]->Uint32Value())));
    }

    //endregion



    void init(Handle<Object> exports){
        //View and Clip

        NODE_SET_METHOD(exports,"attachShader", AttachShader);
        NODE_SET_METHOD(exports,"bindAttribLocation", BindAttribLocation);
        NODE_SET_METHOD(exports,"compileShader", CompileShader);
        NODE_SET_METHOD(exports,"createProgram", CreateProgram);
        NODE_SET_METHOD(exports,"createShader", CreateShader);
        NODE_SET_METHOD(exports,"deleteProgram", DeleteProgram);
        NODE_SET_METHOD(exports,"deleteShader", DeleteShader);
        NODE_SET_METHOD(exports,"detachShader", DetachShader);
//        NODE_SET_METHOD(exports,"getAttachedShaders", GetAttachedShaders);
        NODE_SET_METHOD(exports,"getProgramParameter",GetProgramParameter);
        NODE_SET_METHOD(exports,"getProgramInfoLog",GetProgramInfoLog);
        NODE_SET_METHOD(exports,"getShaderParameter",GetShaderParameter);
        NODE_SET_METHOD(exports,"getShaderInfoLog",GetShaderInfoLog);
        NODE_SET_METHOD(exports,"getShaderSource",GetShaderSource);
        NODE_SET_METHOD(exports,"isProgram",IsProgram);
        NODE_SET_METHOD(exports,"isShader",IsShader);
        NODE_SET_METHOD(exports,"linkProgram", LinkProgram);
        NODE_SET_METHOD(exports,"shaderSource", ShaderSource);
        NODE_SET_METHOD(exports,"useProgram", UseProgram);
        NODE_SET_METHOD(exports,"validateShader", ValidateShader);

        NODE_SET_METHOD(exports,"createBuffer",CreateBuffer);
        NODE_SET_METHOD(exports,"bindBuffer",BindBuffer);
        NODE_SET_METHOD(exports,"bufferData",BufferData);
        NODE_SET_METHOD(exports,"bufferSubData",BufferSubData);
        NODE_SET_METHOD(exports,"deleteBuffer",DeleteBuffer);
//        NODE_SET_METHOD(exports,"getBufferParameter",GetBufferParameter);
        NODE_SET_METHOD(exports,"IsBuffer",IsBuffer);


        GL_CONSTANT_EX("ARRAY_BUFFER",GL_ARRAY_BUFFER);
        GL_CONSTANT_EX("ELEMENT_ARRAY_BUFFER",GL_ELEMENT_ARRAY_BUFFER);
        GL_CONSTANT_EX("STATIC_DRAW",GL_STATIC_DRAW);
        GL_CONSTANT_EX("DYNAMIC_DRAW",GL_DYNAMIC_DRAW);

        GL_CONSTANT_EX("VERTEX_SHADER",GL_VERTEX_SHADER);
        GL_CONSTANT_EX("FRAGMENT_SHADER",GL_FRAGMENT_SHADER);

        GL_CONSTANT_EX("COMPILE_STATUS", GL_COMPILE_STATUS);
//

//        GL_CONST(VERTEX_SHADER);
//        GL_CONST(GL_FRAGMENT_SHADER);
    }
}

