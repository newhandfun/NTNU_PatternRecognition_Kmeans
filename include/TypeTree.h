#ifndef TYPETREE_H
#define TYPETREE_H

#include "LinkList.h"

#include <vector>

using namespace std;


template <typename T>
class TypeTree{
    public:
        TypeTree(){}
        ~TypeTree(){
            //delete each type.
            for(unsigned i=0;i<childrens.size();i++){
                auto ptr = childrens[i] ;
                while(ptr!=nullptr){
                    auto ptr_next = ptr->getNext();
                    delete ptr;
                    ptr = ptr_next;
                }
            }
        }

        LinkList<T>* getRoot(unsigned int type){
            if(type >= childrens.size())return nullptr;
            return childrens[type];
        }

        void addType(LinkList<T>* new_root){
            childrens.push_back(new_root);
        }

        void addType(T* new_root_value){
             childrens.push_back(new LinkList<T>(new_root_value));
        }

        bool addTypeChildren(LinkList<T>* new_node,unsigned int index){
            if(index >= childrens.size())return false;
            childrens[index]->value->next = new_node;
            return true;
        }

        unsigned int getTypeNumber(){
            return childrens.size();
        }
    protected:
        vector<LinkList<T>*> childrens;
};
#endif // TYPETREE_H
