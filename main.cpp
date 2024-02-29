#include "bufferedInputStream.hpp"
#include "byteCodeFileParser.hpp"
#include "interpreter.hpp"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("vm need a parameter as filename");
        exit(-1);
    }
    BufferedInputStream stream(argv[1]);
    ByteCodeFileParser parser(&stream);
    CodeObject* code = parser.parse();
    Interpreter interpreter;
    interpreter.run(code);
    return 0;
}