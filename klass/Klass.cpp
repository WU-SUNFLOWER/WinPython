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

PyObject* Klass::createKlass(
    Handle<PyObject*> dict, Handle<PyObject*> supers, Handle<PyObject*> name
) {
    checkLegalPyObject(dict, DictKlass::getInstance());
    checkLegalPyObject(supers, ListKlass::getInstance());
    checkLegalPyObject(name, StringKlass::getInstance());

    Handle<Klass*> newKlass = new Klass();

    // 将type object的名称挂到klass上
    newKlass->setName(name->as<PyString>());
    // 初始化supers列表
    newKlass->setSuperList(supers->as<PyList>());
    // 如果用户显式指定的super列表为空，则将<class 'object'>作为默认父类
    if (supers->as<PyList>()->getLength() <= 0) {
        newKlass->addSuper(ObjectKlass::getInstance());
    }

    // 创建Python世界对应的type对象，并实现type对象和C++ Klass的双向绑定
    Handle<PyTypeObject*> cls = new PyTypeObject();
    cls->setOwnKlass(newKlass);
    // 排序生成mro列表
    newKlass->orderSupers();

    // 为类方法绑定type object
    for (size_t i = 0; i < dict->as<PyDict>()->getSize(); ++i) {
        Handle<PyObject*> obj = dict->as<PyDict>()->getValueByIndex(i);
        if (!isPyInteger(obj()) && obj->getKlass() == FunctionKlass::getInstance()) {
            obj->as<PyFunction>()->setOwnerClass(cls);
        }
    }
    // 将type object的属性和方法挂到klass上
    newKlass->setKlassDict(dict->as<PyDict>());

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

PyObject* Klass::inplace_add(PyObject* lhs, PyObject* rhs) {
    printf("unsupported operand type(s) for +=: '%s' and '%s'",
        lhs->getKlassNameAsString(),
        rhs->getKlassNameAsString());
    exit(-1);
}

PyDict* Klass::init_self_dict(Handle<PyObject*> object) {
    assert(object->getSelfDict() == nullptr);
    Handle<PyDict*> dict = PyDict::createDict();
    object->setSelfDict(dict);
    return dict;
}

PyObject* Klass::find_in_parents(PyObject* object, PyObject* attr) {
    Klass* klass = object->getKlass();
    PyList* mroList = klass->mro();
    assert(mroList != nullptr);

    PyObject* result = nullptr;
    size_t mroLength = mroList->getLength();
    for (size_t i = 0; i < mroLength; i++) {
        
        PyTypeObject* type = static_cast<PyTypeObject*>(mroList->get(i));
        PyDict* klassDict = type->getOwnKlass()->getKlassDict();
        if (klassDict == nullptr) continue;
        
        result = klassDict->get(attr);
        if (result != nullptr) break;
    }

    return result;
}

PyObject* Klass::getattr(PyObject* object, PyObject* attr) {
    assert(attr->getKlass() == StringKlass::getInstance());

    PyObject* result = nullptr;

    // 首先在对象的自有属性中查找，找到就返回
    PyDict* selfDict = object->getSelfDict();
    if (selfDict != nullptr) {
        result = selfDict->get(attr);
        if (result != nullptr) {
            return result;
        }
    }

    // 如果没找到，就顺着mro列表的顺序遍历查找
    result = find_in_parents(object, attr);
    if (result) {
        if (!isPyInteger(result) && isCommonFuncKlass(result->getKlass())) {
            result = new PyMethod((PyFunction*)result, object);
        }
        return result;
    }

    // 如果在mro列表中也没找到，就触发__getattr__
    PyObject* getattr_func = object->getKlass()->getKlassDict()->get(StringTable::str_getattr);
    if (getattr_func && (getattr_func->getKlass() == FunctionKlass::getInstance())) {
        START_COUNT_TEMP_OBJECTS;
        getattr_func = new PyMethod((PyFunction*)getattr_func, object);
        PUSH_TEMP(attr);
        PUSH_TEMP(getattr_func);
        PyList* args = PyList::createList();
        args->append(attr);
        END_COUNT_TEMP_OBJECTS;
        return Interpreter::getInstance()->callVirtual(getattr_func, args);
    }

    if (result == nullptr) {
        printf("'%s' object has no attribute '%s'\n", 
            object->getKlassNameAsString(),
            static_cast<PyString*>(attr)->getValue());
        exit(-1);
    }

    return result;
}

void Klass::setattr(
    Handle<PyObject*> object, Handle<PyObject*> attr, Handle<PyObject*> value
) {
    assert(attr->getKlass() == StringKlass::getInstance());

    Handle<PyObject*> func = 
        object->getKlass()->getKlassDict()->get(StringTable::str_setattr);

    if (func && (func->getKlass() == FunctionKlass::getInstance())) {
        func = new PyMethod((PyFunction*)func(), object);
        Handle<PyList*> args = PyList::createList();
        args->append(attr);
        args->append(value);
        Interpreter::getInstance()->callVirtual(func, args);
        return;
    }

    Handle<PyDict*> selfDict = object->getSelfDict();
    if (selfDict == nullptr) {
        selfDict = object->initSelfDict();
    }
    selfDict->set(attr, value);
}

PyObject* Klass::allocateInstance(
    Handle<PyObject*> callable, Handle<PyList*> args
) {
    checkLegalPyObject(callable, TypeKlass::getInstance());

    // 为实例化的Python对象分配内存
    Handle<PyObject*> inst = new PyObject();

    // 为Python对象绑定klass
    Handle<PyTypeObject*> cls = static_cast<PyTypeObject*>(callable());
    Handle<Klass*> ownKlass = cls->getOwnKlass();
    inst->setKlass(ownKlass);

    // 调用Python类的__init__函数初始化对象
    Handle<PyObject*> constructor = ownKlass->getKlassDict()->get(StringTable::str_init);
    if (constructor != nullptr) {
        args->insert(0, inst);
        Interpreter::getInstance()->callVirtual(constructor, args);
    }

    return inst;
}

void Klass::oops_do(OopClosure* closure) {
    closure->do_oop(reinterpret_cast<PyObject**>(&_klassDict));
    closure->do_oop(reinterpret_cast<PyObject**>(&_name));
    closure->do_oop(reinterpret_cast<PyObject**>(&_supers));
    closure->do_oop(reinterpret_cast<PyObject**>(&_type_object));
    closure->do_oop(reinterpret_cast<PyObject**>(&_mro));
}

void Klass::oops_do(OopClosure* closure, PyObject* object) {
    // do nothing...
}

PyObject* Klass::isBoolTrue(PyObject* object) {
    return Universe::PyTrue;
}

void Klass::addSuper(Klass* klass) {
    if (_supers == nullptr) {
        _supers = PyList::createList();
    }
    _supers->append(klass->getTypeObject());
}

void Klass::orderSupers() {
    if (_supers == nullptr) {
        return;
    }

    if (_mro == nullptr) {
        _mro = PyList::createList();
    }

    // Method Resolution Order Algorithm
    for (size_t i = 0; i < _supers->getLength(); i++) {
        PyTypeObject* tp_obj = (PyTypeObject*)(_supers->get(i));
        Klass* k = tp_obj->getOwnKlass();
        PyList* k_mro = k->mro();
        
        _mro->append(tp_obj);

        if (k_mro == nullptr) continue;

        for (size_t j = 0; j < k_mro->getLength(); j++) {
            PyTypeObject* tp_obj = (PyTypeObject*)(k_mro->get(j));
            size_t index = _mro->index(tp_obj);

            if (index != NotFound) {
                _mro->deleteByIndex(index);
            }
            _mro->append(tp_obj);
        }
    }

    // 将类自身插入到mro序列的头部
    _mro->insert(0, getTypeObject());

}

