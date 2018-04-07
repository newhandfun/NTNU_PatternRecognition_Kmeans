#ifndef CIMGIMAGE_H
#define CIMGIMAGE_H

#include "Image.h"

#include "CImg.h"
using namespace cimg_library;

class CImgImage : public Image
{
    public:
        CImgImage();
        ~CImgImage();
//
        /* load image by name*/
        void loadImage(char* name) override;
        char* getFileName() override;

        /* function that deal with pixel */
        unsigned int getPixel(int x,int y,Color col) override;
        unsigned int setPixel(int x,int y,Color col) override;
        unsigned int getGrayPixel(int x,int y) override;

        /* save img by 2 ŝdimention array ,return result */
        bool saveIMG(unsigned int** img_data,unsigned int xSize,unsigned int ySize) override;

        /* get size of picture*/
        unsigned int getWidth() override{
            return data.width();
        }
        unsigned int getHeight() override{
            return data.height();
        }
        /* get pixel array*/
        void getPixelColorArray(int*** ptr_r,int*** ptr_g,int*** ptr_b) override;
        void getPixelGrayArray(int*** arr) override;

        void deleteArray(int ***ptr_array) override;
    protected:

        /* data get/set */
        CImg<unsigned char> Getdata() { return data; }
        void Setdata(CImg<unsigned char> val) { data = val; }

    private:
        CImg<unsigned char> data;
        char file_name[200];
        int ***ptr_r=nullptr,***ptr_g=nullptr,*** ptr_b=nullptr,***ptr_gray=nullptr;

};
#endif // CIMGIMAGE_H
