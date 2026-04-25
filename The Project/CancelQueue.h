#pragma once
#include "LinkedQueue.h"
template<typename T>
class CancelQueue : public LinkedQueue<T>
{
public:
    bool CancelOrder(int ID, T& returnedItem)
    {
        if (this->isEmpty())
            return false;

        if (this->frontPtr->getItem()->Get_ID() == ID)
        {
            Node<T>* pdel = this->frontPtr;

            returnedItem = pdel->getItem();

            this->frontPtr = this->frontPtr->getNext();

            if (this->frontPtr == nullptr)
                this->backPtr = nullptr;

            delete pdel;
            this->count--;
            return true;
        }

        Node<T>* ptr = this->frontPtr;
        Node<T>* pdel = nullptr;

        while (ptr->getNext() != nullptr)
        {
            if (ptr->getNext()->getItem()->Get_ID() == ID)
            {
                pdel = ptr->getNext();

                returnedItem = pdel->getItem();

                ptr->setNext(pdel->getNext());

                if (pdel == this->backPtr)
                {
                    this->backPtr = ptr;
                }

                delete pdel;
                this->count--;
                return true;
            }

            ptr = ptr->getNext();
        }

        return false;
    }


};