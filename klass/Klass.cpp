#include "Klass.hpp"
#include "PyObject.hpp"
#include "DictKlass.hpp"
#include "ListKlass.hpp"
#include "StringKlass.hpp"
#include "PyDict.hpp"
#include "PyList.hpp"
#include "PyString.hpp"
#include <cstdio>
#include <cstdlib>
#include "PyTypeObject.hpp"
#include "ObjectKlass.hpp"
#include "TypeKlass.hpp"
#include "Universe.hpp"
#include "StringTable.hpp"
#include "interpreter.hpp"
#include "FunctionKlass.hpp"
#include "PyMethod.hpp"
#include "NativeFunctionKlass.hpp"

Klass::Klass() {
    Universe::PyKlasses->push(this);
}

size_t Klass::getSize() {
    return sizeof(PyObject);
}

PyObject* Klass::createKlass(PyObject* dict, PyObject* supers, PyObject* name) {
    checkLegalPyObject(dict, DictKlass::getInstance());
    checkLegalPyObject(supers, ListKlass::getInstance());
    checkLegalPyObject(name, StringKlass::getInstance());
    
    PyDict* _dict = static_cast<PyDict*>(dict);
    PyList* _supers = static_cast<PyList*>(supers);
    PyString* _name = static_cast<PyString*>(name);

    Klass* newKlass = new Klass();
    newKlass->setKlassDict(_dict);
    newKlass->setName(_name);
    if (_supers->getLength() > 0) {
        PyTypeObject* super = static_cast<PyTypeObject*>(_supers->get(0));
        newKlass->setSuperKlass(super->getOwnKlass());
    }
    else {
        newKlass->setSuperKlass(ObjectKlass::getInstance());
    }

    // ����Python�����Ӧ��type���󣬲�ʵ��type�����C++ Klass��˫���
    PyTypeObject* cls = new PyTypeObject();
    cls->setOwnKlass(newKlass);

    return cls;
}

void* Klass::operator new(size_t size) {
    return Universe::PyHeap->allocateMeta(size);
}

void Klass::print(const PyObject* lhs, int flags) const {
    Klass* klass = lhs->getKlass();
    putchar('<');

    const PyDict* attrDict = klass->getKlassDict();
    if (attrDict != nullptr) {
        const PyObject* moduleName = attrDict->get(StringTable::str_mod);
        if (moduleName != nullptr) {
            moduleName->print(FLAG_PyString_PRINT_RAW);
            putchar('.');
        }
    }

    klass->getName()->print(FLAG_PyString_PRINT_RAW);
    printf(" object at 0x%p>", lhs);
}

void Klass::checkLegalPyObject(const PyObject* obj, const Klass* klass) {
    if (obj == nullptr || obj->getKlass() != klass) {
        printf("call IntegerKlass with illegal Python object.");
        exit(-1);
    }
}

PyObject* Klass::inplace_add(PyObject* lhs, PyObject* rhs) {
    printf("unsupported operand type(s) for +=: '%s' and '%s'",
        lhs->getKlassNameAsString(),
        rhs->getKlassNameAsString());
    exit(-1);
}

PyObject* Klass::getattr(PyObject* object, PyObject* attr) {
    assert(attr->getKlass() == StringKlass::getInstance());

    Klass* klass = object->getKlass();

    // TO DO: __getattr__ħ������

    // �ȼ��һ�¶����Լ���_self_dict
    PyObject* ret = nullptr;
    PyDict* selfDict = object->getSelfDict();
    if (selfDict != nullptr) {
        ret = selfDict->get(attr);
        /*
            ��Python�ж���ĳ����������Ϊһ��������
            ��getattrʱ�������ᱻ�������󷽷�����
            ����������轫���������PyMethod��ֱ�ӷ��ؼ��ɣ�
        */
        if (ret != nullptr) return ret;
    }

    // �ڶ��������������û�ҵ�������ȥ��klass
    // TO DO: ˳��MRO�̳�˳����
    ret = klass->getKlassDict()->get(attr);
    if (ret != nullptr) {
        // ������Ի�ȡĳ������ķ�������
        // ������ԭ�����������PyMethod
        Klass* retKlass = ret->getKlass();
        if (retKlass == FunctionKlass::getInstance()
            || retKlass == NativeFunctionKlass::getInstance()) {
            ret = new PyMethod(static_cast<PyFunction*>(ret), object);
        }    
    }

    if (ret == nullptr) {
        printf("'%s' object has no attribute '%s'\n", 
            klass->getName()->getValue(),
            static_cast<PyString*>(attr)->getValue());
        exit(-1);
    }

    return ret;
}

void Klass::setattr(PyObject* object, PyObject* attr, PyObject* value) {
    assert(attr->getKlass() == StringKlass::getInstance());

    // TO DO: __setattr__ħ������

    PyDict* selfDict = object->getSelfDict();
    if (selfDict == nullptr) selfDict = object->initSelfDict();
    selfDict->set(attr, value);
}

PyObject* Klass::allocateInstance(PyObject* callable, PyList* args) {
    START_COUNT_TEMP_OBJECTS;
    checkLegalPyObject(callable, TypeKlass::getInstance());
    PUSH_TEMP(callable);
    PUSH_TEMP(args);

    // Ϊʵ������Python��������ڴ�
    PyObject* inst = new PyObject();
    PUSH_TEMP(inst);

    // ΪPython�����klass
    PyTypeObject* cls = static_cast<PyTypeObject*>(callable);
    Klass* ownKlass = cls->getOwnKlass();
    inst->setKlass(ownKlass);
    
    // ����Python���__init__������ʼ������
    PyObject* constructor = ownKlass->getKlassDict()->get(StringTable::str_init);
    if (constructor != nullptr) {
        args->insert(0, inst);
        Interpreter::getInstance()->callVirtual(constructor, args);
    }

    END_COUNT_TEMP_OBJECTS;
    return inst;
}

void Klass::oops_do(OopClosure* closure) {
    closure->do_oop(reinterpret_cast<PyObject**>(&_klassDict));
    closure->do_oop(reinterpret_cast<PyObject**>(& _name));
    closure->do_klass(&_super);
    closure->do_oop(reinterpret_cast<PyObject**>(& _type_object));
}

void Klass::oops_do(OopClosure* closure, PyObject* object) {
    // Do nothing
}
