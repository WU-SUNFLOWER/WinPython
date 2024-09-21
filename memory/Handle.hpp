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

	// ����->�����
	T operator ->() { return _value; }
	// ���غ������������
	T operator ()() { return _value; }
	// ��������ת����������أ����� Handle ������ʽת��Ϊ���� T��
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