#include <fstream>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "CImgImage.h"
#include <vector>

#include "LinkList.h"
#include "TypeTree.h"
#include "ImageClassification.h"

using namespace std;

/*****      use to limit data number    ****/
#define MAX_NUMBER 100000

/***        use to store images and its tag.        ***/
template class LinkList<CImgImage>;
vector<CImgImage> images;
vector<int> index_tab_type;

void getHistogram256(int **pixels,int w,int h,int* his_256){
//    cout<<"histogram : [";
    for(int r=0;r<w;r++){
        for(int c=0;c<h;c++){
//            cout<<pixels[r][c]<<',';
            his_256[pixels[r][c]] ++;
        }
    }
//    cout<<"]"<<endl;
}

void getGrayHistogram(Image *img,int* feature){
    int** ptr_pixel = nullptr;
    img->getPixelGrayArray(&ptr_pixel);
    if(feature==nullptr)feature=new int[256];
    for(unsigned int index=0;index<256;index++)feature[index] = 0;
    getHistogram256(ptr_pixel,img->getWidth(),img->getHeight(),feature);
    img->deleteArray(&ptr_pixel);
}

float findHistrogramFeature(Image *img,int* feature){
    float result = 0;
    feature = new int[256];
    getGrayHistogram(img,feature);
    for(int index=0;index<256;index++){
        result += feature[index];
    }
    delete[] feature;
    return result;
}

float findGrayFeatureDiff(const int* feature_1,const int* feature_2){
    float result  = 0;
    for(int index=0;index<256;index++){
         float new_value = (feature_1[index] - feature_2[index]);
         new_value *= new_value;
         result += (float)new_value;
    }
    return sqrt(result);
}

float findFeatureDiff(Image* orginImg,Image* newImg){

    int orgin_width = orginImg->getWidth(),orgin_height = orginImg->getHeight();
    int new_width = newImg->getWidth(),new_height = newImg->getHeight();

    int **gray_pixels_orgin = nullptr;
    int **gray_pixels_new = nullptr;
    orginImg->getPixelGrayArray(&gray_pixels_orgin);
    newImg->getPixelGrayArray(&gray_pixels_new);


    int his_orgin[256]={0},his_new[256]={0};
    getHistogram256(gray_pixels_orgin,orgin_width,orgin_height,his_orgin);
    getHistogram256(gray_pixels_new,new_width,new_height,his_new);

    float result = 0;
    for(int i=0;i<256;i++){
        char dif_orgin = his_new[i] - his_orgin[i];
        if(dif_orgin < 0)dif_orgin *= -1;
        dif_orgin += dif_orgin;
        result += (((float)(dif_orgin)));
    }
    result = sqrt(result);

//    cout<<"img "<<newImg->getFileName()<<" diff is "<<result<<endl;

    return result;
}

void deleteFeature(char* feature){
    for(int index=0;index<256;index++){
        delete[] feature;
    }
}

void coutResult(const vector<vector<int>>& index_type){
    cout<<"===============*************=============="<<endl;
    for(unsigned int type=0;type<index_type.size();type++){
        cout<<"********************************************"<<endl;
        cout<<"Type "<<type<<"....... "<<index_type[type].size()<<endl;
        for(unsigned int index=0;index<index_type[type].size();index++){
            CImgImage* img = &images[index_type[type][index]];
            cout<<img->getFileName()<<endl;
        }
        cout<<"********************************************"<<endl;
    }
    cout<<"===============*************=============="<<endl;
}

