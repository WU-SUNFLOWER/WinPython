#ifndef Buffered_Input_Stream_Hpp
#define Buffered_Input_Stream_Hpp

#include <cstdio>
#include <cstdint>

#define Buffer_Length 256

class BufferedInputStream {
private:
    FILE* fp;
    char buffer[Buffer_Length];
    unsigned short index;
public:
    BufferedInputStream(const char* filename) {
        fp = fopen(filename, "rb");
        fread(buffer, sizeof(char), Buffer_Length, fp);
        index = 0;
    }
    ~BufferedInputStream() {
        close();
    }
    uint8_t readByte() {
        // �����ʱ�������е������ֽڶ�����ȡ���ˣ���Ҫ�������ļ����ж�����
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
        // ����pyc�ļ�����С���򴢴�����
        uint32_t b1 = readByte(); // ���λ
        uint32_t b2 = readByte();
        uint32_t b3 = readByte();
        uint32_t b4 = readByte();  // ���λ
        return b4 << 24 | b3 << 16 | b2 << 8 | b1;
    }
    void close() {
        if (fp != nullptr) {
            fclose(fp);
            fp = nullptr;
        }
    }
};

#endif