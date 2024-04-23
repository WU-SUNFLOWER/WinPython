#ifndef ByteCodeFileParser_Hpp
#define ByteCodeFileParser_Hpp

#include "bufferedInputStream.hpp"
#include "arrayList.hpp"
#include "CodeObject.hpp"

class ByteCodeFileParser {
private:
    BufferedInputStream* fileStream;
    ArrayList<PyString*> stringTable;
public:
    ByteCodeFileParser(BufferedInputStream* stream) : fileStream(stream), stringTable(nullptr) {}
    CodeObject* parse();
    CodeObject* parseCodeObject();
    PyString* getByteCodes();
    PyString* getString();
    PyList* getTuple(bool needToCheck = true);
    PyString* getName();
    PyString* getNoTable();
};
#endif