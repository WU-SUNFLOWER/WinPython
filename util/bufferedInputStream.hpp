#ifndef Buffered_Input_Stream_Hpp
#define Buffered_Input_Stream_Hpp

#include <string>
#include <cstdio>
#include <cstdint>
#include <cstdlib>

#define _CRT_SECURE_NO_WARNINGS

#define Buffer_Length 256

class BufferedInputStream {
private:
    FILE* fp;
    char buffer[Buffer_Length];
    unsigned short index;
public:
    BufferedInputStream(const char* filename) {
        //fp = fopen_s(filename, "rb");
        fopen_s(&fp, filename, "rb");
        if (fp == nullptr) exit(-1);
        fread(buffer, sizeof(char), Buffer_Length, fp);
        index = 0;
    }
    ~BufferedInputStream() {
        close();
    }
    uint8_t readByte() {
        // 如果临时缓冲区中的所有字节都被读取完了，需要继续从文件流中读数据
        if (index >= Buffer_Length) {
            fread(buffer, sizeof(char), Buffer_Length, fp);
            index = 0;
        }
        return buffer[index++];
    }
    void unreadByte() {
        --index;
    }







    int32_t readInt() {
        // 假设pyc文件采用小端序储存整型
        uint32_t b1 = readByte(); // 最低位
        uint32_t b2 = readByte();
        uint32_t b3 = readByte();
        uint32_t b4 = readByte();  // 最高位
        return b4 << 24 | b3 << 16 | b2 << 8 | b1;
    }

    double readDouble() {
        int32_t length = readByte();
        char* str = new char[length + 1];
        for (int32_t i = 0; i < length; ++i) {
            str[i] = readByte();
        }
        str[length] = '\0';
        double value = std::stod(str);
        delete[] str;
        return value;
    }

    void close() {
        if (fp != nullptr) {
            fclose(fp);
            fp = nullptr;
        }
    }
};

#endif