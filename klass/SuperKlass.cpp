#include "SuperKlass.hpp"
#include "PySuper.hpp"
#include "StringTable.hpp"
#include "ObjectKlass.hpp"
#include "Universe.hpp"
#include "PyDict.hpp"
#include "interpreter.hpp"
#include "TypeKlass.hpp"
#include "PyMethod.hpp"
#include "Universe.hpp"
#include "FunctionKlass.hpp"
#include "NativeFunctionKlass.hpp"
#include "FrameObject.hpp"

SuperKlass* SuperKlass::instance = nullptr;

void SuperKlass::initialize() {
    (new PyTypeObject())->setOwnKlass(this);
    setName(StringTable::str_super);
    addSuper(ObjectKlass::getInstance());
}

size_t SuperKlass::getSize() {
    return sizeof(PySuper);
}

PyObject* SuperKlass::getattr(
    Handle<PyObject*> superObject, Handle<PyObject*> attr
) {
    checkLegalPyObject(superObject, this);

    Handle<PyObject*> obj = superObject->as<PySuper>()->object;
    Handle<PyTypeObject*> type = superObject->as<PySuper>()->type;

    Handle<PyList*> mroList = obj->getKlass()->mro();
    size_t mroLength = mroList->getLength();
    size_t index = mroList->index(type) + 1;
    
    Handle<PyObject*> result = nullptr;
    while (index < mroLength) {
        Handle<PyTypeObject*> tp = static_cast<PyTypeObject*>(mroList->get(index));
        Handle<PyDict*> klassDict = tp->getOwnKlass()->getKlassDict();
        if (klassDict != nullptr) {
            result = klassDict->get(attr);
            if (result != nullptr) {
                break;
            }
        }
        ++index;
    }

    if (result != nullptr) {
        if (!isPyInteger(result()) && isCommonFuncKlass(result->getKlass())) {
            result = new PyMethod(result->as<PyFunction>(), obj);
        }
        return result;
    }
    else {
        printf("'super' object has no attribute '%s'",
               attr->as<PyString>()->getValue());
        exit(-1);
    }

    return nullptr;
}

PyObject* SuperKlass::allocateInstance(Handle<PyObject*> callable, Handle<PyList*> args) {

    assert(callable == getTypeObject());

    Handle<PyTypeObject*> type = nullptr;
    Handle<PyObject*> obj = nullptr;
    switch (args->getLength()) {
        case 0: {
            Handle<FrameObject*> frame = Interpreter::getInstance()->getCurrentFrame();
            type = static_cast<PyTypeObject*>(frame->_globals->get(StringTable::str_class));
            if (!type) {
                printf("name '__class__' is not defined\n");
                exit(-1);
            }
            obj = frame->_fastLocals->get(0);
            break;
        }
        case 2: {
            type = static_cast<PyTypeObject*>(args->get(0));
            obj = args->get(1);
            if (type->getKlass() != TypeKlass::getInstance()) {
                printf("super() argument 1 must be type, not %s",
                    type->getKlassNameAsString());
                exit(-1);
            }
            if (obj->isinstance(type) != Universe::PyTrue) {
                printf("super(type, obj): obj must be an instance of type\n");
                exit(-1);
            }
            break;
        }
        default: {
            printf("Typical use to call a cooperative superclass method:\n"
                   "    super() -> same as super(__class__, <first argument>)\n"
                   "    super(type, obj) -> bound super object; requires isinstance(obj, type)\n");
            exit(-1);
        }
    }

    // 为实例化的Python对象分配内存
    Handle<PyObject*> inst = new PySuper(type, obj);

    // 为Python对象绑定klass
    inst->setKlass(this);

    return inst;
}