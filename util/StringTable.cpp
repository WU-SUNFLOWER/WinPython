#include "StringTable.hpp"

PyString* StringTable::str_len = nullptr;
PyString* StringTable::str_true = nullptr;
PyString* StringTable::str_false = nullptr;
PyString* StringTable::str_none = nullptr;
PyString* StringTable::str_next = nullptr;
PyString* StringTable::str_mod = nullptr;
PyString* StringTable::str_init = nullptr;
PyString* StringTable::str_list = nullptr;
PyString* StringTable::str_type = nullptr;
PyString* StringTable::str_str = nullptr;
PyString* StringTable::str_builtin_func = nullptr;
PyString* StringTable::str_method = nullptr;
PyString* StringTable::str_int = nullptr;
PyString* StringTable::str_function = nullptr;
PyString* StringTable::str_dict = nullptr;
PyString* StringTable::str_object = nullptr;
PyString* StringTable::str_isinstance = nullptr;
PyString* StringTable::str_typeof = nullptr;

void StringTable::initialize() {
    str_len = PyString::createString("len", false);
    str_true = PyString::createString("True", false);
    str_false = PyString::createString("False", false);
    str_none = PyString::createString("None", false);
    str_next = PyString::createString("__next__", false);
    str_mod = PyString::createString("__module__", false);
    str_init = PyString::createString("__init__", false);
    str_list = PyString::createString("list", false);
    str_type = PyString::createString("type", false);
    str_typeof = PyString::createString("typeof", false);
    str_str = PyString::createString("str", false);
    str_builtin_func = PyString::createString("builtin_function_or_method", false);
    str_method = PyString::createString("method", false);
    str_int = PyString::createString("int", false);
    str_function = PyString::createString("function", false);
    str_dict = PyString::createString("dict", false);
    str_object = PyString::createString("object", false);
    str_isinstance = PyString::createString("isinstance", false);
}

void StringTable::oops_do(OopClosure* closure) {
    /*
#define do_oop(x) closure->do_oop(reinterpret_cast<PyObject**>(&x));

    do_oop(str_len);
    do_oop(str_false);
    do_oop(str_true);
    do_oop(str_none);
    do_oop(str_next);
    do_oop(str_list);
    do_oop(str_type);
    do_oop(str_typeof);
    do_oop(str_str);
    do_oop(str_builtin_func);
    do_oop(str_method);
    do_oop(str_int);
    do_oop(str_function);
    do_oop(str_dict);
    do_oop(str_isinstance);
    do_oop(str_object);
    do_oop(str_mod);
    do_oop(str_init);    
    */

}