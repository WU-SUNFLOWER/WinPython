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

    // ��type object�����ƹҵ�klass��
    newKlass->setName(name->as<PyString>());
    // ��ʼ��supers�б�
    newKlass->setSuperList(supers->as<PyList>());
    // ����û���ʽָ����super�б�Ϊ�գ���<class 'object'>��ΪĬ�ϸ���
    if (supers->as<PyList>()->getLength() <= 0) {
        newKlass->addSuper(ObjectKlass::getInstance());
    }

    // ����Python�����Ӧ��type���󣬲�ʵ��type�����C++ Klass��˫���
    Handle<PyTypeObject*> cls = new PyTypeObject();
    cls->setOwnKlass(newKlass);
    // ��������mro�б�
    newKlass->orderSupers();

    // Ϊ�෽����type object
    for (size_t i = 0; i < dict->as<PyDict>()->getSize(); ++i) {
        Handle<PyObject*> obj = dict->as<PyDict>()->getValueByIndex(i);
        if (!isPyInteger(obj()) && obj->getKlass() == FunctionKlass::getInstance()) {
            obj->as<PyFunction>()->setOwnerClass(cls);
        }
    }
    // ��type object�����Ժͷ����ҵ�klass��
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

    // �����ڶ�������������в��ң��ҵ��ͷ���
    PyDict* selfDict = object->getSelfDict();
    if (selfDict != nullptr) {
        result = selfDict->get(attr);
        if (result != nullptr) {
            return result;
        }
    }

    // ���û�ҵ�����˳��mro�б��˳���������
    result = find_in_parents(object, attr);
    if (result) {
        if (!isPyInteger(result) && isCommonFuncKlass(result->getKlass())) {
            result = new PyMethod((PyFunction*)result, object);
        }
        return result;
    }

    // �����mro�б���Ҳû�ҵ����ʹ���__getattr__
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

    // Ϊʵ������Python��������ڴ�
    Handle<PyObject*> inst = new PyObject();

    // ΪPython�����klass
    Handle<PyTypeObject*> cls = static_cast<PyTypeObject*>(callable());
    Handle<Klass*> ownKlass = cls->getOwnKlass();
    inst->setKlass(ownKlass);

    // ����Python���__init__������ʼ������
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

    // ����������뵽mro���е�ͷ��
    _mro->insert(0, getTypeObject());

}

