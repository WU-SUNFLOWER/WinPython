#include "byteCodeFileParser.hpp"
#include "PyInteger.hpp"
#include <cassert>
#include <cstdlib>
#include "Universe.hpp"

CodeObject* ByteCodeFileParser::parse() {
    uint32_t magicNumber = fileStream->readInt();
    uint32_t modifiedData = fileStream->readInt();

    uint8_t objectType = fileStream->readByte();
    // ����CodeObject
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
    // CodeObject���������
    // ȫ��CodeObject���������Ĭ��Ϊ"<module>"
    // ������Ӧ��CodeObject����������뺯����һ��
    PyString* name = getName();  
    int32_t beginLineNo = fileStream->readInt();
    PyString* lnotab = getNoTable();

    return new CodeObject(argcount, nLocals, stackSize, flag, byteCodes,
        consts, names, varNames, freeVars, callVars, name, fileName,
        beginLineNo, lnotab
    );
}

PyString* ByteCodeFileParser::getByteCodes() {
    // �ֽ������ͨ�ַ�������`s`(0x73)��Ϊ��ͷ
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
    // tuple��`(`�ַ���0x28����Ϊ��ʼ��־
    if (needToCheck && fileStream->readByte() != '(') {
        fileStream->unreadByte();
        return nullptr;
    }
    int32_t length = fileStream->readInt();  // tuple��Ԫ�صĸ���
    PyList* tuple = new PyList(length);
    for (int32_t i = 0; i < length; ++i) {
        uint8_t objType = fileStream->readByte();
        switch (objType) {
            // Ԫ��ΪCode Object
            case 'c':
                tuple->append(parseCodeObject());
                break;
            // Ԫ��Ϊint
            case 'i':
                tuple->append(new PyInteger(fileStream->readInt()));
                break;
            // Ԫ��ΪNone Object
            case 'N':
                tuple->append(Universe::PyNone);
                break;
            // Ԫ��Ϊ�ַ���
            case 's':
                tuple->append(getString());
                break;
            // Ԫ��Ϊ�ַ���
            case 't': {
                PyString* str = getString();
                tuple->append(str);
                stringTable.push(str);
                break;
            }
            // Ԫ��Ϊint
            case 'R': 
                tuple->append(stringTable.get(fileStream->readInt()));
                break;
            case '(':
                tuple->append(getTuple(false));
                break;
            // �����޷�������object type��ֱ���˳�����
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