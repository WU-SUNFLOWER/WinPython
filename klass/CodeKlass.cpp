#include "CodeKlass.hpp"
#include "CodeObject.hpp"

CodeKlass* CodeKlass::instance = nullptr;

CodeKlass::CodeKlass() {

}

void CodeKlass::oops_do(OopClosure* closure, PyObject* object) {

}

size_t CodeKlass::getSize() {
    return sizeof(CodeObject);
}