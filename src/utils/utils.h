#ifndef UTILS_H_
#define UTILS_H_

#include <nan.h>
#include <node.h>

#define V8_STR(...) NanSymbol(__VA_ARGS__)
#define V8_INT(val) v8::Integer::New(val)
#define V8_NUM(val) v8::Number::New(val)
#define V8_BOOL(val) v8::Boolean::New(val)
#define V8_RETHROW(tc) v8::Local<v8::Value>::New(tc.Exception());

#define EXPORT_SET_CONST(name, constant) \
    exports->Set( \
        V8_STR(name), \
        V8_NUM(constant), \
        static_cast<v8::PropertyAttribute>( \
        v8::ReadOnly|v8::DontDelete))

#define EXPORT_SET_METHOD(method) NODE_SET_METHOD(exports, #method, method)

#define CHECK_ARGS_LEN(argsLen) \
    if(args.Length() != argsLen){ \
        stringstream s; \
        s << __FUNCTION__ << ": " << "Arguments length should be " << argsLen << "."; \
        NanThrowError(s.str().c_str()); \
    }

#define CHECK_ARRAY_LENGTH(arrLen,arrLenExp) \
    if(arrLen != arrLenExp){ \
        stringstream s; \
        s << __FUNCTION__ << ": " << "Array length should be " << arrLenExp << "."; \
        NanThrowError(s.str().c_str()); \
    }


#define CHECK_ARGS_LEN_AT_LEAST(argsLen) \
    if(args.Length() != argsLen) { \
        stringstream s; \
        s << __FUNCTION__ << ": " << "Arguments length should at least be " << argsLen << "."; \
        NanThrowError(s.str().c_str()) \
}

#endif