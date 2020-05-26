#include "UniqueList.hpp"

using namespace std;

UniqueList::UniqueList()
{
    intl = new vector<int>();
}

UniqueList::~UniqueList()
{
    //dtor
}

bool UniqueList::contains(int value)
{
    for (int i = 0; i < intl->size(); ++i) {
        if (intl->at(i) == value) return true;
    }
    return false;
}

bool UniqueList::add(int value)
{
    if (this->contains(value)) {
        return false;
    } else {
        intl->push_back(value);
        return true;
    }
}

bool UniqueList::rem(int value)
{
    for (int i = 0; i < intl->size(); ++i) {
        if (intl->at(i) == value) {
            intl->erase(intl->begin() + i);
            return true; // there can not be more than one hit,
            // because the list is unique
        }
    }
    return false;
}

bool UniqueList::isEmpty()
{
    return intl->empty();
}

int UniqueList::getLast()
{
    if (!isEmpty()) {
        return intl->at(intl->size() - 1);
    }
    else {
        return -1;
    }
}