float verifyCorrection(vector<vector<int>> &type_index,const unsigned int length_data){

    float rate_correction = 0;
    //store [my type index]:[correction type index]
    unsigned int** sum_type = new unsigned int*[type_index.size()];
    //initialize sum_type;
    for(unsigned int index=0;index<type_index.size();index++){
        sum_type[index] = new unsigned int [type_index.size()];
        for(unsigned int index_item=0;index_item<type_index.size();index_item++){
            sum_type[index][index_item] = 0;
        }
    }

    cout<<"My Type \\ True Type table"<<endl;
    //for each member of type
    for(unsigned int index_my_type=0;index_my_type<type_index.size();index_my_type++)
    {
        cout<<index_my_type<<" : "<<endl;
        unsigned int length_current_member = type_index[index_my_type].size();
        for(unsigned int index_current_member=0;index_current_member<length_current_member;index_current_member++)
        {
            //correct type number
            unsigned int correct_type = index_tab_type[type_index[index_my_type][index_current_member]];
            sum_type[index_my_type][correct_type] ++;
        }
        for(unsigned int index_true_type=0;index_true_type<type_index.size();index_true_type++){
            cout << '\t' << sum_type[index_my_type][index_true_type];
        }
        cout<<endl;
    }

    int number_best_type_assign = 0;

    bool *is_type_used = new bool[type_index.size()];
    for(unsigned int index=0;index<type_index.size();index++){
        is_type_used[index] = false;
    }

    cout<<"Type Comparison."<<endl;
    //for each true type;
    for(unsigned int index_true_type=0;index_true_type<type_index.size();index_true_type++){
        //find closest type of my type
        unsigned int index_max_type = 0;
        unsigned int number_correct = 0;
        for(unsigned int index_my_type=0;index_my_type<type_index.size();index_my_type++){
            if(number_correct <= sum_type[index_my_type][index_true_type] && !is_type_used[index_my_type]){
                number_correct = sum_type[index_my_type][index_true_type];
                index_max_type = index_my_type;
            }
        }
        is_type_used[index_max_type] = true;
        number_best_type_assign += number_correct;
        cout<<"True Type "<<index_true_type<<" : My Type "<<index_max_type<<" ---Correct Number "<< number_correct <<endl;
    }

    //calculate
    rate_correction = (float)number_best_type_assign/(float)images.size();

    cout<<"Correction Rate = "<<rate_correction<<endl;

    //free memory
    for(unsigned int index=0;index<type_index.size();index++){
        delete [] sum_type[index];
    }
    delete [] sum_type;
    delete [] is_type_used;

    return rate_correction;
}

//float (*FindFeatureDiff)(const Image* image_1,const Image image_2);

