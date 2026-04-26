#pragma once
#include "LinkedPriorityQueue.h" // Or the appropriate header file name for your Priority Queue

template<typename T>
class CancelPriQueue : public LinkedPriorityQueue<T>
{
public:
    bool CancelOrder(int ID, T& returnedItem)
    {
        if (this->isEmpty())
            return false;

        // Case 1: If the order to be cancelled is the first order in the queue
        if (this->frontPtr->getItem()->Get_ID() == ID)
        {
            PriNode<T>* pdel = this->frontPtr;
            returnedItem = pdel->getItem();

            this->frontPtr = this->frontPtr->getNext();

            delete pdel;

            this->count--; // Decrement the count of elements
            return true;
        }

        // Case 2: If the order is in the middle or at the end of the queue
        PriNode<T>* ptr = this->frontPtr;
        PriNode<T>* pdel = nullptr;

        while (ptr->getNext() != nullptr)
        {
            if (ptr->getNext()->getItem()->Get_ID() == ID)
            {
                pdel = ptr->getNext();
                returnedItem = pdel->getItem();

                // Bypass the node to be deleted
                ptr->setNext(pdel->getNext());

                delete pdel;

                this->count--; // Decrement the count of elements
                return true;
            }

            ptr = ptr->getNext();
        }

        return false;
    }
};