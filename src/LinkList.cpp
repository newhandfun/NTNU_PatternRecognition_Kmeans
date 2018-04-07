#include "LinkList.h"

//template <typename T> LinkList<T>::LinkList()
//{
//    onStart();
//}
//
//template <typename T> LinkList<T>::LinkList(T* new_value)
//{
//    value = new_value;
//    onStart();
//}
//
//template <typename T> LinkList<T>::~LinkList()
//{
//    deleteFromList();
//    delete value;
//    if(*link_number==1){
//        delete link_number;
//    }
//}
//
//template <typename T> void LinkList<T>::setNext(LinkList<T>* new_next){
//    if(new_next==nullptr)return;                //if next node is null,nothing happen
//    next = new_next;                            //set next node
//    new_next->parent = this;                  //let next node's parent point this.
//    new_next->setLinkNumber(link_number);      //let next node's number common with this
//    *link_number += 1;                          //node number +1;
//}
//
//template <typename T> void LinkList<T>::setParent(LinkList<T>* new_parent){
//    if(new_parent==nullptr)return;              //if parent node is null,nothing happen
//    parent = new_parent;                        //set parent node
//    new_parent->next = this;                  //let parent node's next point this
//    new_parent->setLinkNumber(link_number);     //let parent node number common with this
//    *link_number += 1;                          //node number +1;
//}
//
//template <typename T> void LinkList<T>::deleteFromList(){
//            if(next!=nullptr && parent != nullptr){       //if next and parent are fill,
//                next->parent = parent;                    //point next's parent to my parent
//                parent->next = next;                       //point parent's next to my next
//            }else
//            if(next!=nullptr && next->parent!=nullptr){
//                next->parent = nullptr;
//            }else
//            if(parent!=nullptr&&parent->next!=nullptr){
//                parent->next = nullptr;
//            }
//            parent = nullptr;                              //clear parent & next
//            next = nullptr;
//            onDetach();                                 //let next execute function on detach
//            *link_number -= 1;                          //node number -1
//        }
//
//template <typename T> bool LinkList<T>::isNext(){
//    return next!=nullptr;
//}
//
//template <typename T> bool LinkList<T>::hasParent(){
//    return parent!=nullptr;
//}
//
//template <typename T> LinkList<T>* LinkList<T>::getNext(){return next;}
//template <typename T> LinkList<T>* LinkList<T>::getParent(){return parent;}
//
//template <typename T> void LinkList<T>::setLinkNumber(unsigned int* ptr_number){
//    if(ptr_number==nullptr)return;
//    delete link_number;
//    link_number = ptr_number;
//}
//template <typename T> unsigned int LinkList<T>::getLinkNumber()
//{return *link_number;}
//
//template <typename T> void LinkList<T>::onDetach()
//{
//    if(link_number!=nullptr) link_number = nullptr;
//    link_number = new unsigned int(1);
//}
