#ifndef ByteCodeFileParser_Hpp
#define ByteCodeFileParser_Hpp

#include "PyFloat.hpp"
#include "bufferedInputStream.hpp"
#include "arrayList.hpp"
#include "CodeObject.hpp"

class ByteCodeFileParser {
private:
    BufferedInputStream* fileStream;
    ArrayList<PyObject*>* stringTable;
public:
    ByteCodeFileParser(BufferedInputStream* stream);
    CodeObject* parse();
    CodeObject* parseCodeObject();
    PyString* getByteCodes();
    PyString* getString();
    PyList* getTuple(bool needToCheck = true);
    PyString* getName();
    PyString* getNoTable();
    PyFloat* getFloat();
};
#endif