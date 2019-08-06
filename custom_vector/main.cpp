
#include "Vector.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

class trywialna
{
    public:
    int x = 0;
    int z = 4;
};
class test
{
    

public:
    
    test(const int &i, const int &x, const std::string& z)
    {
        text = z;
    }
    test(const test& other)
    {
      //  std::cout << "czemu kurwa to jest wywolywane" <<std::endl;
        text = other.text;
    }
   void print()
    {
        std::cout << text << std::endl;
    }
      test &operator=(const test &t)

    {
      //  std::cout<<"operator przypisania" << std::endl;
        text = t.text;
      //  std::cout<<"operator przypisania 2" << std::endl;
        return *this;
    }
    
     test &operator=(test &&t)
    {
        text = t.text;
        return *this;
    }
    
   
  private:
    std::string text;
};

template <class U, class Z>
void testContainerPushBack(U container, std::string type, Z object, int elements = 100000000)
{
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < elements; i++)
    {
        container.push_back(object);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<long double> elapsed_seconds = end - start;
    std::cout << "Typ: " << type << "\t probka: " << elements << "\t\t time:" << elapsed_seconds.count() << "\n";
}

template <class U>
void testContainerInsert(U container, std::string type, int elements = 10000)
{
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < elements; i++)
    {
        container.insert(container.cbegin(), test(1,2,"test123"));
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<long double> elapsed_seconds = end - start;
    std::cout << "Typ: " << type << "\t probka: " << elements << "\t\t time:" << elapsed_seconds.count() << "\n";
}

template <class U>
void testContainerInsertInt(U container, std::string type, int elements = 10000)
{
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < elements; i++)
    {
        container.insert(container.cbegin(), i);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<long double> elapsed_seconds = end - start;
    std::cout << "Typ: " << type << "\t probka: " << elements << "\t\t time:" << elapsed_seconds.count() << "\n";
}

template <class U>
void testContainerEmplaceBack(U container, std::string type, int elements = 10000)
{
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < elements; i++)
    {
        container.emplace_back(1, 2, "test"+1);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<long double> elapsed_seconds = end - start;
    std::cout << "Typ: " << type << "\t probka: " << elements << "\t\t time:" << elapsed_seconds.count() << "\n";
}

int main()
{

    typedef std::chrono::system_clock timer;

    std::vector<test> vv;
    Vector<test> v;
    std::vector<int> emplaceA;
    Vector<int> emplaceB;
    //std::vector<trywialna> ii;
   // Vector<trywialna> iii;

    //std::cout << " ------------------------------------------- " << std::endl;
  /*    for(int i =1; i<10000000; i*=2)
    {

    testContainerPushBack(vv, "A\t",  test(1,2,"666") ,i);
    testContainerPushBack(v, "B\t", test(1,2,"666")  ,i);
    */
/*for(int i =1; i<10000000; i*=2)
    {
     testContainerEmplaceBack(vv, "A\t" ,i);
     testContainerEmplaceBack(v, "B\t", i);

    }
*/

/*for(int i =0; i<1000; i++)
{
    v.push_back(test(1,2,"555"));
}*/

/*for(int i =1; i<20000; i*=2)
{
    testContainerInsert(vv, "A\t",i);
    testContainerInsert(v, "B\t",i);

}*/

for(int i =1; i<40000; i*=2)
{
    testContainerInsertInt(emplaceA, "A\t",i);
    testContainerInsertInt(emplaceB, "B\t",i);

}

for(int i = 0; i< 100; i++)
{
emplaceB.push_back(std::move(i));
}



emplaceB.insert(emplaceB.cbegin(),555);
emplaceB.insert(emplaceB.cbegin(),666);
emplaceB.insert(emplaceB.cbegin(),777);
emplaceB.insert(emplaceB.cbegin(),888);
emplaceB.insert(emplaceB.cbegin(),999);

for(auto x : emplaceB)
{
    std::cout<<x<<std::endl;
}

Vector<int>::const_iterator xxx(&emplaceB[10]);

emplaceB.insert(xxx,555555555);

for(auto x : emplaceB)
{
    std::cout<<x<<", ";
}
std::cout<<std::endl;


/*Vector<test>::const_iterator x(&v[100]);
v.insert(x,test(1,1,"234"));
v.insert(x,test(1,1,"234"));
v.insert(x,test(1,1,"234"));
v.insert(x,test(1,1,"234"));*/

    return 0;
}