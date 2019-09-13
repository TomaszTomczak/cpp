
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
    int c = 4;

};
class test
{


public:
    test(int i, int x, std::string z)
    {
        
        text = z + "dupa" + std::to_string(i);
        //text += i;
        c = i;
        q1 += std::to_string(i);
        //std::cout<<"new test at: "<<static_cast<void*>(this)<<std::endl;
    }
    
    test(const test& other)
    {
        
        q1 = other.q1; 
        q2 = other.q2;
        q3= other.q3;
        q4 = other.q4;
        q5 = other.q5;
        q6 = other.q6;
        var1 = other.var1;
        var2 = other.var2;
        var3 = other.var3;
        text = other.text;
        c= other.c;
      //  std::cout<<"copy ctor: "<<text<<" object is under address: "<<static_cast<void*>(this)<<", copied from addres: "<<static_cast<const void*>(&other)<<std::endl;
    }
   ~test()
    {
     //  std::cout<<"removing: "<<text<<" under address: "<<static_cast<void*>(this)<<std::endl;
    }
// test(test&&) = default;
 test &operator=(const test &other)         //copy
  {
   q1 = other.q1; 
        q2 = other.q2;
        q3= other.q3;
        q4 = other.q4;
        q5 = other.q5;
        q6 = other.q6;
        var1 = other.var1;
        var2 = other.var2;
        var3 = other.var3;
        text = other.text;
        c= other.c;
      //  std::cout<<"copy operator: "<<text<<" object is under address: "<<static_cast<void*>(this)<<", copied from addres: "<<static_cast<const void*>(&other)<<std::endl;
        return *this;

  }
    void print()
    {
        //std::cout<<"--";
        //std::cout<<"address in memory of this element is: "<<static_cast<void*>(this)<<std::endl;
       // std::cout<<"address to string: "<<static_cast<void*>(&text)<<std::endl;
        std::cout << text<<" i: "<<c<<std::endl ;
        std::cout << q1 << std::endl;
                std::cout<<"--"<<std::endl;
    }



private:
trywialna xxx;
std::string q1 = "LitwoOjczyznoTyMojaTyJestesJakZdrowie1";
std::string q2 = "LitwoOjczyznoTyMojaTyJestesJakZdrowie2";
std::string q3 = "LitwoOjczyznoTyMojaTyJestesJakZdrowie3";
std::string q4 = "LitwoOjczyznoTyMojaTyJestesJakZdrowie4";
std::string q5 = "LitwoOjczyznoTyMojaTyJestesJakZdrowie5";
std::string q6 = "LitwoOjczyznoTyMojaTyJestesJakZdrowie6";
uint64_t var1 = 555555555;
uint64_t var2 = 666666666;
uint64_t var3 = 777777777;
    std::string text = "dupa";
    int c;
};

template <class U, class Z>
void testContainerPushBack(U& container, std::string type, Z object, int elements = 100000000)
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
void testContainerInsert(U& container, std::string type, int elements = 10000)
{
    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < elements; i++)
    {
      // try
      //  {
        // std::cout<<"---inserting value :"<<i<<" container capacity: "<<container.capacity()<<" container size: "<<container.size()<<std::endl;
        // std::cout<<"---- underlaying buffer addrres: "<<static_cast<void*>(container.data())<<std::endl;
          container.insert(container.cbegin(), test(i, i, "test123"));
            
      //  }
      //  catch(const std::exception& e)
      //  {
     //       std::cerr << e.what() << '\n';
      //      break;
      //  }
        
       
    }


    auto end = std::chrono::system_clock::now();
    std::chrono::duration<long double> elapsed_seconds = end - start;
    std::cout <<std::endl<< "Typ: " << type << "\t probka: " << elements << "\t\t time:" << elapsed_seconds.count() << "\n";
   
   // std::cout<<std::endl;
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
        container.emplace_back(1, 2, "test" + 1);
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

    std::vector<int> stdIntTest;
    Vector<int> myIntTest;

    std::vector<int> emplaceA;
    Vector<int> emplaceB;


  /*  v.push_back(test(1,2,"1"));
    v.push_back(test(2,2,"2"));
    v.push_back(test(3,2,"3"));
    v.push_back(test(4,2,"4"));
    v.push_back(test(5,2,"5"));

for(auto x : v)
{
    x.print();
}

Vector<test>::const_iterator ite(&v[2]);

v.erase(ite);
 std::cout << " ---------------- erased --------------------------- " << std::endl;
for(auto x : v)
{
    x.print();
}*/
    //std::vector<trywialna> ii;
    // Vector<trywialna> iii;

 /*  std::cout << " ---------------- Object push back test --------------------------- " << std::endl;
    for (int i = 1; i < 40000; i *= 2)
    {

        testContainerPushBack(vv, "A\t", test(1, 2, "666"), i);
        testContainerPushBack(v, "B\t", test(1, 2, "666"), i);
    }*/


 /*std::cout << " -------------------Int push back test------------------------ " << std::endl;
   for (int i = 1; i < 100; i *= 2)
    {
        testContainerPushBack(stdIntTest, "A\t", i, i);
        testContainerPushBack(myIntTest, "B\t", i, i);
    }
*/
    std::cout << " -------------------insert object------------------------ " << std::endl;
        std::cout << "sizeof:  " <<sizeof(test)<< std::endl;
  //  for(int i =1; i<10; i++)
