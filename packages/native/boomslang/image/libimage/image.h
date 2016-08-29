
#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

class boomslang_Image{
    public:
    boomslang_UnsignedInteger boomslang_texture;

    boomslang_Image();
    ~boomslang_Image();
    void boomslang_load(boomslang_String filename, boomslang_Boolean has_alpha = true, boomslang_Boolean blend = true);
    void boomslang_draw(boomslang_Number x, boomslang_Number y);

    // Not supported yet
    void boomslang_load(unsigned char* data, unsigned int len_h, unsigned int len_w, bool has_alpha, bool blend);
};

#endif // IMAGE_H_INCLUDED
