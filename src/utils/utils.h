#ifndef UTILS_H_
#define UTILS_H_

#include <nan.h>
#include <node.h>

#define V8_STR(...) NanSymbol(__VA_ARGS__)
#define V8_INT(val) v8::Integer::New(val)
#define V8_NUM(val) v8::Number::New(val)
#define V8_BOOL(val) v8::Boolean::New(val)
#define V8_RETHROW(tc) v8::Local<v8::Value>::New(tc.Exception());

#endif