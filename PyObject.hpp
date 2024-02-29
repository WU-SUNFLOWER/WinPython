#ifndef PyObject_Hpp
#define PyObject_Hpp
class PyObject {
public:
    virtual void print() const {};

    virtual PyObject* add(PyObject* other) const {
        return nullptr;
    };

    virtual PyObject* less(PyObject* other) const {
        return nullptr;
    };
    virtual PyObject* less_equal(PyObject* other) const {
        return nullptr;
    }
    virtual PyObject* equal(PyObject* other) const {
        return nullptr;
    }
    virtual PyObject* greater_equal(PyObject* other) const {
        return nullptr;
    }
    virtual PyObject* greater(PyObject* other) const {
        return nullptr;
    };
    virtual PyObject* not_equal(PyObject* other) const {
        return nullptr;
    }
};
#endif