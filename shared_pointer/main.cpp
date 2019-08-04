#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
class shared
{
    public:
    T *pointer;
    int32_t counter = 0;

    void increment()
    {
        counter ++;
        std::cout<< "counter incremented"<<std::endl;
        getCount();
    }
    void decrement()
    {
        counter --;
        std::cout<< "counter decremented"<<std::endl;
        getCount();
    }

    shared() = delete;

    shared(T* object)
    {
        std::cout<< "init constructor" << std::endl;
        pointer = object;
        increment();
    }
    ~shared()
    {
        std::cout <<"destructor"<<std::endl;
        decrement();
        if(!counter)
        {
            delete pointer;
        }
    }
    shared(const shared<T>& other)
    {
        std::cout<<"constructor kopiujacy"<<std::endl;
        counter = other.counter;
        pointer = other.pointer;
        increment();
    }
    void getCount() const
    {
        std::cout<<"ilosc referencji: "<< counter<<std::endl;
    }

    shared<T> &operator=(const shared<T> &copy)
    {
        std::cout<<"przypisanie"<<std::endl;
         pointer = copy.pointer;
         counter = copy.counter;
         increment();
        return *this;
    }
    T *operator->() const
    {
        std::cout<<"wyluskanie"<<std::endl;
        return pointer;
    }
};
class Dupa
{
    public: 
    Dupa() = default;
    std::string getName() const
    {
        return name;
    }
    void setName(const std::string& nazwa)
    {
        name = nazwa;
    }
    private:
    std::string name;
};

void funkcja(shared<Dupa> test)
{
    std::cout<<"Funkcja: "<<test->getName();
}

int main()
{

    auto test1 = shared<Dupa>(new Dupa()); // tworzenie
    test1->setName("nazwa instancji");
    auto test2 = shared<Dupa>(test1); // kopiowanie
    auto test3 = test1;
    funkcja(test1);
    funkcja(test2);
    return 0;

    std::vector<int>* v;


}