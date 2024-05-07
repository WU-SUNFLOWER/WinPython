#include "CodeKlass.hpp"
#include "CodeObject.hpp"

CodeKlass* CodeKlass::instance = nullptr;

CodeKlass::CodeKlass() {

}

void CodeKlass::oops_do(OopClosure* closure, PyObject* object) {
    checkLegalPyObject(object, this);
    CodeObject* code = static_cast<CodeObject*>(object);
    //closure->do_oop(reinterpret_cast<PyObject**>(&code->_byteCodes));
    closure->do_oop(reinterpret_cast<PyObject**>(&code->_consts));
    closure->do_oop(reinterpret_cast<PyObject**>(&code->_names));
    closure->do_oop(reinterpret_cast<PyObject**>(&code->_varNames));
    closure->do_oop(reinterpret_cast<PyObject**>(&code->_freeVars));
    assert(code->_freeVars->_container != nullptr);
    closure->do_oop(reinterpret_cast<PyObject**>(&code->_cellVars));
    //closure->do_oop(reinterpret_cast<PyObject**>(&code->_name));
    //closure->do_oop(reinterpret_cast<PyObject**>(&code->_fileName));
    //closure->do_oop(reinterpret_cast<PyObject**>(&code->_notable));
}

size_t CodeKlass::getSize() {
    return sizeof(CodeObject);
}