#include "bufferedInputStream.hpp"
#include "byteCodeFileParser.hpp"
#include "interpreter.hpp"
#include "Universe.hpp"
#include <cstdlib>
#include <Windows.h>
#include <filesystem>
#include <clocale>

typedef void(*Convert_Func)(const char*, const char*);

int main(int argc, char* argv[]) {
    
    std::string byteCodeFile;
    
    // 传入.py文件
    if (argc == 2) {
        // 加载dll文件
        HINSTANCE dllHandle = LoadLibrary("python27_d.dll");
        if (!dllHandle) {
            printf("Fail to load python27_d.dll");
            exit(-1);
        }
        // 加载转换函数
        Convert_Func converter = 
            reinterpret_cast<Convert_Func>(GetProcAddress(dllHandle, "ConvertCodeStringToByteCode"));
        if (!converter) {
            printf("Fail to load ConvertCodeStringToByteCode");
            exit(-1);
        }
        // 执行转换
        const char* sourceFile = argv[1];
        byteCodeFile =
            std::filesystem::path(sourceFile).replace_extension("pyc").string();
        converter(sourceFile, byteCodeFile.c_str());
    }
    // 传入.pyc文件
    else if (argc == 3 && strcmp(argv[1], "-c") == 0) {
        byteCodeFile = argv[2];
    }
    else {
        puts("Invalid arguments. Use either:\n");
        puts("WinPython <python source code filename>");
        puts("WinPython -c <python bytecode filename>");
        exit(-1);
    }
    setlocale(LC_CTYPE, ".UTF-8");  // 启用UTF8编码，这样中文显示才不会乱码
    Universe::genesis();
    BufferedInputStream stream(byteCodeFile.c_str());
    ByteCodeFileParser parser(&stream);
    CodeObject* code = parser.parse();
    Interpreter* interpreter = interpreter->getInstance();
    interpreter->run(code);
    return 0;
}