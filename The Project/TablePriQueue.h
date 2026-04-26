#pragma once
#include "LinkedPriorityQueue.h"
#include"PriNode.h"
template<typename T>
class TablePriQueue : public LinkedPriorityQueue<T>
{
public:
    bool getBest(int requiredCapacity, T& bestTable)
    {
        if (this->isEmpty()) return false;

        PriNode<T>* prev = nullptr;
        PriNode<T>* curr = this->frontPtr;

        while (curr != nullptr)
        {
            if (curr->getItem()->getCapacity() >= requiredCapacity)
            {
                bestTable = curr->getItem();

                if (prev == nullptr)
                {
                    this->frontPtr = curr->getNext();
                }
                else
                {
                    prev->setNext(curr->getNext());
                }
				// If we removed the last node, update backPtr
                if (curr->getNext() == nullptr) {
                    this->backPtr = prev;
                }
                delete curr;
                this->count--;
                return true;
            }

            prev = curr;
            curr = curr->getNext();
        }

        return false;
    }

};