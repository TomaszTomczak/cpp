
#include "List.hpp"
#include <iostream>
#include <list>


int main()
{
    std::cout<<"hello"<<std::endl;
    List<int> lista; 
    //std::list<int>::iterator::op


    lista.push_back(2);
    lista.push_back(5);

    for(auto i : lista)
    {
        std::cout<<i<<std::endl;
    }

    return 0;
}