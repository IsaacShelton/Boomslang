
#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

#include <fstream>

class boomslang_InputFile {
    public:
    std::ifstream file;

    boomslang_InputFile();
    boomslang_InputFile(boomslang_String);
    ~boomslang_InputFile();
    void boomslang_open(boomslang_String);
    void boomslang_close();
    boomslang_Boolean boomslang_eof();
    boomslang_Boolean boomslang_ok();
    boomslang_String boomslang_readLine();
};

class boomslang_OutputFile {
    public:
    std::ofstream file;

    boomslang_OutputFile();
    boomslang_OutputFile(boomslang_String);
    ~boomslang_OutputFile();
    void boomslang_open(boomslang_String);
    void boomslang_close();
    boomslang_Boolean boomslang_ok();
    void boomslang_write(boomslang_String);
    void boomslang_writeLine(boomslang_String);
};

class boomslang_BinaryInputFile {
    public:
    std::ifstream file;

    boomslang_BinaryInputFile();
    boomslang_BinaryInputFile(boomslang_String);
    ~boomslang_BinaryInputFile();
    void boomslang_open(boomslang_String);
    void boomslang_close();
    boomslang_Boolean boomslang_eof();
    boomslang_Boolean boomslang_ok();
    boomslang_Byte boomslang_readByte();
    boomslang_Integer boomslang_readInteger();
    boomslang_UnsignedInteger boomslang_readUnsignedInteger();
    boomslang_Double boomslang_readDouble();
    boomslang_Float boomslang_readFloat();
    boomslang_Boolean boomslang_readBoolean();
    boomslang_Array<boomslang_Byte> boomslang_readBytes(boomslang_Double);
    boomslang_Array<boomslang_Byte> boomslang_readBytes(boomslang_Integer);
};

class boomslang_BinaryOutputFile {
    public:
    std::ofstream file;

    boomslang_BinaryOutputFile();
    boomslang_BinaryOutputFile(boomslang_String);
    ~boomslang_BinaryOutputFile();
    void boomslang_open(boomslang_String);
    void boomslang_close();
    boomslang_Boolean boomslang_ok();
    void boomslang_writeByte(boomslang_Byte);
    void boomslang_writeInteger(boomslang_Integer);
    void boomslang_writeUnsignedInteger(boomslang_UnsignedInteger);
    void boomslang_writeDouble(boomslang_Double);
    void boomslang_writeFloat(boomslang_Float);
    void boomslang_writeBoolean(boomslang_Boolean);
};

#endif // FILESYSTEM_H_INCLUDED
