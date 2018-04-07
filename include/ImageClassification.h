#ifndef IMAGECLASSIFICATION_H
#define IMAGECLASSIFICATION_H

#include "Image.h"

#include <string.h>

#include <iostream>
using namespace std;


class ImageClassification
{
    public:
        ImageClassification();
        ~ImageClassification();

        int classfication(Image* img){
            //file name
            char* name_img_file = img->getFileName();
            //
            char* ptr_ch_start = nullptr;
//            cout<<"find img : "<<(*name_img_file)<<endl;
            for(unsigned int index_type_name=0;index_type_name<6;index_type_name++){
//                cout<<"type :"<<index_type_name<<" - "<<type_names[index_type_name]<<endl;
                ptr_ch_start = strstr(name_img_file,type_names[index_type_name]);
                if(ptr_ch_start!=nullptr)
                    return index_type_name;
            }
            return -1;
        }


        const char *type_names[6] = {
            "airplane",
            "pyramid",
            "sailing",
            "starfish",
            "sunflower",
            "tower"};
};

#endif // IMAGECLASSIFICATION_H
