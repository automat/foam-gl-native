#include "gl.h"
#include "../utils/utils.h"

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
#include <FreeImage.h>

#include "glConsts.h"

using namespace std;

/*--------------------------------------------------------------------------------------------*/
// REFS & UTILITIES
/*--------------------------------------------------------------------------------------------*/

map<v8::ExternalArrayType, GLuint> ELEMENT_SIZES_FOR_ARRAY_TYPES{
        {v8::kExternalByteArray, 1},
        {v8::kExternalUnsignedByteArray, 1},
        {v8::kExternalShortArray, 2},
        {v8::kExternalUnsignedShortArray, 2},
        {v8::kExternalIntArray, 4},
        {v8::kExternalUnsignedIntArray, 4},
        {v8::kExternalFloatArray, 4}
};

//const GLfloat* getUniformFloat32ArrayData(Local<Object> value, int lengthExp){
//    cout << value->GetIndexedPropertiesExternalArrayDataType() << endl;
//    if(value->GetIndexedPropertiesExternalArrayDataType() != kExternalFloatArray){
//        NanThrowTypeError("Array arg not of type Float32Array.");
//    } else if(value->GetIndexedPropertiesExternalArrayDataLength() != lengthExp){
//        stringstream ss;
//        ss << "Float32Array length must be " << lengthExp << ".";
//        NanThrowError(ss.str().c_str());
//    }
//    return static_cast<const GLfloat*>(value->GetIndexedPropertiesExternalArrayData());
//}
//
//const GLint* getUniformIntArrayData(Local<Object> value, int lengthExp){
//    if(value->GetIndexedPropertiesExternalArrayDataType() != kExternalIntArray){
//        NanThrowTypeError("Array arg not of type IntArray.");
//    } else if(value->GetIndexedPropertiesExternalArrayDataLength() != lengthExp){
//        stringstream ss;
//        ss << "IntArray length must be " << lengthExp << ".";
//        NanThrowError(ss.str().c_str());
//    }
//    return static_cast<const GLint*>(value->GetIndexedPropertiesExternalArrayData());
//}

NAN_WEAK_CALLBACK(ExternalCallback) {
    data.GetValue()->Neuter();
    delete[] static_cast<char*>(data.GetParameter());
}

template <typename Type>
Type* getArrayData(Local<Value> arg, int* num = NULL);

template<>
GLfloat* getArrayData<GLfloat>(Local<Value> arg, int* num) {
    GLfloat *data=NULL;
    if(num) *num=0;

    if(!arg->IsNull()) {
        if(arg->IsArray()) {
            Local<Array> arr = Local<Array>::Cast(arg);
            if(num) *num=arr->Length();
            data = reinterpret_cast<GLfloat*>(arr->GetIndexedPropertiesExternalArrayData());
        }
        else if(arg->IsArrayBufferView()) {
            Local<ArrayBufferView> obj = arg.As<ArrayBufferView>();
            Local<ArrayBuffer> buffer = obj->Buffer();
            if (!buffer->IsExternal()) {
                ArrayBuffer::Contents contents = buffer->Externalize();
                obj->SetIndexedPropertiesToExternalArrayData(contents.Data(), kExternalFloat32Array, contents.ByteLength() / 4);
                NanMakeWeakPersistent(buffer, contents.Data(), ExternalCallback);
            }
            if(num) *num = obj->GetIndexedPropertiesExternalArrayDataLength();
            data = reinterpret_cast<GLfloat*>(obj->GetIndexedPropertiesExternalArrayData());

        }
        else {
            NanThrowError("Bad array argument");
            return NULL;
        }
    }

    return data;
}

template<>
GLint* getArrayData(Local<Value> arg, int* num) {
    GLint *data=NULL;
    if(num) *num=0;

    if(!arg->IsNull()) {
        if(arg->IsArray()) {
            Local<Array> arr = Local<Array>::Cast(arg);
            if(num) *num=arr->Length();
            data = reinterpret_cast<GLint*>(arr->GetIndexedPropertiesExternalArrayData());
        }
        else if(arg->IsArrayBufferView()) {
            Local<ArrayBufferView> obj = arg.As<ArrayBufferView>();
            Local<ArrayBuffer> buffer = obj->Buffer();
            if (!buffer->IsExternal()) {
                ArrayBuffer::Contents contents = buffer->Externalize();
                obj->SetIndexedPropertiesToExternalArrayData(contents.Data(), kExternalInt32Array, contents.ByteLength() / 4);
                NanMakeWeakPersistent(buffer, contents.Data(), ExternalCallback);
            }
            if(num) *num = obj->GetIndexedPropertiesExternalArrayDataLength();
            data = reinterpret_cast<GLint*>(obj->GetIndexedPropertiesExternalArrayData());

        }
        else {
            NanThrowError("Bad array argument");
            return NULL;
        }
    }

    return data;
}

template<>
void* getArrayData<void>(Local<Value> arg, int* num) {
    void *data=NULL;
    if(num) *num=0;

    if(!arg->IsNull()) {
        if(arg->IsArray()) {
            Local<Array> arr = Local<Array>::Cast(arg);
            if(num) *num=arr->Length();
            data = arr->GetIndexedPropertiesExternalArrayData();
        }
        else if(arg->IsArrayBufferView()) {
            Local<ArrayBufferView> obj = arg.As<ArrayBufferView>();
            Local<ArrayBuffer> buffer = obj->Buffer();
            if (!buffer->IsExternal()) {
                ArrayBuffer::Contents contents = buffer->Externalize();
                obj->SetIndexedPropertiesToExternalArrayData(contents.Data(), kExternalInt32Array, contents.ByteLength() / 4);
                NanMakeWeakPersistent(buffer, contents.Data(), ExternalCallback);
            }
            if(num) *num = obj->GetIndexedPropertiesExternalArrayDataLength();
            data = obj->GetIndexedPropertiesExternalArrayData();

        }
        else {
            NanThrowError("Bad array argument");
            return NULL;
        }
    }

    return data;
}

void *getImageData(Local<Object> value){
    if(value->IsNull() || !value->IsObject()){
        NanThrowTypeError("Wrong texture data");
    }
    return value->GetIndexedPropertiesExternalArrayData();
}

//template<typename T,v8::ExternalArrayType C,string S>
//const T* getUniformArrayData(Local<Object> value, int lengthExp){
//    if(value->GetIndexedPropertiesExternalArrayDataType() != C){
//        stringstream ss;
//        ss << "Array arg not of " << S << ".";
//        NanThrowTypeError(ss.str().c_str());
//    } else if(value->GetIndexedPropertiesExternalArrayDataLength() != lengthExp){
//        stringstream ss;
//        ss << S << " length must be " << lengthExp << ".";
//        NanThrowError(ss.str().c_str());
//    }
//    return static_cast<const T*>(value->GetIndexedPropertiesExternalArrayData());
//}

//const void* getArrayData(Local<Object> value){
//    if(value->IsNull()){
//        NanThrowError("Array data is null.");
//    }
//    ExternalArrayType type = value->GetIndexedPropertiesExternalArrayDataType();
//    if(type != kExternalByteArray || type != kExternalUnsignedByteArray ||
//       type != kExternalShortArray || type != kExternalUnsignedShortArray ||
//       type != kExternalIntArray || type != kExternalUnsignedIntArray ||
//       type != kExternalFloatArray){
//        NanThrowTypeError("Array type not supported.");
//    }
//    return value->GetIndexedPropertiesExternalArrayData();
//}

const GLfloat* getFloat32ArrayData(Local<Object> value, int expLen = -1){
    if(value->IsNull()){
        NanThrowError("Array data is null.");
    }
    if(value->GetIndexedPropertiesExternalArrayDataType() != kExternalFloatArray){
        NanThrowTypeError("Object is not of type Float32Array.");
    }
    int len = value->GetIndexedPropertiesExternalArrayDataLength();
    if(expLen > 0 && expLen != len){
        stringstream ss;
        ss << "Float32Array length should be " << expLen << " not " << len << ".";
        NanThrowRangeError(ss.str().c_str());
    }
    return reinterpret_cast<const GLfloat *>(value->GetIndexedPropertiesExternalArrayData());
}

/*--------------------------------------------------------------------------------------------*/
// OPENGL ERRORS
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(getError){
    NanScope();
    GLenum error = glGetError();
    NanReturnValue(NanNew(error));
}

