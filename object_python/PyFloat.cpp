#include "PyFloat.hpp"
#include "Universe.hpp"
#include "FloatKlass.hpp" 


PyFloat* PyFloat::createFloat(double value) {
	return new PyFloat(value);
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
