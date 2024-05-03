#include "Stack.hpp"
#include "PyObject.hpp"
#include "arrayList.hpp"

template class Stack<PyObject**>;
template class Stack<PyObject*>;
template class Stack<ArrayList<PyObject*>*>;