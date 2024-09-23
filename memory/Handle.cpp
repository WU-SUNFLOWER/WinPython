#include "Handle.hpp"
#include "FrameObject.hpp"
#include "PyList.hpp"
#include "PyFunction.hpp"
#include "PyMethod.hpp"
#include "PyCell.hpp"
#include "map.hpp"
#include "PyRange.hpp"
#include "PySuper.hpp"

template<class T>
inline Handle<T>::Handle(T value) : LinkedList(), _value(value) {
	HandleMark::getInstance()->add_handle(this);
}

template<class T>
inline Handle<T>::Handle(const Handle<T>& other)
	: LinkedList(), _value(other._value) 
{
	HandleMark::getInstance()->add_handle(this);
}

template<class T>
Handle<T>::~Handle() {
	_value = nullptr;
	HandleMark::getInstance()->del_handle(this);
}

template<class T>
void Handle<T>::oops_do(OopClosure* closure) {
	closure->do_oop(reinterpret_cast<PyObject**>(& _value));
}

template<>
void Handle<FrameObject*>::oops_do(OopClosure* closure) {
	_value->oops_do(closure);
}

template<>
void Handle<Klass*>::oops_do(OopClosure* closure) {
	_value->oops_do(closure);
}

template<>
void Handle<Map<PyObject*, PyObject*>*>::oops_do(OopClosure* closure) {
	closure->do_map(&_value);
}

template<>
void Handle<ArrayList<PyObject*>*>::oops_do(OopClosure* closure) {
	closure->do_array_list(&_value);
}

template<>
void Handle<ArrayList<Klass*>*>::oops_do(OopClosure* closure) {
	closure->do_array_list(&_value);
}

template class Handle<PyFunction*>;
template class Handle<PyMethod*>;
template class Handle<PyList*>;
template class Handle<PyDict*>;
template class Handle<PyObject*>;
template class Handle<PyCell*>;
template class Handle<PyString*>;
template class Handle<PyTypeObject*>;
template class Handle<PyRange*>;
template class Handle<PySuper*>;
template class Handle<CodeObject*>;
template class Handle<FrameObject*>;
template class Handle<Klass*>;
template class Handle<Map<PyObject*, PyObject*>*>;
template class Handle<ArrayList<PyObject*>*>;
template class Handle<ArrayList<Klass*>*>;

HandleMark* HandleMark::instance = nullptr;

HandleMark::HandleMark() {

}

HandleMark* HandleMark::getInstance() {
	if (instance == nullptr) {
		instance = new HandleMark();
	}
	return instance;
}

void HandleMark::oops_do(OopClosure* closure) {
	LinkedList* cur = &_handle_list;
	while ((cur = cur->next()) != &_handle_list) {
		cur->oops_do(closure);
	}
}

void HandleMark::add_handle(LinkedList* handle_node) {
	handle_node->add_before(&_handle_list);
}

void HandleMark::del_handle(LinkedList* handle_node) {
	handle_node->del();
}