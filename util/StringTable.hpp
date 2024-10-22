#ifndef Hpp_StringTable
#define Hpp_StringTable

#include "PyString.hpp"
#include "OopClosure.hpp"

class StringTable {
    friend class Universe;
private:
    static void initialize();
public:

    static void oops_do(OopClosure* closure);

    static PyString* str_len;
    static PyString* str_false;
    static PyString* str_true;
    static PyString* str_none;
    static PyString* str_next;
    static PyString* str_list;
    static PyString* str_type;
    static PyString* str_typeof;
    static PyString* str_str;
    static PyString* str_builtin_func;
    static PyString* str_method;
    static PyString* str_int;
    static PyString* str_function;
    static PyString* str_dict;
    static PyString* str_isinstance;
    static PyString* str_object;
    static PyString* str_mod;
    static PyString* str_init;
    static PyString* str_bool;
    static PyString* str_NoneType;
    static PyString* str_float;
    static PyString* str_mro;
    static PyString* str_class;
    static PyString* str_super;
    static PyString* str_getattr;
    static PyString* str_setattr;
    static PyString* str_dict_values;
    static PyString* str_dict_keys;
    static PyString* str_dict_items;
    static PyString* str_range;
};

#endif