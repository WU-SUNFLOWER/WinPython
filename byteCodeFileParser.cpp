#include "byteCodeFileParser.hpp"
#include <cassert>

CodeObject* ByteCodeFileParser::parse() {
    uint32_t magicNumber = fileStream->readInt();
    uint32_t modifiedData = fileStream->readInt();

    printf("magic number=0x%x, data=0x%x\n", magicNumber, modifiedData);
    
    uint8_t objectType = fileStream->readByte();
    // ����CodeObject
    if (objectType == 'c') {
        CodeObject* result = parseCodeObject();
        puts("success to parse code object!");
        return result;
    }

    return nullptr;
}

CodeObject* ByteCodeFileParser::parseCodeObject() {
    int32_t argcount = fileStream->readInt();
    int32_t nLocals = fileStream->readInt();
    int32_t stackSize = fileStream->readInt();
    int32_t flag = fileStream->readInt();
    printf("argcount=%d, flag=%d\n", argcount, flag);

    PyString* byteCodes = getByteCodes();
    ArrayList<PyObject*>* consts = getTuple();
    ArrayList<PyObject*>* names = getTuple();
    ArrayList<PyObject*>* varNames = getTuple();
    ArrayList<PyObject*>* freeVars = getTuple();
    ArrayList<PyObject*>* callVars = getTuple();

    PyString* fileName = getName();
    PyString* moduleName = getName();
    int32_t beginLineNo = fileStream->readInt();
    PyString* lnotab = getNoTable();

    return new CodeObject(argcount, nLocals, stackSize, flag, byteCodes,
        consts, names, varNames, freeVars, callVars, moduleName, fileName,
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

ArrayList<PyObject*>* ByteCodeFileParser::getTuple() {
    // tuple��`(`�ַ���0x28����Ϊ��ʼ��־
    if (fileStream->readByte() != '(') {
        fileStream->unreadByte();
        return nullptr;
    }
    int32_t length = fileStream->readInt();  // tuple��Ԫ�صĸ���
    ArrayList<PyObject*>* tuple = new ArrayList<PyObject*>(length);
    for (int32_t i = 0; i < length; ++i) {
        uint8_t objType = fileStream->readByte();
        switch (objType) {
            // Ԫ��ΪCode Object
            case 'c':
                tuple->push(parseCodeObject());
                break;
            // Ԫ��Ϊint
            case 'i':
                tuple->push(new PyInteger(fileStream->readInt()));
                break;
            // Ԫ��ΪNone Object
            case 'N':
                tuple->push(nullptr);
                break;
            // Ԫ��Ϊ�ַ���
            case 's':
                tuple->push(getString());
                break;
            // 
            case 't': {
                PyString* str = getString();
                tuple->push(str);
                stringTable.push(str);
                break;
            }
            //
            case 'R': 
                tuple->push(stringTable.get(fileStream->readInt()));
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