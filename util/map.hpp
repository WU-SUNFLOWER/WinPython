#ifndef Map_Hpp
#define Map_Hpp

// ��ϣ���Ա����
template<typename KEY, typename VAL>
class MapItem {
public:
    KEY key;
    VAL value;

    void* operator new[](size_t size);
    
    // �޲ι��캯��
    MapItem() : key(0), value(0) {};
    // ��ͨ���캯��
    MapItem(KEY key, VAL value) : key(key), value(value) {};
    // �������캯��
    MapItem(const MapItem<KEY, VAL>& other) : key(other.key), value(other.value) {};

};

template<typename KEY, typename VAL>
class Map {
private:
    VAL _default;
    MapItem<KEY, VAL>* ptr;
    size_t capacity;
    size_t length;
    void expand();
public:
    Map(VAL defaultValue);
    ~Map();
    
    void* operator new(size_t size);

    // ��ϣ�����ɾ�Ĳ��������
    void set(KEY key, VAL value);
    VAL remove(KEY key);
    VAL get(KEY key);
    bool has(KEY key);

    
    // ���ݹ�ϣ��������ʵ�±꣬����keyֵ��valueֵ
    KEY getKeyByIndex(size_t index);
    KEY getValueByIndex(size_t index);
    // ����keyֵ�������ϣ�������±�
    size_t getIndex(KEY& key);

    MapItem<KEY, VAL>* getPtr() const {
        return ptr;
    }

    size_t getLength() const {
        return length;
    }
    size_t getCapacity() const {
        return capacity;
    }
};
#endif