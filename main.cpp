#include "bufferedInputStream.hpp"
#include "byteCodeFileParser.hpp"
#include "interpreter.hpp"
#include "Universe.hpp"
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("vm need a parameter as filename");
        exit(-1);
    }
    Universe::genesis();
    BufferedInputStream stream(argv[1]);
    ByteCodeFileParser parser(&stream);
    CodeObject* code = parser.parse();
    Interpreter interpreter;
    interpreter.run(code);
    return 0;
}