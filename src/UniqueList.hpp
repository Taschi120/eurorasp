#ifndef __INCLUDE_UNIQUELIST_HPP_
#define __INCLUDE_UNIQUELIST_HPP_

#include <vector>

using namespace std;

class UniqueList
{
    public:
        UniqueList();
        virtual ~UniqueList();

        bool contains(int value);
        bool add(int value);
        bool rem(int value);
        bool isEmpty();
        int getLast();

    protected:

    private:
        vector<int> *intl;

};

#endif // __INCLUDE_UNIQUELIST_HPP_
