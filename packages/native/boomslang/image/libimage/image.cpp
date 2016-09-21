
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "C:/Boomslang/core/boomslangcore.h"
#include "image.h"

boomslang_Image::boomslang_Image(){}
boomslang_Image::~boomslang_Image(){
    glDeleteTextures(1 ,&boomslang_texture.data);
}
void boomslang_Image::boomslang_load(unsigned char* data, unsigned int len_h, unsigned int len_w, bool has_alpha, bool blend){
    int w, h, comp;
    unsigned char* image;

    if(has_alpha){
        image = stbi_load_from_memory(data, 4*len_w*len_h, &w, &h, &comp, STBI_rgb_alpha);
    } else {
        image = stbi_load_from_memory(data, 3*len_w*len_h, &w, &h, &comp, STBI_rgb);
    }

    if(image == NULL){
        std::cerr << "Failed to load image" << std::endl;
    }

    glGenTextures( 1, &boomslang_texture.data );
    glBindTexture( GL_TEXTURE_2D, boomslang_texture.data );

    if(!blend){
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    }
    else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }

    if(has_alpha){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    stbi_image_free(image);
}
void boomslang_Image::boomslang_load(boomslang_String filename, boomslang_Boolean has_alpha, boomslang_Boolean blend){
    int w, h, comp;
    unsigned char* image;

    if(has_alpha.data){
        image = stbi_load(filename.data.c_str(), &w, &h, &comp, STBI_rgb_alpha);
    }
    else {
        image = stbi_load(filename.data.c_str(), &w, &h, &comp, STBI_rgb);
    }

    if(image == NULL){
        std::cerr << "Failed to load image" << std::endl;
    }

    glGenTextures( 1, &boomslang_texture.data );
    glBindTexture( GL_TEXTURE_2D, boomslang_texture.data );

    if(!blend.data){
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    }
    else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }

    if(has_alpha.data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    stbi_image_free(image);
}
void boomslang_Image::boomslang_draw(boomslang_Double x, boomslang_Double y){
    int width;
    int height;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, boomslang_texture.data);

    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3ub(255, 255, 255);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2i(x.data, y.data);
    glTexCoord2i(1, 0);
    glVertex2i(x.data + width, y.data);
    glTexCoord2i(1, 1);
    glVertex2i(x.data + width, y.data + height);
    glTexCoord2i(0, 1);
    glVertex2i(x.data, y.data + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
