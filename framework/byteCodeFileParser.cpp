#include "byteCodeFileParser.hpp"
#include "PyInteger.hpp"
#include <cassert>
#include <cstdlib>
#include "Universe.hpp"

CodeObject* ByteCodeFileParser::parse() {
    uint32_t magicNumber = fileStream->readInt();
    uint32_t modifiedData = fileStream->readInt();

    uint8_t objectType = fileStream->readByte();
    // 解析CodeObject
    if (objectType == 'c') {
        CodeObject* result = parseCodeObject();
        return result;
    }

    return nullptr;
}

CodeObject* ByteCodeFileParser::parseCodeObject() {
    int32_t argcount = fileStream->readInt();
    int32_t nLocals = fileStream->readInt();
    int32_t stackSize = fileStream->readInt();
    int32_t flag = fileStream->readInt();

    PyString* byteCodes = getByteCodes();
    PyList* consts = getTuple();
    PyList* names = getTuple();  
    PyList* varNames = getTuple();
    PyList* freeVars = getTuple();
    PyList* callVars = getTuple();

    PyString* fileName = getName();
    // CodeObject对象的名称
    // 全局CodeObject对象的名称默认为"<module>"
    // 函数对应的CodeObject对象的名称与函数名一致
    PyString* name = getName();  
    int32_t beginLineNo = fileStream->readInt();
    PyString* lnotab = getNoTable();

    return new CodeObject(argcount, nLocals, stackSize, flag, byteCodes,
        consts, names, varNames, freeVars, callVars, name, fileName,
        beginLineNo, lnotab
    );
}

PyString* ByteCodeFileParser::getByteCodes() {
    // 字节码和普通字符串都以`s`(0x73)作为开头
    assert(fileStream->readByte() == 's');
    return getString();
}

PyString* ByteCodeFileParser::getString() {
    int32_t length = fileStream->readInt();
    uint8_t* tempBuf = new uint8_t[length];
    for (size_t i = 0; i < length; ++i) {
        tempBuf[i] = fileStream->readByte();
    }
    PyString* s = new PyString(tempBuf, length);
    delete[] tempBuf;
    return s;
}

PyList* ByteCodeFileParser::getTuple(bool needToCheck) {
    // tuple以`(`字符（0x28）作为起始标志
    if (needToCheck && fileStream->readByte() != '(') {
        fileStream->unreadByte();
        return nullptr;
    }
    int32_t length = fileStream->readInt();  // tuple中元素的个数
    PyList* tuple = new PyList(length);
    for (int32_t i = 0; i < length; ++i) {
        uint8_t objType = fileStream->readByte();
        switch (objType) {
            // 元素为Code Object
            case 'c':
                tuple->append(parseCodeObject());
                break;
            // 元素为int
            case 'i':
                tuple->append(new PyInteger(fileStream->readInt()));
                break;
            // 元素为None Object
            case 'N':
                tuple->append(Universe::PyNone);
                break;
            // 元素为字符串
            case 's':
                tuple->append(getString());
                break;
            // 元素为字符串
            case 't': {
                PyString* str = getString();
                tuple->append(str);
                stringTable.push(str);
                break;
            }
            // 元素为int
            case 'R': 
                tuple->append(stringTable.get(fileStream->readInt()));
                break;
            case '(':
                tuple->append(getTuple(false));
                break;
            // 遇到无法解析的object type，直接退出程序
            default:
                printf("Unknown Object Type 0x%x\n", objType);
                exit(-1);
        }
    }
    return tuple;
}

PyString* ByteCodeFileParser::getName() {
    uint8_t objType = fileStream->readByte();
    switch (objType) {
        case 's':
            return getString();
        case 't': {
            PyString* str = getString();
            stringTable.push(str);
            return str;
        }
        case 'R':
            return stringTable.get(fileStream->readInt());
        default:
            printf("Unknown Object Type 0x%x When executing ByteCodeFileParser::getName() \n", objType);
            exit(-1);
    }
}

PyString* ByteCodeFileParser::getNoTable() {
    uint8_t objType = fileStream->readByte();
    if (objType != 's' && objType != 't') {
        fileStream->unreadByte();
        return nullptr;
    }
    return getString();
}