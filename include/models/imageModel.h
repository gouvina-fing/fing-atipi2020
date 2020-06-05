#ifndef IMAGE_MODEL_H
#define IMAGE_MODEL_H

// Image representation
struct ImageModel {

    // Properties
    short width, height;
    float** matrix;

    // Getters & Setters
    short getWidth();
    void setWidth(short w);
    short getHeight();
    void setHeight(short h);
    float** getMatrix();
    void setMatrix(float** m);
};

#endif