

template <typename T>
class List
{
    template <typename U>
    class Node
    {
        public:
        Node(const U& value)
        {

        }
       
        Node<U>* previous;
        Node<U>* next;
        U object

    };

    public:
    void push_back(const T& value)
    {
        
    }
    void push_back(T&& value)
    {

    }

    private:
    int size = 0;
    Node<T>* begin = nullptr;
    Node<T>* end = nullptr;
};