/*--------------------------------------------------------------------------------------------*/
// VERTEX ARRAYS
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(vertexPointer){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLint size = args[0]->Int32Value();
    GLenum type = args[1]->Uint32Value();
    GLsizei stride = args[2]->Uint32Value();
    const void *pointer = getArrayData<void>(args[3]->ToObject());
    glVertexPointer(size,type,stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(normalPointer){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum type = args[0]->Uint32Value();
    GLsizei stride = args[1]->Int32Value();
    const void *pointer = getArrayData<void>(args[2]->ToObject());
    glNormalPointer(type,stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(colorPointer){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLint size = args[0]->Int32Value();
    GLenum type = args[1]->Uint32Value();
    GLsizei stride = args[2]->Uint32Value();
    const void *pointer = getArrayData<void>(args[3]->ToObject());
    glColorPointer(size,type,stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(secondaryColorPointer){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLint size = args[0]->Int32Value();
    GLenum type = args[1]->Uint32Value();
    GLsizei stride = args[3]->Uint32Value();
    const void *pointer = getArrayData<void>(args[4]->ToObject());
    glSecondaryColorPointer(size,type,stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(indexPointer){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum type = args[0]->Uint32Value();
    GLsizei stride = args[1]->Uint32Value();
    const void *pointer = getArrayData<void>(args[2]->ToObject());
    glIndexPointer(type,stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(edgeFlagPointer){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLsizei stride = args[0]->Uint32Value();
    const void *pointer = getArrayData<void>(args[1]->ToObject());
    glEdgeFlagPointer(stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(fogCoordPointer){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum type = args[0]->Uint32Value();
    GLsizei stride = args[1]->Uint32Value();
    const void *pointer = getArrayData<void>(args[2]->ToObject());
    glFogCoordPointer(type,stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(texCoordPointer){
    NanScope();
    GLint size = args[0]->Int32Value();
    GLenum type = args[1]->Uint32Value();
    GLsizei stride = args[2]->Uint32Value();
    const void *pointer = getArrayData<void>(args[3]->ToObject());
    glTexCoordPointer(size,type,stride,pointer);
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

NAN_METHOD(vertexAttribIPointer){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLuint index = args[0]->Uint32Value();
    GLint size = args[1]->Int32Value();
    GLenum type = args[2]->Uint32Value();
    GLsizei stride = args[3]->Uint32Value();
    const void *pointer = getArrayData<void>(args[4]->ToObject());
    glVertexAttribIPointer(index,size,type,stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(vertexAttribLPointer){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLuint index = args[0]->Uint32Value();
    GLint size = args[1]->Int32Value();
    GLenum type = args[2]->Uint32Value();
    GLsizei stride = args[3]->Uint32Value();
    const void *pointer = getArrayData<void>(args[4]->ToObject());
    glVertexAttribLPointer(index,size,type,stride,pointer);
    NanReturnUndefined();
}

NAN_METHOD(enableClientState){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum array = args[0]->Uint32Value();
    glEnableClientState(array);
    NanReturnUndefined();
}

NAN_METHOD(disableClientState){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum array = args[0]->Uint32Value();
    glDisableClientState(array);
    NanReturnUndefined();
}

NAN_METHOD(vertexAttribDivisor){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint index = args[0]->Uint32Value();
    GLuint divisor = args[1]->Uint32Value();
    glVertexAttribDivisor(index,divisor);
    NanReturnUndefined();
}

NAN_METHOD(clientActiveTexture){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum texture = args[0]->Uint32Value();
    glClientActiveTexture(texture);
    NanReturnUndefined();
}

NAN_METHOD(arrayElement){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLint i = args[0]->Int32Value();
    glArrayElement(i);
    NanReturnUndefined();
}

//region DRAWING COMMANDS
NAN_METHOD(drawArrays){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum mode = args[0]->Uint32Value();
    GLint first = args[1]->Int32Value();
    GLsizei count = args[2]->Uint32Value();
    glDrawArrays(mode, first, count);
    NanReturnUndefined();
}

NAN_METHOD(drawArraysInstanced){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLenum mode = args[0]->Uint32Value();
    GLint first = args[1]->Int32Value();
    GLsizei count = args[2]->Uint32Value();
    GLsizei primcount = args[3]->Uint32Value();
    glDrawArraysInstanced(mode,first,count,primcount);
    NanReturnUndefined();
}

//drawArraysIndirect
//multiDrawArrays

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

NAN_METHOD(drawElementsInstanced){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLenum  mode      = args[0]->Uint32Value();
    GLsizei count     = args[1]->Uint32Value();
    GLenum  type      = args[2]->Uint32Value();
    GLvoid *indices   = reinterpret_cast<GLvoid *>(args[3]->Uint32Value());
    GLsizei primcount = args[4]->Uint32Value();

    glDrawElementsInstanced(mode,count,type,indices,primcount);
    NanReturnUndefined();
}

//multiDrawElements

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

//drawElementsBaseVertex
//drawRangeElementsBaseVertex
//drawElementsInstancedBaseVertex
//drawElementsIndirect
//multiDrawElementsBaseVertex

//endregion

/*--------------------------------------------------------------------------------------------*/
// VERTEX SPECIFICATION – (use buffers, this is just for completeness)
/*--------------------------------------------------------------------------------------------*/

//region BEGIN AND END
NAN_METHOD(begin){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum mode = args[0]->Uint32Value();
    glBegin(mode);
    NanReturnUndefined();
}

NAN_METHOD(end){
    NanScope();
    glEnd();
    NanReturnUndefined();
}

NAN_METHOD(patchParameteri){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum pname = args[0]->Uint32Value();
    GLint value = args[1]->Int32Value();
    glPatchParameteri(pname,value);
    NanReturnUndefined();
}
//endregion

//region POLYGON EDGES
NAN_METHOD(edgeFlag){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLboolean flag = static_cast<GLboolean>(args[0]->BooleanValue());
    glEdgeFlag(flag);
    NanReturnUndefined();
}
//edgeFlagv
//endregion

//region VERTEX SPECIFICATION
NAN_METHOD(vertex2f){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLfloat x = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat y = static_cast<GLfloat>(args[1]->NumberValue());
    glVertex2f(x,y);
    NanReturnUndefined();
}

NAN_METHOD(vertex3f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLfloat x = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat y = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat z = static_cast<GLfloat>(args[2]->NumberValue());
    glVertex3f(x,y,z);
    NanReturnUndefined();
}

NAN_METHOD(vertex4f){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLfloat x = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat y = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat z = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat w = static_cast<GLfloat>(args[3]->NumberValue());
    glVertex4f(x,y,z,w);
    NanReturnUndefined();
}

NAN_METHOD(vertex2fv){
    NanScope();
    CHECK_ARGS_LEN(1);
    const GLfloat *coords = getFloat32ArrayData(args[0]->ToObject());
    glVertex2fv(coords);
    NanReturnUndefined();
}

NAN_METHOD(vertex3fv){
    NanScope();
    CHECK_ARGS_LEN(1);
    const GLfloat *coords = getFloat32ArrayData(args[0]->ToObject());
    glVertex3fv(coords);
    NanReturnUndefined();
}

NAN_METHOD(vertex4fv){
    NanScope();
    CHECK_ARGS_LEN(1);
    const GLfloat *coords = getFloat32ArrayData(args[0]->ToObject());
    glVertex4fv(coords);
    NanReturnUndefined();
}

//vertexP{234}ui
//vertexP{234}uiv

NAN_METHOD(texCoord1f){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLfloat s = static_cast<GLfloat>(args[0]->NumberValue());
    glTexCoord1f(s);
    NanReturnUndefined();
}

NAN_METHOD(texCoord2f){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLfloat s = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat t = static_cast<GLfloat>(args[1]->NumberValue());
    glTexCoord2f(s,t);
    NanReturnUndefined();
}

NAN_METHOD(texCoord3f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLfloat s = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat t = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat r = static_cast<GLfloat>(args[2]->NumberValue());
    glTexCoord3f(s,t,r);
    NanReturnUndefined();
}

NAN_METHOD(texCoord4f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLfloat s = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat t = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat r = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat q = static_cast<GLfloat>(args[3]->NumberValue());
    glTexCoord4f(s,t,r,q);
    NanReturnUndefined();
}

//texCoordP{1234}ui
//texCoordP{1234}uiv

NAN_METHOD(multiTexCoord1f){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum texture = args[0]->Uint32Value();
    GLfloat s = static_cast<GLfloat>(args[1]->NumberValue());
    glMultiTexCoord1f(texture,s);
    NanReturnUndefined();
}

NAN_METHOD(multiTexCoord2f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum texture = args[0]->Uint32Value();
    GLfloat s = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat t = static_cast<GLfloat>(args[2]->NumberValue());
    glMultiTexCoord2f(texture,s,t);
    NanReturnUndefined();
}

NAN_METHOD(multiTexCoord3f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum texture = args[0]->Uint32Value();
    GLfloat s = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat t = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat r = static_cast<GLfloat>(args[3]->NumberValue());
    glMultiTexCoord3f(texture,s,t,r);
    NanReturnUndefined();
}

NAN_METHOD(multiTexCoord4f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum texture = args[0]->Uint32Value();
    GLfloat s = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat t = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat r = static_cast<GLfloat>(args[3]->NumberValue());
    GLfloat q = static_cast<GLfloat>(args[4]->NumberValue());
    glMultiTexCoord4f(texture,s,t,r,q);
    NanReturnUndefined();
}

NAN_METHOD(multiTexCoord1fv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum texture = args[0]->Uint32Value();
    const GLfloat *v = getFloat32ArrayData(args[0]->ToObject());
    glMultiTexCoord1fv(texture,v);
    NanReturnUndefined();
}

NAN_METHOD(multiTexCoord2fv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum texture = args[0]->Uint32Value();
    const GLfloat *v = getFloat32ArrayData(args[0]->ToObject());
    glMultiTexCoord2fv(texture,v);
    NanReturnUndefined();
}

NAN_METHOD(multiTexCoord3fv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum texture = args[0]->Uint32Value();
    const GLfloat *v = getFloat32ArrayData(args[0]->ToObject());
    glMultiTexCoord3fv(texture,v);
    NanReturnUndefined();
}

NAN_METHOD(multiTexCoord4fv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum texture = args[0]->Uint32Value();
    const GLfloat *v = getFloat32ArrayData(args[0]->ToObject());
    glMultiTexCoord4fv(texture,v);
    NanReturnUndefined();
}

//multiTexCoordP{1234}ui(
//multiTexCoordP{1234}uiv

NAN_METHOD(normal3f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLfloat nx = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat ny = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat nz = static_cast<GLfloat>(args[2]->NumberValue());
    glNormal3f(nx,ny,nz);
    NanReturnUndefined();
}

NAN_METHOD(normal3fv){
    NanScope();
    CHECK_ARGS_LEN(1);
    const GLfloat *v = getFloat32ArrayData(args[0]->ToObject());
    glNormal3fv(v);
    NanReturnUndefined();
}

//normalP3ui
//normalP3uiv

NAN_METHOD(fogCoordf){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLfloat coord = static_cast<GLfloat>(args[0]->NumberValue());
    glFogCoordf(coord);
    NanReturnUndefined();
}

NAN_METHOD(fogCoordfv){
    NanScope();
    CHECK_ARGS_LEN(2);
    const GLfloat *coord = getFloat32ArrayData(args[0]->ToObject());
    glFogCoordfv(coord);
    NanReturnUndefined();
}

NAN_METHOD(color3i){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLint red = args[0]->Int32Value();
    GLint green = args[1]->Int32Value();
    GLint blue = args[2]->Int32Value();
    glColor3i(red,green,blue);
    NanReturnUndefined();
}

NAN_METHOD(color4i){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLint red = args[0]->Int32Value();
    GLint green = args[1]->Int32Value();
    GLint blue = args[2]->Int32Value();
    GLint alpha = args[3]->Int32Value();
    glColor4i(red,green,blue,alpha);
    NanReturnUndefined();
}

NAN_METHOD(color3f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLfloat red = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat green = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat blue = static_cast<GLfloat>(args[2]->NumberValue());
    glColor3f(red,green,blue);
    NanReturnUndefined();
}

NAN_METHOD(color4f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLfloat red = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat green = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat blue = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat alpha = static_cast<GLfloat>(args[3]->NumberValue());
    glColor4f(red,green,blue,alpha);
    NanReturnUndefined();
}

NAN_METHOD(color3fv){
    NanScope();
    CHECK_ARGS_LEN(1);
    const GLfloat *v = getFloat32ArrayData(args[0]->ToObject());
    glColor3fv(v);
    NanReturnUndefined();
}

NAN_METHOD(color4fv){
    NanScope();
    CHECK_ARGS_LEN(1);
    const GLfloat *v = getFloat32ArrayData(args[0]->ToObject());
    glColor4fv(v);
    NanReturnUndefined();
}

//color{34}{bsifd ubusui}
//colorP{34}ui
//colorP{34}uiv

NAN_METHOD(secondaryColor3i){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLint red = args[0]->Int32Value();
    GLint green = args[1]->Int32Value();
    GLint blue = args[2]->Int32Value();
    glSecondaryColor3i(red,green,blue);
    NanReturnUndefined();
}

NAN_METHOD(secondaryColor3f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLfloat red = static_cast<GLfloat>(args[0]->NumberValue());
    GLfloat green = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat blue = static_cast<GLfloat>(args[2]->NumberValue());
    glSecondaryColor3f(red,green,blue);
    NanReturnUndefined();
}

//secondaryColor3{bsid ubusui}
//secondaryColor3{bsifd ubusui}v

//index{sifd ub}
//index{sifd ub}v

NAN_METHOD(vertexAttrib1f){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint index = args[0]->Uint32Value();
    GLfloat x = static_cast<GLfloat>(args[1]->NumberValue());
    glVertexAttrib1f(index,x);
    NanReturnUndefined();
}

NAN_METHOD(vertexAttrib2f){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLuint index = args[0]->Uint32Value();
    GLfloat x = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat y = static_cast<GLfloat>(args[2]->NumberValue());
    glVertexAttrib2f(index,x,y);
    NanReturnUndefined();
}

NAN_METHOD(vertexAttrib3f){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLuint index = args[0]->Uint32Value();
    GLfloat x = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat y = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat z = static_cast<GLfloat>(args[3]->NumberValue());
    glVertexAttrib3f(index,x,y,z);
    NanReturnUndefined();
}

NAN_METHOD(vertexAttrib4f){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLuint index = args[0]->Uint32Value();
    GLfloat x = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat y = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat z = static_cast<GLfloat>(args[3]->NumberValue());
    GLfloat w = static_cast<GLfloat>(args[4]->NumberValue());
    glVertexAttrib4f(index,x,y,z,w);
    NanReturnUndefined();
}

//endregion

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
            NanReturnValue(NanNew(params));
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
    NanReturnValue(NanNew(enabled));
}


/*--------------------------------------------------------------------------------------------*/
// VIEWPORT & CLIPPING
/*--------------------------------------------------------------------------------------------*/

//region CONTROLLING VIEWPORT
//depthRangeArrayv
NAN_METHOD(depthRangeIndexed){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLuint index = args[0]->Uint32Value();
    GLclampd n = args[0]->NumberValue();
    GLclampd f = args[1]->NumberValue();
    glDepthRangeIndexed(index,n,f);
    NanReturnUndefined();
}

NAN_METHOD(depthRange) {
    NanScope();
    CHECK_ARGS_LEN(2);
    GLclampd n = args[0]->NumberValue();
    GLclampd f = args[1]->NumberValue();
    glDepthRange(n,f);
    NanReturnUndefined();
}

//viewportArrayv

NAN_METHOD(viewportIndexedf){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLuint index = args[0]->Uint32Value();
    GLfloat x = static_cast<GLfloat>(args[1]->NumberValue());
    GLfloat y = static_cast<GLfloat>(args[2]->NumberValue());
    GLfloat w = static_cast<GLfloat>(args[3]->NumberValue());
    GLfloat h = static_cast<GLfloat>(args[4]->NumberValue());
    glViewportIndexedf(index,x,y,w,h);
    NanReturnUndefined();
}

//viewportIndexedfv

NAN_METHOD(viewport) {
    NanScope();
    CHECK_ARGS_LEN(4);
    GLint x = args[0]->Int32Value();
    GLint y = args[1]->Int32Value();
    GLsizei w = args[2]->Uint32Value();
    GLsizei h = args[3]->Uint32Value();
    glViewport(x,y,w,h);
    NanReturnUndefined();
}
//endregion

//region CLIPPING
//clipPlane
//getClipPlane
//endregion

/*--------------------------------------------------------------------------------------------*/
// SHADERS AND PROGRAMS
/*--------------------------------------------------------------------------------------------*/

//region SHADER OBJECTS

NAN_METHOD(createShader) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint shader = glCreateShader(args[0]->Uint32Value());
    NanReturnValue(NanNew(shader));
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
    NanReturnUndefined();
}
//endregion

//region PROGRAM OBJECTS

NAN_METHOD(createProgram) {
    NanScope();
    GLuint program = glCreateProgram();
    NanReturnValue(NanNew(program));
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
    NanReturnUndefined();
}
//endregion

//region PROGRAM PIPELINE OBJECTS

NAN_METHOD(createProgramPipeline){
    NanScope();
    GLuint pipeline;
    glGenProgramPipelines(1,&pipeline);
    NanReturnValue(NanNew(pipeline));
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
    NanReturnValue(NanNew(glGetAttribLocation(program,*name)));
}

NAN_METHOD(getActiveAttrib){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    GLuint index = args[1]->Uint32Value();
    GLsizei maxLength = 1024;
    GLsizei length;
    GLsizei size;
    GLenum type;
    char name[maxLength];
    glGetActiveAttrib(program,index,maxLength,&length,&size,&type,name);
    Local<Object> out = NanNew<Object>();
    out->Set(NanNew("size"),NanNew(size));
    out->Set(NanNew("type"),NanNew(type));
    out->Set(NanNew("name"),NanNew(name));
    NanReturnValue(out);
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

//endregion

//region UNIFORM VARIABLES

NAN_METHOD(getUniformLocation){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value name(args[1]);
    NanReturnValue(NanNew(glGetUniformLocation(program,*name)));
}

NAN_METHOD(getUniformBlockIndex){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value uniformBlockName(args[1]);
    NanReturnValue(NanNew(glGetUniformBlockIndex(program,*uniformBlockName)));
}

//getActiveUniformBlockName
//getActiveUniformBlockiv
//getUniformIndices
//getActiveUniformName

NAN_METHOD(getActiveUniform){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    GLuint index = args[1]->Uint32Value();
    GLsizei maxLength = 1024;
    GLsizei length;
    GLsizei size;
    GLenum type;
    char name[maxLength];
    glGetActiveUniform(program,index,maxLength,&length,&size,&type,name);
    Local<Object> out = NanNew<Object>();
    out->Set(NanNew("size"),NanNew(size));
    out->Set(NanNew("type"),NanNew(type));
    out->Set(NanNew("name"),NanNew(name));
    NanReturnValue(out);
}

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

NAN_METHOD(uniform1fv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint location = args[0]->Uint32Value();
    Local<Object> obj = args[2]->ToObject();
//    const GLfloat *value = getArrayData(obj, 1);
    const GLfloat *value = getArrayData<GLfloat>(obj);
    glUniform1fv(location,1,value);
    NanReturnUndefined();
}

NAN_METHOD(uniform2fv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint location = args[0]->Uint32Value();
    Local<Object> obj = args[2]->ToObject();
//    const GLfloat *value = getArrayData(obj, 2);
    const GLfloat *value = getArrayData<GLfloat>(obj);
    glUniform2fv(location,1,value);
    NanReturnUndefined();
}

NAN_METHOD(uniform3fv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint location = args[0]->Uint32Value();
    Local<Object> obj = args[2]->ToObject();
//    const GLfloat *value = getArrayData(obj, 3);
    const GLfloat *value = getArrayData<GLfloat>(obj);
    glUniform3fv(location,1,value);
    NanReturnUndefined();
}

NAN_METHOD(uniform4fv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint location = args[0]->Uint32Value();
    Local<Object> obj = args[2]->ToObject();
//    const GLfloat *value = getArrayData(obj,4);
    const GLfloat *value = getArrayData<GLfloat>(obj);
    glUniform4fv(location,1,value);
    NanReturnUndefined();
}

NAN_METHOD(uniform1iv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint location = args[0]->Uint32Value();
    Local<Object> obj = args[2]->ToObject();
//    const GLint *value = getArrayData<GLint>(obj, 1);
    const GLint *value = getArrayData<GLint>(obj);
    glUniform1iv(location,1,value);
    NanReturnUndefined();
}

NAN_METHOD(uniform2iv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint location = args[0]->Uint32Value();
    Local<Object> obj = args[2]->ToObject();
//    const GLint *value = getUniformIntArrayData(obj, 2);
    const GLint *value = getArrayData<GLint>(obj);
    glUniform2iv(location,1,value);
    NanReturnUndefined();
}

NAN_METHOD(uniform3iv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint location = args[0]->Uint32Value();
    Local<Object> obj = args[2]->ToObject();
//    const GLint *value = getUniformIntArrayData(obj, 3);
    const GLint *value = getArrayData<GLint>(obj);
    glUniform3iv(location,1,value);
    NanReturnUndefined();
}

NAN_METHOD(uniform4iv){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint location = args[0]->Uint32Value();
    Local<Object> obj = args[2]->ToObject();
//    const GLint *value = getUniformIntArrayData(obj, 4);
    const GLint *value = getArrayData<GLint>(obj);
    glUniform4iv(location,1,value);
    NanReturnUndefined();
}


NAN_METHOD(uniformMatrix2fv) {
    NanScope();
    CHECK_ARGS_LEN(3);
    GLint     location  = args[0]->Int32Value();
    GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());

    Local<Object> obj = args[2]->ToObject();
//    const GLfloat *value = getUniformFloat32ArrayData(obj, 4);
    const GLfloat *value = getArrayData<GLfloat>(obj);
    glUniformMatrix2fv(location,1,transpose,value);
    NanReturnUndefined();
}

NAN_METHOD(uniformMatrix3fv) {
    NanScope();
    CHECK_ARGS_LEN(3);
    GLint     location  = args[0]->Int32Value();
    GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());

    Local<Object> obj = args[2]->ToObject();
//    const GLfloat *value = getUniformFloat32ArrayData(obj, 9);
    const GLfloat *value = getArrayData<GLfloat>(obj);
    glUniformMatrix3fv(location,1,transpose,value);
    NanReturnUndefined();
}

NAN_METHOD(uniformMatrix4fv) {
    NanScope();
    CHECK_ARGS_LEN(3);

    GLint     location  = args[0]->Int32Value();
    GLboolean transpose = static_cast<GLboolean>(args[1]->BooleanValue());
    const GLfloat *value = getArrayData<GLfloat>(args[2]->ToObject());

    glUniformMatrix4fv(location,1,transpose,value);
    NanReturnUndefined();
}

//glProgramUniform{1234}ui
//glProgramUniform{1234}uiv
//glProgramUniformMatrix{234}{fd}v
//glProgramUniformMatrixf{2x3,3x2,2x4, 4x2,3x4,4x3}{fd}v
//endregion

//region UNIFORM BUFFER OBJECTS BINDINGS
NAN_METHOD(UniformBlockBinding){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLuint program = args[0]->Uint32Value();
    GLuint uniformBlockIndex = args[1]->Uint32Value();
    GLuint uniformBlockBinding = args[2]->Uint32Value();
    glUniformBlockBinding(program,uniformBlockIndex,uniformBlockBinding);
    NanReturnUndefined();
}
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
    NanReturnValue(NanNew(location));
}

NAN_METHOD(getFrageDataIndex){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint program = args[0]->Uint32Value();
    String::Utf8Value name(args[1]);
    GLint index = glGetFragDataIndex(program,*name);
    NanReturnValue(NanNew(index));
}
//endregion

/*--------------------------------------------------------------------------------------------*/
// SHADER AND PROGRAM QUERIES
/*--------------------------------------------------------------------------------------------*/

//region SHADER QUERIES
NAN_METHOD(isShader) {
    NanScope();
    NanReturnValue(NanNew(glIsShader(args[0]->Uint32Value())));
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
            NanReturnValue(NanNew(static_cast<bool>(value)));
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
    NanReturnValue(NanNew(infoLog));
}

NAN_METHOD(getShaderSource) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint shader = args[0]->Uint32Value();
    GLint len;
    glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &len);
    GLchar *src = new GLchar[len];
    glGetShaderSource(shader, len, NULL, src);
    Local<String> str = NanNew(src);
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
    NanReturnValue(NanNew(value));
}

//getProgramiv

NAN_METHOD(isProgramPipeline){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLboolean value = glIsProgramPipeline(args[0]->Uint32Value());
    NanReturnValue(NanNew(value));
}

//GetProgramPipelineiv

NAN_METHOD(getProgramInfoLog) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint program = args[0]->Uint32Value();
    GLsizei len = 1024;
    GLchar infoLog[len];
    glGetProgramInfoLog(program, len, &len, infoLog);
    NanReturnValue(NanNew(infoLog));
}

NAN_METHOD(getProgramPipelineInfoLog){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint pipeline = args[0]->Uint32Value();
    GLsizei len = 1024;
    GLchar infoLog[len];
    glGetProgramPipelineInfoLog(pipeline,len,&len,infoLog);
    NanReturnValue(NanNew(infoLog));
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
            NanReturnValue(NanNew(static_cast<bool>(params)));
        case GL_ACTIVE_UNIFORMS:
        case GL_ACTIVE_ATTRIBUTES:
            glGetProgramiv(program,pname,&params);
            NanReturnValue(NanNew(params));
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
    Local<Array> out = NanNew<Array>(2);
    out->Set(0,NanNew(val[0]));
    out->Set(1,NanNew(val[1]));
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

NAN_METHOD(fogi){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum pname = args[0]->Uint32Value();
    GLint  param = args[1]->Int32Value();
    glFogi(pname,param);
    NanReturnUndefined();
}

NAN_METHOD(fogf){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum pname = args[0]->Uint32Value();
    GLfloat param = static_cast<GLfloat>(args[0]->Int32Value());
    glFogf(pname,param);
    NanReturnUndefined();
}

//fog{if}v

/*--------------------------------------------------------------------------------------------*/
// TEXTURING
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(activeTexture){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum texture = args[0]->Uint32Value();
    glActiveTexture(texture);
    NanReturnUndefined();
}

//region TEXTURE OBJECTS
NAN_METHOD(bindTexture){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum target = args[0]->Uint32Value();
    GLuint texture = args[1]->Uint32Value();
    glBindTexture(target,texture);
    NanReturnUndefined();
}

NAN_METHOD(createTexture){
    NanScope();
    GLuint texture = 0;
    glGenTextures(1,&texture);
    NanReturnValue(NanNew(texture));
}

NAN_METHOD(genTextures){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint num = args[0]->Uint32Value();
    GLuint textures[num];
    glGenTextures(num,textures);
    Local<Array> out = NanNew<Array>(num);
    for(uint32_t i = 0; i < num; ++i){
        out->Set(i,NanNew(textures[i]));
    }
    NanReturnValue(out);
}

NAN_METHOD(deleteTexture){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint texture = args[0]->Uint32Value();
    glDeleteTextures(1,&texture);
    NanReturnUndefined();
}

//areTexturesResident
//prioritizeTextures
//endregion

//region SAMPLER OBJECTS
NAN_METHOD(createSampler){
    NanScope();
    GLuint sampler = 0;
    glGenSamplers(1,&sampler);
    NanReturnValue(NanNew(sampler));
}

NAN_METHOD(bindSampler){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint unit = args[0]->Uint32Value();
    GLuint sampler = args[1]->Uint32Value();
    glBindSampler(unit,sampler);
    NanReturnUndefined();
}

//samplerParameter{if}v
//samplerParameterI{u ui}v

NAN_METHOD(deleteSampler){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint sampler = args[0]->Uint32Value();
    glDeleteSamplers(1,&sampler);
    NanReturnUndefined();
}
//endregion

//region TEXTURE IMAGE SPEC
NAN_METHOD(texImage3D){
    NanScope();
    CHECK_ARGS_LEN(10);
    GLenum target = args[0]->Uint32Value();
    GLint level   = args[1]->Int32Value();
    GLint internalformat = args[2]->Int32Value();
    GLsizei width = args[3]->Uint32Value();
    GLsizei height = args[4]->Uint32Value();
    GLsizei depth = args[5]->Uint32Value();
    GLint border = args[6]->Int32Value();
    GLenum format = args[7]->Uint32Value();
    GLenum type = args[8]->Uint32Value();
    const void *pixels = getImageData(args[9]->ToObject());
    glTexImage3D(target,level,internalformat,width,height,depth,border,format,type,pixels);
    NanReturnUndefined();
}

NAN_METHOD(texImage2D){
    NanScope();
    CHECK_ARGS_LEN(9);
    GLenum target = args[0]->Uint32Value();
    GLint level = args[1]->Int32Value();
    GLint internalformat = args[2]->Int32Value();
    GLsizei width = args[3]->Uint32Value();
    GLsizei height = args[4]->Uint32Value();
    GLint border = args[5]->Int32Value();
    GLenum format = args[6]->Uint32Value();
    GLenum type = args[7]->Uint32Value();
    const void *pixels = getImageData(args[8]->ToObject());
    glTexImage2D(target,level,internalformat,width,height,border,format,type,pixels);
    NanReturnUndefined();
}

NAN_METHOD(texImage1D){
    NanScope();
    CHECK_ARGS_LEN(8);
    GLenum target = args[0]->Uint32Value();
    GLint level = args[1]->Int32Value();
    GLint internalformat = args[2]->Int32Value();
    GLsizei width = args[3]->Uint32Value();
    GLint border = args[4]->Int32Value();
    GLenum format = args[5]->Uint32Value();
    GLenum type = args[6]->Uint32Value();
    const void *pixels = getImageData(args[7]->ToObject());
    glTexImage1D(target,level,internalformat,width,border,format,type,pixels);
    NanReturnUndefined();
}

//endregion

//region ALTERNATE TEXTURE IMAGE SPEC
NAN_METHOD(copyTexImage2D){
    NanScope();
    CHECK_ARGS_LEN(8);
    GLenum target = args[0]->Uint32Value();
    GLint level = args[1]->Int32Value();
    GLenum internalformat = args[2]->Uint32Value();
    GLint x = args[3]->Int32Value();
    GLint y = args[4]->Int32Value();
    GLsizei width = args[5]->Uint32Value();
    GLsizei height = args[6]->Uint32Value();
    GLint border = args[7]->Int32Value();
    glCopyTexImage2D(target,level,internalformat,x,y,width,height,border);
    NanReturnUndefined();
}

NAN_METHOD(copyTexImage1D){
    NanScope();
    CHECK_ARGS_LEN(7);
    GLenum target = args[0]->Uint32Value();
    GLint level = args[1]->Int32Value();
    GLenum internalformat = args[2]->Uint32Value();
    GLint x = args[3]->Int32Value();
    GLint y = args[4]->Int32Value();
    GLsizei width = args[5]->Uint32Value();
    GLint border = args[6]->Int32Value();
    glCopyTexImage1D(target,level,internalformat,x,y,width,border);
    NanReturnUndefined();
}

//texSubImage3D
//texSubImage2D
//texSubImage1D

NAN_METHOD(copyTexSubImage3D){
    NanScope();
    CHECK_ARGS_LEN(9);
    GLenum target = args[0]->Uint32Value();
    GLint level = args[1]->Int32Value();
    GLint xoffset = args[2]->Int32Value();
    GLint yoffset = args[3]->Int32Value();
    GLint zoffset = args[4]->Int32Value();
    GLint x = args[5]->Int32Value();
    GLint y = args[6]->Int32Value();
    GLsizei width = args[7]->Uint32Value();
    GLsizei height = args[8]->Uint32Value();
    glCopyTexSubImage3D(target,level,xoffset,yoffset,zoffset,x,y,width,height);
    NanReturnUndefined();
}

NAN_METHOD(copyTexSubImage2D){
    NanScope();
    CHECK_ARGS_LEN(8);
    GLenum target = args[0]->Uint32Value();
    GLint level = args[1]->Int32Value();
    GLint xoffset = args[2]->Int32Value();
    GLint yoffset = args[3]->Int32Value();
    GLint x = args[4]->Int32Value();
    GLint y = args[5]->Int32Value();
    GLsizei width = args[6]->Uint32Value();
    GLsizei height = args[7]->Uint32Value();
    glCopyTexSubImage2D(target,level,xoffset,yoffset,x,y,width,height);
    NanReturnUndefined();
}

NAN_METHOD(copyTexSubImage1D){
    NanScope();
    CHECK_ARGS_LEN(6);
    GLenum target = args[0]->Uint32Value();
    GLint level = args[1]->Int32Value();
    GLint xoffset = args[2]->Int32Value();
    GLint x = args[3]->Int32Value();
    GLint y = args[4]->Int32Value();
    GLsizei width = args[5]->Uint32Value();
    glCopyTexSubImage1D(target,level,xoffset,x,y,width);
    NanReturnUndefined();
}
//endregion

//region COMPRESSED TEXTURE IMAGES
//compressedTexImage3D
//compressedTexImage2D
//compressedTexImage1D
//compressedTexSubImage3D
//compressedTexSubImage2D
//compressedTexSubImage1D
//endregion

//region MULTISAMPLE TEXTURES
//texImage3DMultisample
//texImage2DMultisample
//endregion

//region BUFFER TEXTURES
//texBuffer
//endregion

//region TEXTURE PARAMETERS
NAN_METHOD(texParameteri){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
    GLint param = args[2]->Int32Value();
    glTexParameteri(target,pname,param);
    NanReturnUndefined();
}

NAN_METHOD(texParameterf){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
    GLfloat param = static_cast<GLfloat>(args[2]->NumberValue());
    glTexParameterf(target,pname,param);
    NanReturnUndefined();
}

NAN_METHOD(texParameterfv){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
    //const GLfloat *params = reinterpret_cast<const GLfloat*>(getArrayData(args[2]->ToObject()));
    const GLfloat *params = getArrayData<GLfloat>(args[2]->ToObject());
    glTexParameterfv(target,pname,params);
    NanReturnUndefined();
}

NAN_METHOD(texParameteriv){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
//    const GLint *params = reinterpret_cast<const GLint *>(getArrayData(args[2]->ToObject()));
    const GLint *params = getArrayData<GLint>(args[2]->ToObject());
    glTexParameteriv(target,pname,params);
    NanReturnUndefined();
}

NAN_METHOD(texParameterIiv){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
//    const GLint *params = reinterpret_cast<const GLint *>(getArrayData(args[2]->ToObject()));
    const GLint *params = getArrayData<GLint>(args[2]->ToObject());
    glTexParameterIiv(target,pname,params);
    NanReturnUndefined();
}

NAN_METHOD(texParameterIuiv){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
//    const GLuint *params = reinterpret_cast<const GLuint *>(getArrayData(args[2]->ToObject()));
    const GLuint *params = getArrayData<GLuint>(args[2]->ToObject());
    glTexParameterIuiv(target,pname,params);
    NanReturnUndefined();
}
//endregion

//region TEXTURE MINIFICATION
NAN_METHOD(generateMipMap){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum target = args[0]->Uint32Value();
    glGenerateMipmap(target);
    NanReturnUndefined();
}
//endregion

//region TEXTURE ENVIRONMENTS & FUNCTIONS
NAN_METHOD(texEnvi){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
    GLint param = args[2]->Int32Value();
    glTexEnvi(target,pname,param);
    NanReturnUndefined();
}

NAN_METHOD(texEnvf){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
    GLfloat param = args[2]->Uint32Value();
    glTexEnvf(target,pname,param);
    NanReturnUndefined();
}

NAN_METHOD(texEnviv){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
//    const GLint *params = reinterpret_cast<const GLint *>(getArrayData(args[2]->ToObject()));
    const GLint *params = getArrayData<GLint>(args[2]->ToObject());
    glTexEnviv(target,pname,params);
    NanReturnUndefined();
}

NAN_METHOD(texEnvfv){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum target = args[0]->Uint32Value();
    GLenum pname = args[1]->Uint32Value();
//    const GLfloat *params = reinterpret_cast<const GLfloat *>(getArrayData(args[2]->ToObject()));
    const GLfloat *params = getArrayData<GLfloat>(args[2]->ToObject());
    glTexEnvfv(target,pname,params);
    NanReturnUndefined();
}
//endregion

//region ENUMERATED QUERIES
//getTexEnv{if}v
//getTexGen{ifd}v
//getTexParameter{if}v
//getTexParameter{i ui}v
//getTexLevelParameter{if}v
//endregion

//region TEXTURE QUERIES
//getTexImage
//getCompressedTexImage
NAN_METHOD(isTexture){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint texture = args[0]->Uint32Value();
    glIsTexture(texture);
    NanReturnUndefined();
}
//endregion

//region SAMPLER QUERIES
//getSamplerParameter{if}v
//getSamplerParameterI{i ui}v
//endregion

/*--------------------------------------------------------------------------------------------*/
// PER-FRAGMENT OPERATIONS
/*--------------------------------------------------------------------------------------------*/

//region SCISSOR TEST
//scissorArrayv
//scissorIndexed
//scissorIndexedv
NAN_METHOD(scissor) {
    NanScope();
    CHECK_ARGS_LEN(4);
    GLint left = args[0]->Int32Value();
    GLint bottom = args[1]->Int32Value();
    GLsizei width = args[2]->Int32Value();
    GLsizei height = args[3]->Int32Value();
    glScissor(left,bottom,width,height);
    NanReturnUndefined();
}
//endregion

//region MULTISAMPLE FRAGMENT OPERATIONS
NAN_METHOD(sampleCoverage){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLclampf value = static_cast<float>(args[0]->NumberValue());
    GLboolean invert = static_cast<GLboolean>(args[1]->BooleanValue());
    glSampleCoverage(value,invert);
    NanReturnUndefined();
}

NAN_METHOD(sampleMaski){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint maskNumber = args[0]->Uint32Value();
    GLbitfield mask = args[1]->Uint32Value();
    glSampleMaski(maskNumber,mask);
    NanReturnUndefined();
}
//endregion

//region ALPHA TEST
NAN_METHOD(alphaFunc){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum func = args[0]->Uint32Value();
    GLclampf ref = static_cast<float>(args[1]->NumberValue());
    glAlphaFunc(func,ref);
    NanReturnUndefined();
}
//endregion

//region STENCIL TEST
NAN_METHOD(stencilFunc){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum func = args[0]->Uint32Value();
    GLint ref = args[1]->Int32Value();
    GLuint mask = args[2]->Uint32Value();
    glStencilFunc(func,ref,mask);
    NanReturnUndefined();
}

NAN_METHOD(stencilFuncSeparate){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLenum face = args[0]->Uint32Value();
    GLenum func = args[1]->Uint32Value();
    GLint ref = args[2]->Int32Value();
    GLuint mask = args[3]->Uint32Value();
    glStencilFuncSeparate(face,func,ref,mask);
    NanReturnUndefined();
}

NAN_METHOD(stencilOp){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLenum sfail = args[0]->Uint32Value();
    GLenum dpfail = args[1]->Uint32Value();
    GLenum dppass = args[2]->Uint32Value();
    glStencilOp(sfail,dpfail,dppass);
    NanReturnUndefined();
}

NAN_METHOD(stencilOpSeparate){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLenum face = args[0]->Uint32Value();
    GLenum sfail = args[1]->Uint32Value();
    GLenum dpfail = args[2]->Uint32Value();
    GLenum dppass = args[3]->Uint32Value();
    glStencilOpSeparate(face,sfail,dpfail,dppass);
    NanReturnUndefined();
}
//endregion

//region DEPTH BUFFER TEST
NAN_METHOD(depthFunc){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum func = args[0]->Uint32Value();
    glDepthFunc(func);
    NanReturnUndefined();
}
//endregion

//region OCCLUSION QUERIES
NAN_METHOD(beginQuery){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum target = args[0]->Uint32Value();
    GLuint id = args[1]->Uint32Value();
    glBeginQuery(target,id);
    NanReturnUndefined();
}

NAN_METHOD(endQuery){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum target = args[0]->Uint32Value();
    glEndQuery(target);
    NanReturnUndefined();
}
//endregion

//region BLENDING
NAN_METHOD(blendEquation){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum mode = args[0]->Uint32Value();
    glBlendEquation(mode);
    NanReturnUndefined();
}

NAN_METHOD(blendEquationi){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLuint buf = args[0]->Uint32Value();
    GLenum mode = args[1]->Uint32Value();
    glBlendEquationi(buf,mode);
    NanReturnUndefined();
}

NAN_METHOD(blendEquationSeparate){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum modeRGB = args[0]->Uint32Value();
    GLenum modeAlpha = args[1]->Uint32Value();
    glBlendEquationSeparate(modeRGB,modeAlpha);
    NanReturnUndefined();
}

NAN_METHOD(blendEquationSeparatei){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLuint buf = args[0]->Uint32Value();
    GLenum modeRGB = args[1]->Uint32Value();
    GLenum modeAlpha = args[2]->Uint32Value();
    glBlendEquationSeparatei(buf,modeRGB,modeAlpha);
    NanReturnUndefined();
}

NAN_METHOD(blendFunc){
    NanScope();
    CHECK_ARGS_LEN(2);
    GLenum src = args[0]->Uint32Value();
    GLenum dst = args[1]->Uint32Value();
    glBlendFunc(src,dst);
    NanReturnUndefined();
}

NAN_METHOD(blendFunci){
    NanScope();
    CHECK_ARGS_LEN(3);
    GLuint buf = args[0]->Uint32Value();
    GLenum src = args[1]->Uint32Value();
    GLenum dst = args[2]->Uint32Value();
    glBlendFunci(buf,src,dst);
    NanReturnUndefined();
}

NAN_METHOD(blendFuncSeparate){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLenum srcRGB = args[0]->Uint32Value();
    GLenum dstRGB = args[1]->Uint32Value();
    GLenum srcAlpha = args[2]->Uint32Value();
    GLenum dstAlpha = args[3]->Uint32Value();
    glBlendFuncSeparate(srcRGB,dstRGB,srcAlpha,dstAlpha);
    NanReturnUndefined();
}

NAN_METHOD(blendFuncSeparatei){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLuint buf = args[0]->Uint32Value();
    GLenum srcRGB = args[1]->Uint32Value();
    GLenum dstRGB = args[2]->Uint32Value();
    GLenum srcAlpha = args[3]->Uint32Value();
    GLenum dstAlpha = args[4]->Uint32Value();
    glBlendFuncSeparatei(buf,srcRGB,dstRGB,srcAlpha,dstAlpha);
    NanReturnUndefined();
}

NAN_METHOD(blendColor){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLclampf red = static_cast<GLclampf>(args[0]->NumberValue());
    GLclampf green = static_cast<GLclampf>(args[1]->NumberValue());
    GLclampf blue = static_cast<GLclampf>(args[2]->NumberValue());
    GLclampf alpha = static_cast<GLclampf>(args[3]->NumberValue());
    glBlendColor(red,green,blue,alpha);
    NanReturnUndefined();
}
//endregion

//region LOGICAL OPERATION
NAN_METHOD(logicalOp){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum op = args[0]->Uint32Value();
    glLogicOp(op);
    NanReturnUndefined();
}
//endregion

/*--------------------------------------------------------------------------------------------*/
// UNIFORMS AND ATTRIBUTES
/*--------------------------------------------------------------------------------------------*/


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
    NanReturnValue(NanNew(buffer));
}

NAN_METHOD(genBuffers){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint num = args[0]->Uint32Value();
    GLuint buffers[num];
    glGenBuffers(num,buffers);
    Local<Array> out = NanNew<Array>(num);
    for(uint32_t i = 0; i < num; ++i){
        out->Set(i,NanNew(buffers[i]));
    }
    NanReturnValue(out);
}

NAN_METHOD(bindBuffer) {
    NanScope();
    CHECK_ARGS_LEN(2);
    glBindBuffer(args[0]->Uint32Value(), args[1]->Uint32Value());
    NanReturnUndefined();
}

NAN_METHOD(bindBufferRange){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLenum target   = args[0]->Uint32Value();
    GLuint index    = args[1]->Uint32Value();
    GLuint buffer   = args[2]->Uint32Value();
    GLintptr offset = args[3]->Uint32Value();
    GLsizeiptr size = args[4]->Uint32Value();
    glBindBufferRange(target,index,buffer,offset,size);
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
                size = 1;
                size = obj->GetIndexedPropertiesExternalArrayDataLength() * size;
                glBufferData(target, size, static_cast<const GLbyte*>(obj->GetIndexedPropertiesExternalArrayData()), usage);
                break;
            case kExternalShortArray:
            case kExternalUnsignedShortArray:
                size = 2;
                size = obj->GetIndexedPropertiesExternalArrayDataLength() * size;
                glBufferData(target, size, static_cast<const GLshort *>(obj->GetIndexedPropertiesExternalArrayData()), usage);
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
    CHECK_ARGS_LEN(4);
    GLenum target = args[0]->Uint32Value();
    GLintptr offset = args[1]->Uint32Value();
    Local<Object> obj = Local<Object>::Cast(args[2]);

    int elementSize = ELEMENT_SIZES_FOR_ARRAY_TYPES[obj->GetIndexedPropertiesExternalArrayDataType()];
    GLsizeiptr size = obj->GetIndexedPropertiesExternalArrayDataLength() * elementSize;
    void *data = obj->GetIndexedPropertiesExternalArrayData();

    glBufferSubData(target, offset, size, data);
    NanReturnUndefined();
}

NAN_METHOD(mapBuffer){
    NanScope();
    CHECK_ARGS_LEN(4);
    GLenum target = args[0]->Uint32Value();
    GLenum access = args[1]->Uint32Value();
    GLenum type   = args[2]->Uint32Value();
    int numberOfElements = args[3]->Int32Value();

    Local<Object> global = NanGetCurrentContext()->Global();
    Local<Value> val     = global->Get(NanNew("Float32Array"));
    Local<Function> f32c = val.As<Function>();

    Local<Value> size = NanNew(numberOfElements);
    Local<Object> array = f32c->NewInstance(1,&size);

    void *data = glMapBuffer(target,access);

    switch (type) {
        case GL_FLOAT :
            array->SetIndexedPropertiesToExternalArrayData(
                    reinterpret_cast<GLfloat *>(data),
                    kExternalFloatArray,
                    numberOfElements
            );
            break;
        case GL_UNSIGNED_SHORT:
            array->SetIndexedPropertiesToExternalArrayData(
                    reinterpret_cast<GLshort *>(data),
                    kExternalShortArray,
                    numberOfElements
            );
            break;
        case GL_UNSIGNED_INT:
            array->SetIndexedPropertiesToExternalArrayData(
                    reinterpret_cast<GLint *>(data),
                    kExternalUnsignedIntArray,
                    numberOfElements
            );
            break;
        default:
            NanThrowTypeError("Wrong data type");
            break;
    }

    NanReturnValue(array);
}

NAN_METHOD(mapBufferRange){
    NanScope();
    CHECK_ARGS_LEN(5);
    GLenum target     = args[0]->Uint32Value();
    GLintptr offset   = args[1]->Uint32Value();
    GLint length      = args[2]->Uint32Value();
    GLbitfield access = args[3]->Uint32Value();
    GLenum type       = args[4]->Uint32Value();

    Local<Object> global = NanGetCurrentContext()->Global();
    Local<Value> val     = global->Get(NanNew("Float32Array"));
    Local<Function> f32c = val.As<Function>();

    Local<Value> size = NanNew(length);
    Local<Object> array = f32c->NewInstance(1,&size);

    void *data = glMapBufferRange(target,offset,length,access);

    switch (type){
        case GL_FLOAT:
            array->SetIndexedPropertiesToExternalArrayData(
                    reinterpret_cast<GLfloat *>(data),
                    kExternalFloatArray,
                    length
            );
            break;
        case GL_UNSIGNED_SHORT:
            array->SetIndexedPropertiesToExternalArrayData(
                    reinterpret_cast<GLshort *>(data),
                    kExternalShortArray,
                    length
            );
            break;
        case GL_UNSIGNED_INT:
            array->SetIndexedPropertiesToExternalArrayData(
                    reinterpret_cast<GLint *>(data),
                    kExternalUnsignedIntArray,
                    length
            );
            break;
        default:
            NanThrowError("Wrong data type");
            break;
    }

    NanReturnValue(array);
}

NAN_METHOD(unmapBuffer){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLenum target = args[0]->Uint32Value();
    GLboolean success = glUnmapBuffer(target);
    NanReturnValue(NanNew(success));
}

NAN_METHOD(deleteBuffer) {
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint buffer = args[0]->Uint32Value();
    glDeleteBuffers(1, &buffer);
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
            NanReturnValue(NanNew(params));
        default:
            break;
    }
    NanReturnUndefined();
}

NAN_METHOD(isBuffer) {
    NanScope();
    CHECK_ARGS_LEN(1);
    NanReturnValue(NanNew(glIsBuffer(args[0]->Uint32Value())));
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
    NanReturnValue(NanNew(vao));
}

NAN_METHOD(genVertexArrays){
    NanScope();
    CHECK_ARGS_LEN(1);
    GLuint num = args[0]->Uint32Value();
    GLuint arrays[num];
    glGenVertexArrays(num,arrays);
    Local<Array> out = NanNew<Array>(num);
    for(uint32_t i = 0; i < num; ++i){
        out->Set(i,NanNew(arrays[i]));
    }
    NanReturnValue(out);
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
    NanReturnValue(NanNew(value));
}

/*--------------------------------------------------------------------------------------------*/
// SPECIAL
/*--------------------------------------------------------------------------------------------*/

NAN_METHOD(readImageData){
    NanScope();
    CHECK_ARGS_LEN(1);
    char *filename = *String::Utf8Value(args[0]->ToString());

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename,0);
    FIBITMAP *img = FreeImage_Load(format, filename, 0);

    if(!img){
        NanThrowError("Image path not valid");
    }

    int width       = FreeImage_GetWidth(img);
    int height      = FreeImage_GetHeight(img);
    int numPixels   = width * height;
    BYTE *bits   = FreeImage_GetBits(img);
    GLenum glformat = 0;

    FREE_IMAGE_TYPE type = FreeImage_GetImageType(img);

    Local<Object> global  = NanGetCurrentContext()->Global();
    Local<Value> val      = global->Get(NanNew("Uint8ClampedArray"));
    Local<Function> arr_c = val.As<Function>();

    int dataSize;
    Local<Value> size;
    Local<Object> array;

    switch (type){
        case FIT_BITMAP:
            switch (FreeImage_GetBPP(img)){
                case 8: {
                    size  = NanNew(numPixels);
                    array = arr_c->NewInstance(1,&size);
                    array->SetIndexedPropertiesToPixelData(bits,numPixels);
                    glformat = GL_LUMINANCE;
                    break;
                }
                case 16: {
                    FIBITMAP *tmp_ = FreeImage_ConvertTo24Bits(img);
                    FreeImage_Unload(img);
                    img = tmp_;
                    bits = FreeImage_GetBits(img);
                }
                case 24: {
                    dataSize = numPixels * 3;

                    uint8_t *data = new uint8_t[dataSize];
                    RGBTRIPLE *pix = (RGBTRIPLE *) bits;
                    for (int i = 0, j = 0; i < dataSize; i += 3, ++j) {
                        data[i    ] = pix[j].rgbtRed;
                        data[i + 1] = pix[j].rgbtGreen;
                        data[i + 2] = pix[j].rgbtBlue;
                    }

                    size = NanNew(dataSize);
                    array = arr_c->NewInstance(1, &size);
                    array->SetIndexedPropertiesToPixelData(data,dataSize);
                    glformat = GL_RGB;

                    delete [] data;
                    break;
                }
                case 32: {
                    dataSize = numPixels * 4;

                    uint8_t *data = new uint8_t[dataSize];
                    RGBQUAD *pix = (RGBQUAD *) bits;
                    for (int i = 0, j = 0; i < dataSize; i += 4, ++j) {
                        data[i    ] = pix[j].rgbRed;
                        data[i + 1] = pix[j].rgbGreen;
                        data[i + 2] = pix[j].rgbBlue;
                        data[i + 3] = pix[j].rgbReserved;
                    }

                    size = NanNew(dataSize);
                    array = arr_c->NewInstance(1, &size);
                    array->SetIndexedPropertiesToPixelData(data, dataSize);
                    glformat = GL_RGBA;

                    delete [] data;
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        case FIT_UINT16:
            NanThrowError("FIT_UINT16 Format currently not supported");
            break;
        case FIT_FLOAT:
            NanThrowError("FIT_FLOAT Format currently not supported");
            break;
        default:
            break;
    }

    FreeImage_Unload(img);

    Local<Object> out = NanNew<Object>();
    out->Set(NanNew("width"),NanNew(width));
    out->Set(NanNew("height"),NanNew(height));
    out->Set(NanNew("glformat"),NanNew(glformat));
    out->Set(NanNew("data"),array);

    NanReturnValue(out);
}

NAN_METHOD(writeImage){
    NanScope();
    CHECK_ARGS_LEN(5);
    if(args[0]->IsUndefined()){
        NanThrowError("No path supplied.");
    }

    char* path = *String::Utf8Value(args[0]);

    GLuint x = args[1]->Uint32Value();
    GLuint y = args[2]->Uint32Value();
    GLuint w = args[3]->Uint32Value();
    GLuint h = args[4]->Uint32Value();

    GLuint w_ = w - x;
    GLuint h_ = h - y;

    BYTE *pixels = new BYTE[w_ * h_ * 3];
    glReadPixels(x,y,w,h,GL_RGB,GL_UNSIGNED_BYTE,pixels);

    FIBITMAP *image = FreeImage_ConvertFromRawBits(pixels,w_,h_,3 * w_, 24, 0x0000FF, 0xFF0000, 0x00FF00,false);
    FreeImage_Save(FIF_PNG, image, path, 0);

    FreeImage_Unload(image);
    delete [] pixels;

    NanReturnUndefined();
}

/*--------------------------------------------------------------------------------------------*/
// EXPORT
/*--------------------------------------------------------------------------------------------*/

void gl::init(Handle<Object> exports) {
    /*----------------------------------------------------------------------------------------*/
    // OPENGL ERRORS
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(getError);

    /*----------------------------------------------------------------------------------------*/
    // VERTEX ARRAY
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(vertexPointer);
    EXPORT_SET_METHOD(normalPointer);
    EXPORT_SET_METHOD(colorPointer);
    EXPORT_SET_METHOD(secondaryColorPointer);
    EXPORT_SET_METHOD(indexPointer);
    EXPORT_SET_METHOD(edgeFlagPointer);
    EXPORT_SET_METHOD(fogCoordPointer);
    EXPORT_SET_METHOD(texCoordPointer);
    EXPORT_SET_METHOD(vertexAttribPointer);
    EXPORT_SET_METHOD(vertexAttribIPointer);
    EXPORT_SET_METHOD(vertexAttribLPointer);
    EXPORT_SET_METHOD(enableClientState);
    EXPORT_SET_METHOD(disableClientState);
    EXPORT_SET_METHOD(vertexAttribDivisor);
    EXPORT_SET_METHOD(clientActiveTexture);
    EXPORT_SET_METHOD(arrayElement);

    //region DRAWING COMMANDS
    EXPORT_SET_METHOD(drawArrays);
    EXPORT_SET_METHOD(drawArraysInstanced);
    //drawArraysIndirect
    //multiDrawArrays
    EXPORT_SET_METHOD(drawElements);
    EXPORT_SET_METHOD(drawElementsInstanced);
    //multiDrawElements
    EXPORT_SET_METHOD(drawRangeElements);
    //drawElementsBaseVertex
    //drawRangeElementsBaseVertex
    //drawElementsInstancedBaseVertex
    //drawElementsIndirect
    //multiDrawElementsBaseVertex
    //endregion

    /*----------------------------------------------------------------------------------------*/
    // PER FRAGMENT OPERATIONS
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(depthFunc);

    /*----------------------------------------------------------------------------------------*/
    // SPECIAL FUNCTIONS
    /*----------------------------------------------------------------------------------------*/

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

    /*----------------------------------------------------------------------------------------*/
    // VIEWPORT & CLIPPING
    /*----------------------------------------------------------------------------------------*/

    //region CONTROLLING VIEWPORT
    //depthRangeArrayv
    EXPORT_SET_METHOD(depthRangeIndexed);
    EXPORT_SET_METHOD(depthRange);
    //viewportArrayv
    EXPORT_SET_METHOD(viewportIndexedf);
    //viewportIndexedfv
    EXPORT_SET_METHOD(viewport);
    //endregion

    //region CLIPPING
    //clipPlane
    //getClipPlane
    //endregion

    /*----------------------------------------------------------------------------------------*/
    // PROGRAMS AND SHADERS
    /*----------------------------------------------------------------------------------------*/

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
    EXPORT_SET_METHOD(getActiveAttrib);
    EXPORT_SET_METHOD(bindAttribLocation);
    //endregion

    //region UNIFORM VARIABLES
    EXPORT_SET_METHOD(getUniformLocation);
    EXPORT_SET_METHOD(getUniformBlockIndex);
    //getActiveUniformBlockName
    //getActiveUniformBlockiv
    //getUniformIndices
    //getActiveUniformName
    EXPORT_SET_METHOD(getActiveUniform);
    //getActiveUniformsiv
    EXPORT_SET_METHOD(uniform1f);
    EXPORT_SET_METHOD(uniform2f);
    EXPORT_SET_METHOD(uniform3f);
    EXPORT_SET_METHOD(uniform4f);
    EXPORT_SET_METHOD(uniform1i);
    EXPORT_SET_METHOD(uniform2i);
    EXPORT_SET_METHOD(uniform3i);
    EXPORT_SET_METHOD(uniform4i);
    EXPORT_SET_METHOD(uniform1fv);
    EXPORT_SET_METHOD(uniform2fv);
    EXPORT_SET_METHOD(uniform3fv);
    EXPORT_SET_METHOD(uniform4fv);
    EXPORT_SET_METHOD(uniform1iv);
    EXPORT_SET_METHOD(uniform2iv);
    EXPORT_SET_METHOD(uniform3iv);
    EXPORT_SET_METHOD(uniform4iv);
    EXPORT_SET_METHOD(uniformMatrix2fv);
    EXPORT_SET_METHOD(uniformMatrix3fv);
    EXPORT_SET_METHOD(uniformMatrix4fv);
    //endregion

    //region UNIFORM BUFFER OBJECTS BINDINGS
    EXPORT_SET_METHOD(UniformBlockBinding);
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

    EXPORT_SET_METHOD(fogi);
    EXPORT_SET_METHOD(fogf);
    //fog{if}v

    /*----------------------------------------------------------------------------------------*/
    // TEXTURING
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(activeTexture);

    //region TEXTURE OBJECTS
    EXPORT_SET_METHOD(bindTexture);
    EXPORT_SET_METHOD(createTexture);
    EXPORT_SET_METHOD(genTextures);
    EXPORT_SET_METHOD(deleteTexture);
    //areTexturesResident
    //prioritizeTextures
    //endregion

    //region SAMPLER OBJECTS
    EXPORT_SET_METHOD(createSampler);
    EXPORT_SET_METHOD(bindSampler);
    //samplerParameter{if}v
    //samplerParameterI{u ui}v
    EXPORT_SET_METHOD(deleteSampler);
    //endregion

    //region TEXTURE IMAGE SPEC
    EXPORT_SET_METHOD(texImage3D);
    EXPORT_SET_METHOD(texImage2D);
    EXPORT_SET_METHOD(texImage1D);
    //endregion

    //region ALTERNATE TEXTURE IMAGE SPEC
    EXPORT_SET_METHOD(copyTexImage2D);
    EXPORT_SET_METHOD(copyTexImage1D);
    //texSubImage3D
    //texSubImage2D
    //texSubImage1D
    EXPORT_SET_METHOD(copyTexSubImage3D);
    EXPORT_SET_METHOD(copyTexSubImage2D);
    EXPORT_SET_METHOD(copyTexSubImage1D);
    //endregion

    //region COMPRESSED TEXTURE IMAGES
    //compressedTexImage3D
    //compressedTexImage2D
    //compressedTexImage1D
    //compressedTexSubImage3D
    //compressedTexSubImage2D
    //compressedTexSubImage1D
    //endregion

    //region MULTISAMPLE TEXTURES
    //texImage3DMultisample
    //texImage2DMultisample
    //endregion

    //region BUFFER TEXTURES
    //texBuffer
    //endregion

    //region TEXTURE PARAMETERS
    EXPORT_SET_METHOD(texParameteri);
    EXPORT_SET_METHOD(texParameterf);
    EXPORT_SET_METHOD(texParameterfv);
    EXPORT_SET_METHOD(texParameteriv);
    EXPORT_SET_METHOD(texParameterIiv);
    EXPORT_SET_METHOD(texParameterIuiv);
    //endregion

    //region TEXTURE MINIFICATION
    EXPORT_SET_METHOD(generateMipMap);
    //endregion

    //region TEXTURE ENVIRONMENTS & FUNCTIONS
    EXPORT_SET_METHOD(texEnvi);
    EXPORT_SET_METHOD(texEnvf);
    EXPORT_SET_METHOD(texEnviv);
    EXPORT_SET_METHOD(texEnvfv);
    //endregion

    //region TEXTURE QUERIES
    //getTexImage
    //getCompressedTexImage
    EXPORT_SET_METHOD(isTexture);
    //endregion

    //region SAMPLER QUERIES
    //getSamplerParameter{if}v
    //getSamplerParameterI{i ui}v
    //endregion

    /*----------------------------------------------------------------------------------------*/
    // PER-FRAGMENT OPERATIONS
    /*----------------------------------------------------------------------------------------*/

    //region SCISSOR TEST
    //scissorArrayv
    //scissorIndexed
    //scissorIndexedv
    EXPORT_SET_METHOD(scissor);
    //endregion

    //region MULTISAMPLE FRAGMENT OPERATIONS
    EXPORT_SET_METHOD(sampleCoverage);
    EXPORT_SET_METHOD(sampleMaski);
    //endregion

    //region ALPHA
    EXPORT_SET_METHOD(alphaFunc);
    EXPORT_SET_METHOD(stencilFunc);
    EXPORT_SET_METHOD(stencilFuncSeparate);
    EXPORT_SET_METHOD(stencilOp);
    EXPORT_SET_METHOD(stencilOpSeparate);
    //endregion

    //region DEPTH BUFFER TEST
    EXPORT_SET_METHOD(depthFunc);
    //endregion

    //region OCCLUSION QUERIES
    EXPORT_SET_METHOD(beginQuery);
    EXPORT_SET_METHOD(endQuery);
    //endregion

    //region BLENDING
    EXPORT_SET_METHOD(blendEquation);
    EXPORT_SET_METHOD(blendEquationi);
    EXPORT_SET_METHOD(blendEquationSeparate);
    EXPORT_SET_METHOD(blendEquationSeparatei);
    EXPORT_SET_METHOD(blendFunc);
    EXPORT_SET_METHOD(blendFunci);
    EXPORT_SET_METHOD(blendFuncSeparate);
    EXPORT_SET_METHOD(blendFuncSeparatei);
    EXPORT_SET_METHOD(blendColor);
    //endregion

    //region LOGICAL OPERATION
    EXPORT_SET_METHOD(logicalOp);
    //endregion



    /*----------------------------------------------------------------------------------------*/
    // UNIFORMS AND ATTRIBUTES
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(enableVertexAttribArray);
    EXPORT_SET_METHOD(disableVertexAttribArray);





    EXPORT_SET_METHOD(vertexAttribPointer);

    /*----------------------------------------------------------------------------------------*/
    // BUFFER
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(createBuffer);
    EXPORT_SET_METHOD(genBuffers);
    EXPORT_SET_METHOD(bindBuffer);
    EXPORT_SET_METHOD(bindBufferRange);
    EXPORT_SET_METHOD(bufferData);
    EXPORT_SET_METHOD(bufferSubData);
    EXPORT_SET_METHOD(mapBuffer);
    EXPORT_SET_METHOD(mapBufferRange);
    EXPORT_SET_METHOD(unmapBuffer);
    EXPORT_SET_METHOD(deleteBuffer);
    EXPORT_SET_METHOD(getBufferParameter);
    EXPORT_SET_METHOD(isBuffer);

    /*----------------------------------------------------------------------------------------*/
    // VERTEX ARRAY OBJECTS
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(createVertexArray);
    EXPORT_SET_METHOD(genVertexArrays);
    EXPORT_SET_METHOD(deleteVertexArray);
    EXPORT_SET_METHOD(bindVertexArray);
    EXPORT_SET_METHOD(isVertexArray);

    /*----------------------------------------------------------------------------------------*/
    // SPECIAL
    /*----------------------------------------------------------------------------------------*/

    EXPORT_SET_METHOD(readImageData);
    EXPORT_SET_METHOD(writeImage);

    /*----------------------------------------------------------------------------------------*/
    // CONSTANTS
    /*----------------------------------------------------------------------------------------*/

    glConsts::init(exports);
}
