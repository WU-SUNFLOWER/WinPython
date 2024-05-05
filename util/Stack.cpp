#include "Stack.hpp"
#include "PyObject.hpp"
#include "arrayList.hpp"
#include "Block.hpp"

template class Stack<PyObject**>;
template class Stack<PyObject*>;
template class Stack<Block>;
template class Stack<ArrayList<PyObject*>*>;