#ifndef Handle_Hpp
#define Handle_Hpp

#include "LinkedList.hpp"
#include "OopClosure.hpp"

template<class T>
class Handle : public LinkedList {
private:
	T _value;
public:
	Handle(T value);
	Handle(const Handle<T>& other);
	~Handle();

	virtual void oops_do(OopClosure* closure) override;

	// 重载->运算符
	T operator ->() { return _value; }
	// 重载函数调用运算符
	T operator ()() { return _value; }
	// 这是类型转换运算符重载，允许将 Handle 对象隐式转换为类型 T。
	operator T() { return _value; }

	void operator =(T v) { _value = v; }
	void operator =(const Handle<T>& other) { _value = other._value; }
	bool operator ==(T v) { return _value == v; }
	bool operator ==(const Handle<T>& other) { return _value == other._value; }
	bool operator !=(T v) { return _value != v; }
	bool operator !=(const Handle<T>& other) { return _value != other._value; }

};

class HandleMark {
private:
	static HandleMark* instance;
	LinkedList _handle_list;
	HandleMark();
public:
	static HandleMark* getInstance();

	void oops_do(OopClosure* closure);

	void add_handle(LinkedList* handle_node);
	void del_handle(LinkedList* handle_node);
};

#endif