float kmeans(vector<vector<int>> &type_index,int** feature_vectors,const unsigned int type_number
    ,const unsigned int orgin_data_size,vector<int*> &centroids,float error_allow){

    //store img feature vectors
    feature_vectors = new int*[orgin_data_size];
    for(unsigned int index =0;index<orgin_data_size;index++){
        int* feature = new int[256];
        getGrayHistogram(&images[index],feature);
//        cout<<"feature address of img "<<index<<" is "<<&images[index]<<endl;
        feature_vectors[index] = feature;
//        cout<<"feature of img "<<index<<" at 126 is "<<feature_vectors[index][126]<<endl;
    }

    //those parameter use to check is number selected.
    bool is_used[orgin_data_size] = {0};
    vector<int> usable_index ;                                  //push usable index to vector

    cout<<"start loop....."<<endl;
    //initial values that check if img used.
    for(unsigned int i=0;i<orgin_data_size;i++){is_used[i]=false;}
    for(unsigned int index=0;index<orgin_data_size;index++)
        usable_index.push_back(index);

    for(unsigned int type=0;type<type_number;type++){

        cout<<"type "<<type<<" ."<<endl;

        // if data are used up,classification is over.
        if(usable_index.size()==0){
            continue;
        }

        //if type or centroid of this type is empty, create new one.
        if( type_index.size()<=type){
            //create a new type in tree
            vector<int> type_member;
            type_index.push_back(type_member);

        }
        if(centroids.size()<=type){
            //add new int array into centroids
            centroids.push_back(new int[256]);
        }
        //if no item in old type,add an item in this type.
        if(type_index[type].size()==0){

            unsigned int type_re_index = 0;

            //find a non-used image,then select it to be centoriods.
//            type_re_index = usable_index[rand()%usable_index.size()];

            //find a none-used image that has much difference with other centroids.
            unsigned int times_test = 0;
            unsigned int times_max_test = orgin_data_size/type_number ;
            do{
                type_re_index = usable_index[rand()%usable_index.size()];
                if(type==0)break;
            }while(
                findGrayFeatureDiff(feature_vectors[type_re_index],centroids[type-1])<4000   ||
                times_test++ < times_max_test
            );

            auto iter_usable_index = find(usable_index.begin(),usable_index.end(),type_re_index);

            cout<<"this type is not data.we will indicate one to be centroid. select index : ";
            cout<<type_re_index<<endl;

            //sign used item.
            is_used[type_re_index] = true;
            usable_index.erase(iter_usable_index);

            type_index[type].push_back(type_re_index);
        }else{
            cout<<"this type have "<<type_index[type].size()<<" data,we will average their feature value to be centroid."<<endl;
        }

//        cout<<"Get centroid by color:[";
        for(unsigned int index_color=0;index_color<256;index_color++){
            unsigned int value_sum = 0;
            for(unsigned int index_type_item=0;index_type_item<type_index[type].size();index_type_item++){
                unsigned int index_item = type_index[type][index_type_item];
                value_sum += feature_vectors[index_item][index_color];
//                cout<<feature_vectors[index_type_item][index_color]<<",";
            }
            centroids[type][index_color] = value_sum / type_index[type].size();
        }
//        cout<<")"<<endl;
        //clear data in old type classification
        type_index[type].clear();
    }
    for(unsigned int index_img=0;index_img<orgin_data_size;index_img++){
        if(is_used[index_img]){
            continue;
        }

        float diff_best_type_with_centroid = 99999;
        unsigned int index_best_type = 0;
//        cout<<"img "<<index_img<<" : "<<endl;
        for(unsigned int index_type=0;index_type<type_number;index_type++){
            float diff_this_type_with_centroid
                = findGrayFeatureDiff( feature_vectors[index_img] , centroids[index_type] );
//            cout<< &feature_vectors[index_img][0]<<"/"<< &centroids[index_type][0]<<"  , ";
            if(diff_this_type_with_centroid<diff_best_type_with_centroid){
                diff_best_type_with_centroid = diff_this_type_with_centroid;
                index_best_type = index_type;
            }
        }

//        cout<<endl<<"img "<<index_img<<" - centroid "<<index_best_type<<" = "<<diff_best_type_with_centroid<<endl;

        type_index[index_best_type].push_back(index_img);

        is_used[index_img] = true;
    }

    for(unsigned int i=0;i<type_index.size();i++){
        cout<<"type "<<i<<" has "<<type_index[i].size()<<" items."<<endl;
    }

}

ImageClassification imageClassification;

void ImgLoad(vector<CImgImage> &imgs){
    char imgName[MAX_NUMBER];                       //store name;
//    int max_image_number = 1048;
    ifstream inFile("pattern_list");                //read file
    while (inFile.getline(imgName,MAX_NUMBER)
//        && imgs.size()<100
        ) {    //when read line,do this
        //create img
        CImgImage  newImage;                        //create new image
        newImage.loadImage(imgName);                //read image by file name
        imgs.push_back(newImage);                 //add image to images
    }
    inFile.close();
}

void imgClusting(vector<CImgImage> &imgs,vector<int> &tags){
    vector<vector<int>> type_index;
    vector<int*> centroids;
    float error_allow = 0.1;
    float error_fact = 1;
    int** ptr_features = nullptr;
    while( error_fact > error_allow ){
        cout<<"Enter any key to contiune....."<<endl;
        getchar();
        kmeans(type_index,ptr_features,6,images.size(),centroids,error_allow);
        error_fact = 1 - verifyCorrection(type_index,images.size());
    }
    for(unsigned int i=0;i<images.size();i++){
        delete[] ptr_features[i];
    }
    for(unsigned int i=0;i<6;i++){
        delete[] centroids[i];
    }
    delete [] ptr_features;
}

void imgTag(vector<CImgImage> &imgs,vector<int> &tags){
    int index = 0 ;
    while (index < imgs.size()) {    //when read line,do this
        int index_type = imageClassification.classfication(&imgs[index]);
        tags.push_back(index_type);
//        cout<<"img "<<(images.size()-1)<<" : tag : "<<index_type<<endl;
        index++;
    }
}

