#include "PyFloat.hpp"
#include "Universe.hpp"
#include "FloatKlass.hpp" 


PyFloat* PyFloat::createFloat(double value) {
	return new PyFloat(value);
}

PyFloat::PyFloat(double v) : value(v) {
	setKlass(FloatKlass::getInstance());
}
