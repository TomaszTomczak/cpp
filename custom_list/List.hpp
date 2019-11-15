
#include <type_traits>
#include <iterator>
#include <iostream>

template <typename T>
class List
{
    private:
    template <typename U>
    struct Node
    {
        Node(const U& value)
        {
            object = value;
        }
       
        Node<U>* previous = nullptr;
        Node<U>* next = nullptr;
        U object;
    };

    public:
    List() = default; 

  template <bool isConst>
  class iteratorBase
  {
  public:
    // iterator traits
    typedef iteratorBase self_type;
    typedef T value_type;
    typedef std::conditional_t<isConst, const T &, T &> reference;
    typedef std::conditional_t<isConst, const T *, T *> pointer;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef int difference_type;

    //ctors
    iteratorBase(Node<T>* ptr) : ptr_(ptr) {}
    template <bool WasConst, class = std::enable_if_t<isConst && !WasConst>>
    iteratorBase(const iteratorBase<WasConst> &rhs) : ptr_(rhs.ptr_){/*std::cout << "conversion to const" << std::endl;*/};
    iteratorBase() {}


    self_type operator++()
    {
        std::cout<< "++"<<std::endl;
      self_type i = *this;
      ptr_ = ptr_->next;
      return i;
    }
    self_type operator++(int junk)
    {
                std::cout<< "++ with junk"<<std::endl;
      ptr_++;
      return *this;
    }
    self_type operator=(const self_type &other)
    {
        std::cout<<"="<<std::endl;
      ptr_ = other.ptr_;
      return *this;
    }
    pointer operator->()
    {
      std::cout<<"->"<<std::endl;
      return ptr_->object;
    }
    reference operator*()
    {
      std::cout<<"*"<<std::endl;
      return ptr_->object;
    }
    bool operator==(const self_type &rhs) { return ptr_ == rhs.ptr_; }
    bool operator!=(const self_type &rhs) { return ptr_ != rhs.ptr_; }
    // TODO: implement -- operator

    friend iteratorBase<true>; //ugh... I believe somewhere exists better way to handle this
  private:
    Node<T>* ptr_;
  };

  typedef iteratorBase<true> const_iterator;
  typedef iteratorBase<false> iterator;

    iterator begin()
    {
        return iterator(m_head);
    }
    iterator end()
    {
        return iterator(m_tail->next);
    }
    void push_back(const T& value)
    {
        Node<T>* new_element = new Node<T>(value);
        if(size == 0)
        {
            m_head = new_element;
            m_tail = new_element;
        }
        else
        {
            m_tail->next = new_element;
            new_element-> previous = m_tail;
            m_tail = new_element;
        }
        size ++;
        
    }
    /*void push_back(T&& value)
    {

    }*/

    private:
    int size = 0;
    Node<T>* m_head = nullptr;
    Node<T>* m_tail = nullptr;
};