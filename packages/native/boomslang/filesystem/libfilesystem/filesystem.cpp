
#include "C:/Boomslang/core/boomslangcore.h"
#include "filesystem.h"

boomslang_InputFile::boomslang_InputFile(){}
boomslang_InputFile::boomslang_InputFile(boomslang_String filename){
    file.open( filename.data.c_str() );
}
boomslang_InputFile::~boomslang_InputFile(){
    if( file.is_open() ) file.close();
}
void boomslang_InputFile::boomslang_open(boomslang_String filename){
    file.open( filename.data.c_str() );
}
void boomslang_InputFile::boomslang_close(){
    file.close();
}
boomslang_Boolean boomslang_InputFile::boomslang_ok(){
    return boomslang_Boolean( file.is_open() );
}
boomslang_Boolean boomslang_InputFile::boomslang_eof(){
    return boomslang_Boolean( file.eof() );
}
boomslang_String boomslang_InputFile::boomslang_readLine(){
    boomslang_String result;
    std::getline(file, result.data, file.widen('\n'));
    return result;
}

boomslang_OutputFile::boomslang_OutputFile(){}
boomslang_OutputFile::boomslang_OutputFile(boomslang_String filename){
    file.open( filename.data.c_str() );
}
boomslang_OutputFile::~boomslang_OutputFile(){
    if( file.is_open() ) file.close();
}
void boomslang_OutputFile::boomslang_open(boomslang_String filename){
    file.open( filename.data.c_str() );
}
void boomslang_OutputFile::boomslang_close(){
    file.close();
}
boomslang_Boolean boomslang_OutputFile::boomslang_ok(){
    return boomslang_Boolean( file.is_open() );
}
void boomslang_OutputFile::boomslang_write(boomslang_String str){
    file << str.data;
}
void boomslang_OutputFile::boomslang_writeLine(boomslang_String str){
    file << str.data << std::endl;
}

boomslang_BinaryInputFile::boomslang_BinaryInputFile(){}
boomslang_BinaryInputFile::boomslang_BinaryInputFile(boomslang_String filename){
    file.open( filename.data.c_str(), std::ios::binary );
}
boomslang_BinaryInputFile::~boomslang_BinaryInputFile(){
    if( file.is_open() ) file.close();
}
void boomslang_BinaryInputFile::boomslang_open(boomslang_String filename){
    file.open( filename.data.c_str(), std::ios::binary );
}
void boomslang_BinaryInputFile::boomslang_close(){
    file.close();
}
boomslang_Boolean boomslang_BinaryInputFile::boomslang_ok(){
    return boomslang_Boolean( file.is_open() );
}
boomslang_Boolean boomslang_BinaryInputFile::boomslang_eof(){
    return boomslang_Boolean( file.eof() );
}
boomslang_Byte boomslang_BinaryInputFile::boomslang_readByte(){
    char byte; file.read(&byte, 1);
    return boomslang_Byte(byte);
}
boomslang_Integer boomslang_BinaryInputFile::boomslang_readInteger(){
    int32_t integer; file.read( (char*) &integer, sizeof(int32_t) );
    return boomslang_Integer(integer);
}
boomslang_UnsignedInteger boomslang_BinaryInputFile::boomslang_readUnsignedInteger(){
    uint32_t uinteger; file.read( (char*) &uinteger, sizeof(uint32_t) );
    return boomslang_UnsignedInteger(uinteger);
}
boomslang_Double boomslang_BinaryInputFile::boomslang_readDouble(){
    double number; file.read( (char*) &number, sizeof(double) );
    return boomslang_Double(number);
}
boomslang_Float boomslang_BinaryInputFile::boomslang_readFloat(){
    double number; file.read( (char*) &number, sizeof(double) );
    return boomslang_Float(number);
}
boomslang_Boolean boomslang_BinaryInputFile::boomslang_readBoolean(){
    bool boolean; file.read( (char*) &boolean, sizeof(bool) );
    return boomslang_Boolean(boolean);
}
boomslang_Array<boomslang_Byte> boomslang_BinaryInputFile::boomslang_readBytes(boomslang_Double len){
    boomslang_Array<boomslang_Byte> bytes(len);
    char* cstr_bytes = new char[ (int) len.data ];

    file.read(cstr_bytes, (size_t) len.data);

    for(size_t i = 0; i < (size_t) len.data; i++){
        bytes.data[i] = cstr_bytes[i];
    }

    delete[] cstr_bytes;
    return bytes;
}
boomslang_Array<boomslang_Byte> boomslang_BinaryInputFile::boomslang_readBytes(boomslang_Integer len){
    boomslang_Array<boomslang_Byte> bytes(len);
    char* cstr_bytes = new char[ (int) len.data ];

    file.read(cstr_bytes, (size_t) len.data);

    for(size_t i = 0; i < (size_t) len.data; i++){
        bytes.data[i] = cstr_bytes[i];
    }

    delete[] cstr_bytes;
    return bytes;
}

boomslang_BinaryOutputFile::boomslang_BinaryOutputFile(){}
boomslang_BinaryOutputFile::boomslang_BinaryOutputFile(boomslang_String filename){
    file.open( filename.data.c_str(), std::ios::binary );
}
boomslang_BinaryOutputFile::~boomslang_BinaryOutputFile(){
    if( file.is_open() ) file.close();
}
void boomslang_BinaryOutputFile::boomslang_open(boomslang_String filename){
    file.open( filename.data.c_str(), std::ios::binary );
}
void boomslang_BinaryOutputFile::boomslang_close(){
    file.close();
}
boomslang_Boolean boomslang_BinaryOutputFile::boomslang_ok(){
    return boomslang_Boolean( file.is_open() );
}
void boomslang_BinaryOutputFile::boomslang_writeByte(boomslang_Byte byte){
    file.write( (char*) &byte.data, sizeof(uint8_t) );
}
void boomslang_BinaryOutputFile::boomslang_writeInteger(boomslang_Integer integer){
    file.write( (char*) &integer.data, sizeof(int32_t) );
}
void boomslang_BinaryOutputFile::boomslang_writeUnsignedInteger(boomslang_UnsignedInteger unsigned_integer){
    file.write( (char*) &unsigned_integer.data, sizeof(uint32_t) );
}
void boomslang_BinaryOutputFile::boomslang_writeDouble(boomslang_Double number){
    file.write( (char*) &number.data, sizeof(double) );
}
void boomslang_BinaryOutputFile::boomslang_writeFloat(boomslang_Float number){
    file.write( (char*) &number.data, sizeof(double) );
}
void boomslang_BinaryOutputFile::boomslang_writeBoolean(boomslang_Boolean boolean){
    file.write( (char*) &boolean.data, sizeof(bool) );
}