int main()
{
    srand (time(NULL));
    ImgLoad(images);
    imgTag (images,index_tab_type);
    imgClusting(images,index_tab_type);
	return 0;
}

typedef LinkList<CImgImage> LinkImage;
typedef TypeTree<CImgImage> ImageType;


void testLinkList(){
    int index  = 1;
    LinkList<int>* ll  = nullptr;
    //link 100 node(parent to parent)
    for(index=1;index<101;index++){
        auto nll = new LinkList<int>(new int(index));
        if(ll!=nullptr){
            ll->setParent(nll);
        }
        ll = nll;
        cout << "Node "<< index  << ":" << " , current link number :"   <<ll->getLinkNumber() << endl;
    }
    //get the first node
    while(ll->getNext()!=nullptr){
        auto nll = ll->getNext();
        ll = nll;
    }
    //log then delete each node
    while(ll!=nullptr){
        auto nll = ll->getParent();
        cout << "value is " << *ll->value << endl;
        delete ll;
        ll = nll;
    }
    //pause
    char a;
    cin>>a;
}

class TagedLinkImage{
    public:
        LinkImage* link_image;
        float tag ;

        static bool compare(TagedLinkImage a,TagedLinkImage b){return a.tag < b.tag; }
};

void kmeans(ImageType &type_tree,const unsigned int type_number,const unsigned int orgin_data_size,const float min_diff){
    if(orgin_data_size<type_number)
        return ;
    if(min_diff>1)
        return ;
    //data link list and the head of it(s)
    LinkImage *ptr_data = nullptr;
    LinkImage *ptr_head = nullptr;
    //record number of data in list
    unsigned int data_size = orgin_data_size;
    //create the link list of datas
    for(unsigned int i = 0;i<data_size;i++){
        LinkImage* new_datium = new LinkImage(&images[i]);       //create a new linklist node
        if(ptr_head!=nullptr){new_datium->setParent(ptr_data);}//if it is not first one,set it as children of last one.
        ptr_data = new_datium;                                  //save it
        if(ptr_head==nullptr){ptr_head = new_datium;}         //if it is first one,it is head
        if(ptr_data->getNext()!=nullptr)ptr_data = ptr_data->getNext();                              //
    }
    while(ptr_data->getParent()!=nullptr){
        cout<< &(*(ptr_data->value))<<" : "<<((void*) ptr_data->value->getFileName())<<endl;
        ptr_data = ptr_data->getParent();
    }
    for(unsigned int type_index=0;type_index<type_number||data_size!=0;type_index++,data_size--){
        cout<<"create type....."<<type_index<<endl;
        //create root node of link list
        unsigned int type_index_number = rand()%data_size;  //random get img
        //find img of index
        ptr_data=ptr_head;
        cout<<"get random number....."<<type_index_number<<endl;
//        cout<<"head is "<<ptr_head<<endl;
        for(unsigned int index = 0;
            index!=type_index_number;
            index++){
            ptr_data = ptr_data->getNext();
            cout<<ptr_data<<endl;
        }
        LinkImage *root = ptr_data;
        type_tree.addType(root);                            //add type to tree
        if(type_index_number==0){ptr_head = root->getNext();}//if head is root,second will be head.
        root->deleteFromList();                             //delete root node from list
        cout<<"get root data....."<<root->value->getFileName()<<endl;

        //get feature difference of all images .
        vector<TagedLinkImage> feature_diffs;                                 //store feature difference
        ptr_data = ptr_head ;                                   //set ptr to head
//        cout<<"get feature difference...ptr_data:"<<ptr_data<<" ptr_head:"<<ptr_head<<endl;
        while(ptr_data!=nullptr){
            cout<<"ptr_data = "<<ptr_data->value->getFileName()<<endl;
            //tag this image
            TagedLinkImage tageLinkImage;
            tageLinkImage.link_image = ptr_data;
            tageLinkImage.tag = findFeatureDiff(root->value,ptr_data->value);
            feature_diffs.push_back(tageLinkImage);
            //point next data
            ptr_data = ptr_data->getNext();
            cout<<ptr_data<<endl;
        }
        //sort by feature difference.
        sort(feature_diffs.begin(),feature_diffs.begin()+data_size,TagedLinkImage::compare);

//        cout<<"feature diffs number is "<<feature_diffs.size()<<endl;

        ptr_data = root;
        cout<<"add close image to this type....."<<endl;
        if(type_index + 1 == type_number){
            int index=0;
            while(ptr_data!=nullptr){
                ptr_data->setNext(feature_diffs[index++].link_image);
                ptr_data = ptr_data->getNext();
            }
            return;
        }

        for(unsigned int diff_index=0;feature_diffs[diff_index].tag <= min_diff;diff_index++){
            cout<<feature_diffs[diff_index].link_image->value->getFileName()<<endl;
//            //delete node from orgin linklist then put it in type set.
//            feature_diffs[diff_index].link_image->deleteFromList();
//            ptr_data->setNext(feature_diffs[diff_index].link_image);
//            ptr_data = ptr_data->getNext();
//            diff_index--;

        }

    }
}

