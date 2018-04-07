#include "CImgImage.h"
#include "ctype.h"

#include "CImg.h"
using namespace cimg_library;

#include <iostream>
using namespace std;

CImgImage::CImgImage()
{
}

CImgImage::~CImgImage()
{
    if(ptr_r!=nullptr)
        deleteArray(ptr_r);
    if(ptr_g!=nullptr)
        deleteArray(ptr_g);
    if(ptr_b!=nullptr)
        deleteArray(ptr_b);
    if(ptr_gray!=nullptr)
        deleteArray(ptr_gray);
//    if(file_name!=nullptr)
//    delete file_name;
}

void CImgImage::loadImage(char* name){
    strcpy(file_name,name);
    data.load(name);
//    cout<<"create image"<<file_name<<":"<<name<<endl;
}

char* CImgImage::getFileName(){
    return file_name;
}

/* function that deal with pixel */
unsigned int CImgImage::getPixel(int x,int y,Color col){
    return data(x,y,(unsigned int)col);
//    return 0;
}
unsigned int CImgImage::getGrayPixel(int x,int y){
    return (unsigned int)(
        0.3333*getPixel(x,y,Color::red) +
        0.3333*getPixel(x,y,Color::green)  +
        0.3333*getPixel(x,y,Color::blue)
         );
}
unsigned int CImgImage::setPixel(int x,int y,Color col){
    return 0;
}

/* save img by 2 dimention array ,return result */
bool CImgImage::saveIMG(unsigned int** img_data,unsigned int xSize,unsigned int ySize){
    return false;
}

/*get 3 orginal color array of per pixel*/
void CImgImage::getPixelColorArray(int ***r,int ***g,int*** b){
    int w = getWidth();
    int h = getHeight();

    if((*r)!=nullptr)delete *ptr_r;
    if((*g)!=nullptr)delete *ptr_g;
    if((*b)!=nullptr)delete *ptr_b;

    *r = new int*[w];
    *g = new int*[w];
    *b = new int*[w];
    for(int i=0;i<w;i++){
        (*r)[i] = new int[h];
        (*g)[i] = new int[h];
        (*b)[i] = new int[h];
        for(int j=0;j<h;j++){
            (*r)[i][j] = getPixel(i,j,Color::red);
            (*g)[i][j] = getPixel(i,j,Color::green);
            (*b)[i][j] = getPixel(i,j,Color::blue);
        }
    }
}

void CImgImage::deleteArray(int ***ptr_array){
    int w = getWidth();
    int h = getHeight();
    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
//            delete[] (*ptr_array)[i]+j ;
        }
        delete[] (*ptr_array)[i];
    }
    delete[] (*ptr_array);
}

void CImgImage::getPixelGrayArray(int ***arr){
    int w = getWidth();
    int h = getHeight();
//    cout<<"get pixel gray array"<<endl;
    *arr = new int*[w];
    for(int row=0;row<w;row++){
        (*arr)[row] = new int[h];
        for(int col=0;col<h;col++){
            (*arr)[row][col] = getGrayPixel(row,col);
//            cout<<(*arr)[row][col]<<",";
        }
    }
//    cout<<endl;
}
