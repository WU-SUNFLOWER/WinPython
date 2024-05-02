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

void StringTable::oops_do(OopClosure* closure) {

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
}