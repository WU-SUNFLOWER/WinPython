#ifndef ByteCodeFileParser_Hpp
#define ByteCodeFileParser_Hpp

#include "bufferedInputStream.hpp"
#include "arrayList.hpp"
#include "CodeObject.hpp"
#include "PyInteger.hpp"

class ByteCodeFileParser {
private:
    BufferedInputStream* fileStream;
    ArrayList<PyString*> stringTable;
public:
    ByteCodeFileParser(BufferedInputStream* stream) : fileStream(stream) {}
    CodeObject* parse();
    CodeObject* parseCodeObject();
    PyString* getByteCodes();
    PyString* getString();
    ArrayList<PyObject*>* getTuple();
    PyString* getName();
    PyString* getNoTable();
};
#endif