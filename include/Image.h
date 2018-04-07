#ifndef IMAGE_H
#define IMAGE_H

/*Color part of pixel*/
enum Color{
    red = 0,green,blue
};

class Image
{
    public:
        Image();
        virtual ~Image();

        /* load image by name*/
        virtual void loadImage(char* name)=0;
        virtual char* getFileName()=0;

        /* function that deal with pixel */
        virtual unsigned int getPixel(int x,int y,Color col)=0;
        virtual unsigned int setPixel(int x,int y,Color col)=0;
        virtual unsigned int getGrayPixel(int x,int y)=0;

        /* save img by 2 dimention array ,return result */
        virtual bool saveIMG(unsigned int** img_data,unsigned int xSize,unsigned int ySize)=0;

        /* get size of picture*/
        virtual unsigned int getWidth()=0;
        virtual unsigned int getHeight()=0;

        /* get pixel array*/
        virtual void getPixelColorArray(int*** r,int*** g,int*** b)=0;
        virtual void getPixelGrayArray(int*** arr)=0;

        virtual void deleteArray(int ***ptr_array)=0;
    protected:
    private:
};

#endif // IIMAGE_H
