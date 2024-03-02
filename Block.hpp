#ifndef Block_Hpp
#define Block_Hpp

#include <cstdint>

class Block {
private:
    uint8_t _type;
    size_t _target;
    size_t _level;
public:
    Block() : _type(0), _target(0), _level(0) {};
    Block(uint8_t type, size_t target, size_t level)
        : _type(type), _target(target), _level(level) {};
    Block(const Block& other)
        : _type(other._type), _target(other._target), _level(other._level) {};

    friend class Interpreter;
};
#endif