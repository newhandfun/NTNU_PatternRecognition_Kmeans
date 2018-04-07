#ifndef LINKLIST_H
#define LINKLIST_H

template <typename T>
class LinkList
{
    public :

        typedef void (*function_ptrll_void)(LinkList<T>* node);

        LinkList(){onStart();}
        LinkList(T* new_value){value = new_value;onStart();}

        ~LinkList(){
            deleteFromList();
            delete value;
            if(*link_number==1){
                delete link_number;
            }
        }

        T* value ;

        //set next node
        void setNext(LinkList<T>* new_next){
            if(new_next==nullptr)return;                //if next node is null,nothing happen
            next = new_next;                            //set next node
            new_next->parent = this;                  //let next node's parent point this.
            new_next->setLinkNumber(link_number);      //let next node's number common with this
            *link_number += 1;                          //node number +1;
        }
        void setParent(LinkList<T>* new_parent){
            if(new_parent==nullptr)return;              //if parent node is null,nothing happen
            parent = new_parent;                        //set parent node
            new_parent->next = this;                  //let parent node's next point this
            new_parent->setLinkNumber(link_number);     //let parent node number common with this
            *link_number += 1;                          //node number +1;
        }

        void deleteFromList(){
            if(next!=nullptr && parent != nullptr){       //if next and parent are fill,
                next->parent = parent;                    //point next's parent to my parent
                parent->next = next;                       //point parent's next to my next
            }else
            if(next!=nullptr && next->parent!=nullptr){
                next->parent = nullptr;
            }else
            if(parent!=nullptr&&parent->next!=nullptr){
                parent->next = nullptr;
            }
            parent = nullptr;                              //clear parent & next
            next = nullptr;
            onDetach();                                 //let next execute function on detach
            *link_number -= 1;                          //node number -1
        }

        bool isNext(){return next!=nullptr ;}
        bool hasParent(){return parent!=nullptr;}

        LinkList<T>* getNext(){return next;}
        LinkList<T>* getParent(){return parent;}

        void setLinkNumber(unsigned int* ptr_number){
            if(ptr_number==nullptr)return;
            delete link_number;
            link_number = ptr_number;
        }
        unsigned int getLinkNumber(){return *link_number;}

        void onDetach(){
            if(link_number!=nullptr) link_number = nullptr;
            link_number = new unsigned int(1);
        }

        void DoWhenTraversalNext(function_ptrll_void func){
            LinkList<T>* node = this;
            do{
                func(node);
                node = node->getNext();
            }while(node!=nullptr);
        }
        void DoWhenTraversalParent(function_ptrll_void func){
            LinkList<T>* node = this;
            do{
                func(node);
                node = node->getParent();
            }while(node!=nullptr);
        }
    protected  :
        LinkList<T>* next = nullptr;
        LinkList<T>* parent = nullptr;
        unsigned int *link_number = nullptr;
    private    :
        void onStart(){ onDetach();}

};

#endif // LINKLIST_H