void kmeans(vector<vector<Image*>> &type_tree,const unsigned int type_number
    ,const unsigned int orgin_data_size,const float min_diff){

    //those parameter use to check is number selected.
    bool is_used[orgin_data_size] = {0};                        //check is index of image used
    for(unsigned int i=0;i<orgin_data_size;i++){is_used[i]=false;}

    vector<int> usable_index ;                                  //push usable index to vector
    for(unsigned int index=0;index<orgin_data_size;index++)
        usable_index.push_back(index);

    //start algorithm
    for(unsigned int type=0;type<type_number;type++){

        cout<<"type "<<type<<" .start"<<endl;

        if(usable_index.size()==0){
            cout<<"number is used over."<<endl;
            return;
        }

        //find a non-used image,then select it to be type root
        int type_re_index = usable_index[rand()%usable_index.size()];
        auto iter_usable_index = find(usable_index.begin(),usable_index.end(),type_re_index);

        cout<<"type "<<type<<" : "<<"index"<<type_re_index<<","<<" image remain : "<<usable_index.size()<<endl;
        is_used[type_re_index] = true;
        usable_index.erase(iter_usable_index);
        cout<<"image remain : "<<usable_index.size()<<endl;

        //create a new root in tree
        vector<Image*> type_member;
        type_member.push_back(&images[type_re_index]);
        type_tree.push_back(type_member);

        //find feature difference with other image to classification
        for(unsigned int index=0;index<orgin_data_size;index++){
            if(!is_used[index]){
                Image* ptr_image = &images[index];
                float feature_diff = findFeatureDiff(type_tree[type][0],ptr_image);
                if(feature_diff<min_diff){
                    cout<<index<<":"<<feature_diff<<endl;
                    is_used[index] = true;
                    usable_index.erase(find(usable_index.begin(),usable_index.end(),index));
                    cout<<usable_index.size()<<endl;
                    type_tree[type].push_back(ptr_image);
                }
            }
        }
    }
    if(usable_index.size()==0)
        return;
    cout<<"usable image remian "<<usable_index.size()<<"."<<endl;
    //classificate remaining image
    while(usable_index.size()!=0){
        unsigned int img_index = usable_index[usable_index.size() - 1];
        CImgImage* ptr_image = &images[img_index];
//        cout<<"image "<<img_index<<": start classification."<<endl;

        //find closest type by centro
        int cloest_type = 0;
        float min_dis = 0;
        for(unsigned int type=0;type<type_number;type++){
            float dis = findFeatureDiff(type_tree[type][0],ptr_image);
//            cout<<"type "<<type<<" distance is "<<dis<<endl;
            if(dis<min_dis){
                cloest_type = type;
                dis = min_dis;
            }
        }

        type_tree[cloest_type].push_back(&images[usable_index[img_index]]);
        usable_index.pop_back();
    }
}
