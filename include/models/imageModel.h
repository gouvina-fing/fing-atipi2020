#ifndef IMAGE_MODEL_H
#define IMAGE_MODEL_H

// Image representation
struct ImageModel {

    // Properties
    short width, height;
    unsigned char** matrix;
    bool empty;

    // Getters & Setters
    short getWidth();
    void setWidth(short w);
    short getHeight();
    void setHeight(short h);
    unsigned char** getMatrix();
    void setMatrix(unsigned char** m);
    bool getEmpty();
    void setEmpty(bool e);
};

#endif