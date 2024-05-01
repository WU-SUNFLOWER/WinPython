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
    str_len = new PyString("len");
    str_true = new PyString("True");
    str_false = new PyString("False");
    str_none = new PyString("None");
    str_next = new PyString("__next__");
    str_mod = new PyString("__module__");
    str_init = new PyString("__init__");
    str_list = new PyString("list");
    str_type = new PyString("type");
    str_typeof = new PyString("typeof");
    str_str = new PyString("str");
    str_builtin_func = new PyString("builtin_function_or_method");
    str_method = new PyString("method");
    str_int = new PyString("int");
    str_function = new PyString("function");
    str_dict = new PyString("dict");
    str_object = new PyString("object");
    str_isinstance = new PyString("isinstance");
}