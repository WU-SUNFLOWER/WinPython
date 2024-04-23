#ifndef Hpp_StringTable
#define Hpp_StringTable

#include "PyString.hpp"

class StringTable {
public:
    static PyString* str_len;
    static PyString* str_false;
    static PyString* str_true;
    static PyString* str_none;
    static PyString* str_next;
    static PyString* str_list;
    static PyString* str_type;
    static PyString* str_str;
    static PyString* str_builtin_func;
    static PyString* str_method;
    static PyString* str_int;
    static PyString* str_function;
    static PyString* str_dict;
    static PyString* str_isinstance;
    static PyString* str_object;
    static PyString* str_mod;
};

#endif