//{
        testContainerInsert(vv, "A\t",10);
        std::cout<<"--------------------------------------------------------------------B-"<<std::endl;
        testContainerInsert(v, "B\t",10);

//}

  std::cout << std::boolalpha;
  std::cout << "is_move_constructible:" << std::endl;
  std::cout << "test: " << std::is_move_constructible<test>::value << std::endl;
std::cout << "is_trivially_destructible:" << std::endl;
  std::cout << "test: " << std::is_trivially_destructible<test>::value << std::endl;
   std::cout << "is_trivially_move_constructible:" << std::endl;
  std::cout << "test " << std::is_trivially_move_constructible<test>::value << std::endl;

    std::cout << "is_move_constructible:" << std::endl;
  std::cout << "trywwialna: " << std::is_move_constructible<trywialna>::value << std::endl;
std::cout << "is_trivially_destructible:" << std::endl;
  std::cout << "trywialna: " << std::is_trivially_destructible<trywialna>::value << std::endl;
   std::cout << "is_trivially_move_constructible:" << std::endl;
  std::cout << "trywialna " << std::is_trivially_move_constructible<trywialna>::value << std::endl;
   std::cout << "is_trivially_move_copyable:" << std::endl;
    std::cout << std::is_trivially_copyable<test>::value << '\n';
        std::cout << std::is_trivially_copyable<trywialna>::value << '\n';

    std::cout << " -------------------print my vector------------------------ " << std::endl;
   
   Vector<test>::const_iterator ite(&v[2]);
      Vector<test>::const_iterator ite2(&v[4]);
   v.erase(ite,ite2);

for(auto x : v)
{
    x.print();
}

    std::cout << " -------------------print standard vector------------------------ " << std::endl;
for(auto x : vv)
{
    x.print();
}





  /* for (int i = 1; i < 2; i *= 2)
    {
     testContainerInsert(v, "B\t",  );
     testContainerInsert(vv, "A\t",  8192);
   
    }*/
   /* for(int i =0; i<=16; i++)
    {
        v.push_back(test(i,i,"dupa"));
        v[i].print();
        std::cout<<std::endl;
    }
    std::cout << " -------------------vector std------------------------ " << std::endl;

    for(int i =0; i<=16; i++)
    {
        vv.push_back(test(i,i,"dupa"));
        vv[i].print();
        std::cout<<std::endl;
    }

    std::cout<<"size of A: "<<vv.size()<<std::endl<<"size of B: "<<v.size()<<std::endl;
    std::cout<<"capacity of A: "<<vv.capacity()<<std::endl<<"capacity of B: "<<v.capacity()<<std::endl;
    std::cout<<"size of test: "<<sizeof(test)<<std::endl;
    //std::cout << " second data" << std::endl;
    //v[1].print();
   // Vector<test>::iterator it(&v[2048]);
    //test t = *it;
    //t.print();
    std::cout<<std::endl;
    /*for(int i=0; i<1025; i++)
    {
        v[i].print();
        std::cout<<" ";
    }*/



  /*  std::cout<<"#############"<<std::endl;
    
        for (auto x : v)
        {
            x.print();
            std::cout << " ";
        }
  

    std::cout << " first data" << std::endl;

    for(test x : vv)
    {
        x.print();
        std::cout<<" ";
    }
 
 */


/*
for(int i = 0; i< 100; i++)
{
emplaceB.push_back(std::move(i));


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
*/

    /*Vector<test>::const_iterator x(&v[100]);
v.insert(x,test(1,1,"234"));
v.insert(x,test(1,1,"234"));
v.insert(x,test(1,1,"234"));
v.insert(x,test(1,1,"234"));*/

    return 0;
}