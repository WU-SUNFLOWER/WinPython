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
    str_len = PyString::createString("len", true);
    str_true = PyString::createString("True", true);
    str_false = PyString::createString("False", true);
    str_none = PyString::createString("None", true);
    str_next = PyString::createString("__next__", true);
    str_mod = PyString::createString("__module__", true);
    str_init = PyString::createString("__init__", true);
    str_list = PyString::createString("list", true);
    str_type = PyString::createString("type", true);
    str_typeof = PyString::createString("typeof", true);
    str_str = PyString::createString("str", true);
    str_builtin_func = PyString::createString("builtin_function_or_method", true);
    str_method = PyString::createString("method", true);
    str_int = PyString::createString("int", true);
    str_function = PyString::createString("function", true);
    str_dict = PyString::createString("dict", true);
    str_object = PyString::createString("object", true);
    str_isinstance = PyString::createString("isinstance", true);
}