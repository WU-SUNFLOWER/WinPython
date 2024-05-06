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

    // 创建Python世界对应的type对象，并实现type对象和C++ Klass的双向绑定
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

    // TO DO: __getattr__魔法方法

    // 先检查一下对象自己的_self_dict
    PyObject* ret = nullptr;
    PyDict* selfDict = object->getSelfDict();
    if (selfDict != nullptr) {
        ret = selfDict->get(attr);
        /*
            若Python中对象某个自有属性为一个函数，
            在getattr时它将不会被当作对象方法处理。
            因此这里无需将函数打包成PyMethod，直接返回即可！
        */
        if (ret != nullptr) return ret;
    }

    // 在对象的自有属性中没找到，就跑去找klass
    // TO DO: 顺着MRO继承顺序找
    ret = klass->getKlassDict()->get(attr);
    if (ret != nullptr) {
        // 如果尝试获取某个对象的方法函数
        // 则将其由原生函数打包成PyMethod
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

    // TO DO: __setattr__魔法方法

    PyDict* selfDict = object->getSelfDict();
    if (selfDict == nullptr) selfDict = object->initSelfDict();
    selfDict->set(attr, value);
}

PyObject* Klass::allocateInstance(PyObject* callable, PyList* args) {
    START_COUNT_TEMP_OBJECTS;
    checkLegalPyObject(callable, TypeKlass::getInstance());
    PUSH_TEMP(callable);
    PUSH_TEMP(args);

    // 为实例化的Python对象分配内存
    PyObject* inst = new PyObject();
    PUSH_TEMP(inst);

    // 为Python对象绑定klass
    PyTypeObject* cls = static_cast<PyTypeObject*>(callable);
    Klass* ownKlass = cls->getOwnKlass();
    inst->setKlass(ownKlass);
    
    // 调用Python类的__init__函数初始化对象
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
