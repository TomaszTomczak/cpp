

template <typename T>
class List
{
    template <typename U>
    class Node
    {
        public:
        Node(const U& value)
        {
            object = value;
        }
       
        Node<U>* previous = nullptr;
        Node<U>* next = nullptr;
        U object

    };

    public:
    void push_back(const T& value)
    {
        Node<T>* new_element = new Node<T>(value);
        if(size == 0)
        {
            begin = new_element;
            end = new_element;
        }
        else
        {
            end->next = new_element;
            new_element-> previous = end;
            end = new_element;
        }
        size ++;
        
    }
    void push_back(T&& value)
    {

    }

    private:
    int size = 0;
    Node<T>* begin = nullptr;
    Node<T>* end = nullptr;
};