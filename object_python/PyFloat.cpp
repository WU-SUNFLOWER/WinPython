#include "PyFloat.hpp"
#include "Universe.hpp"
#include "FloatKlass.hpp" 


PyFloat* PyFloat::createFloat(double value) {
	return new PyFloat(value);
}

PyObject* PyFloat::add(PyObject* other) {
	if (other->getKlass() == FloatKlass::getInstance()) {
		return new PyFloat(this->value + static_cast<PyFloat*>(other)->getValue());

	}
	return nullptr;
}

PyObject* PyFloat::sub(PyObject* other) {
	if (other->getKlass() == FloatKlass::getInstance()) {
		return new PyFloat(this->value - static_cast<PyFloat*>(other)->getValue());
	}
	return nullptr;
}

PyObject* PyFloat::mul(PyObject* other) {
	if (other->getKlass() == FloatKlass::getInstance()) {
		return new PyFloat(this->value * static_cast<PyFloat*>(other)->getValue());
	}
	return nullptr;
}

PyObject* PyFloat::div(PyObject* other) {
	if (other->getKlass() == FloatKlass::getInstance()) {
		double otherValue = static_cast<PyFloat*>(other)->getValue();
		if (otherValue != 0) {
			return new PyFloat(this->value / otherValue);
		}
		else {
			printf("³ýÊýÎª0");
			return NULL;
		}
	}
	return nullptr;
}

void* PyFloat::operator new(size_t size) {
	return Universe::PyHeap->allocateMeta(size);
}

void PyFloat::operator delete(void*) {
	puts("Can't delete PyFloat object.");
	exit(-1);
}

PyFloat::PyFloat(double v) : value(v) {
	setKlass(FloatKlass::getInstance());
}
