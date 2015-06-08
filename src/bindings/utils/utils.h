#ifndef UTILS_H_
#define UTILS_H_

#include <nan.h>
#include <node.h>
#include <sstream>

#define EXPORT_SET_CONST(name, constant) \
    exports->Set( \
        NanNew(name), \
        NanNew<Number>(constant))

#define EXPORT_SET_METHOD(method) NODE_SET_METHOD(exports, #method, method)

#define CHECK_ARGS_LEN(argsLen) \
    if(args.Length() != argsLen){ \
        std::stringstream s; \
        s << __FUNCTION__ << ": " << "Arguments length should be " << argsLen << "."; \
        NanThrowError(s.str().c_str()); \
    }

#define CHECK_ARGS_LEN_OPT_2(argsLen0,argsLen1) \
    { \
        int len = args.Length(); \
        if(len != argsLen0 || len != argsLen0){ \
            std::stringstream s; \
            s << __FUNCTION__ << ": " << "Arguments length should be " << argsLen0 << " or " << argsLen1 << "."; \
            NanThrowError(s.str().c_str()); \
        } \
    } \

#define CHECK_ARRAY_LENGTH(arrLen,arrLenExp) \
    if(arrLen != arrLenExp){ \
        std::stringstream s; \
        s << __FUNCTION__ << ": " << "Array length should be " << arrLenExp << "."; \
        NanThrowError(s.str().c_str()); \
    }


#define CHECK_ARGS_LEN_AT_LEAST(argsLen) \
    if(args.Length() != argsLen) { \
        std::stringstream s; \
        s << __FUNCTION__ << ": " << "Arguments length should at least be " << argsLen << "."; \
        NanThrowError(s.str().c_str()) \
}

#endif