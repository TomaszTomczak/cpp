#pragma once
#include <stdint.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <cstring>
	

template <typename T>
class allocator
{
public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T value_type;

  template <class U> struct rebind {typedef allocator<U> other;};
  allocator() noexcept {}
  template <class U>
  allocator(allocator<U> const &) noexcept {}

  pointer allocate(std::size_t n)
  {
   // std::cout<<"allocating: "<<n*sizeof(value_type)<<std::endl;
    return static_cast<pointer>(::operator new(n * sizeof(value_type)));
   //pointer ret =  static_cast<pointer>(::operator new(n*sizeof(T)));
   // std::cerr << " allocated at: " << (void*)ret << std::endl;
   // return ret;
    //return new T[n];
  }
  void deallocate(value_type *p, std::size_t) noexcept
  {
    ::operator delete(p);
  }

  template <class U, class... Args>
  void construct(U *p, Args &&... args)
  {
    //std::cout<<"construct "<<std::endl;
    ::new (p) U(std::forward<Args>(args)...);
  }
};

template <typename T, typename Alloc = allocator<T>>
class Vector
{
public:

  template <bool isConst>
  class iteratorBase
  {
  public:
    // iterator traits
    typedef iteratorBase self_type;
    typedef T value_type;
    typedef std::conditional_t<isConst, const T &, T &> reference;
    typedef std::conditional_t<isConst, const T *, T *> pointer;
    typedef std::random_access_iterator_tag iterator_category;
    typedef int difference_type;

    //ctors
    iteratorBase(pointer ptr) : ptr_(ptr) {}
    template <bool WasConst, class = std::enable_if_t<isConst && !WasConst>>
    iteratorBase(const iteratorBase<WasConst> &rhs) : ptr_(rhs.ptr_) { /*std::cout << "conversion to const" << std::endl;*/ };
    iteratorBase() {}

    // operators
    ptrdiff_t operator-(const iteratorBase &it) { return ptr_ - it.ptr_; }
    self_type operator++()
    {
      self_type i = *this;
      ptr_++;
      return i;
    }
    self_type operator++(int junk)
    {
      ptr_++;
      return *this;
    }
    self_type operator=(const self_type &other)
    {
      ptr_ = other.ptr_;
      return *this;
    }
    pointer operator->() { return ptr_; }
    reference operator*() { return *ptr_; }
    bool operator==(const self_type &rhs) { return ptr_ == rhs.ptr_; }
    bool operator!=(const self_type &rhs) { return ptr_ != rhs.ptr_; }

    friend iteratorBase<true>; //ugh... I believe somewhere exists better way to handle this
  private:
    pointer ptr_;
  };

  typedef iteratorBase<true> const_iterator;
  typedef iteratorBase<false> iterator;

  class reverse_iterator
  {
  };

  Vector()
  {
   // std::cout << "default constructor" << std::endl;
  };

  Vector( uint32_t count, const T& value, const Alloc& allocator = Alloc())
  {
    a = allocator;
    buffer = a.allocate(count);
    for(int i = 0; i<=count; i++)
    {
      push_back(value);
    }
  }

  Vector(std::initializer_list<T> ilist, const Alloc& allocator = Alloc()) //TODO: update to handle allocator
  {
    a = allocator;
    m_reservedSize = static_cast<int>(ilist.size());
    for (auto x : ilist)
    {
      push_back(x);
    }
  }
  explicit Vector( const Alloc& alloc ) noexcept
  {
    a = alloc;
  }
  explicit Vector( uint32_t count, const Alloc& alloc = Alloc() );
  Vector( const Vector& other )
  {
      a = other.a;
      m_size= other.m_size;
      m_reservedSize = other.m_reservedSize;
      buffer = a.allocate(m_reservedSize);

      for(int i=0; i<m_size; i++)
      {
        buffer[i] = other.buffer[i];
      }
  }
  //Vector( const Vector& other, const Alloc& alloc);
  Vector(Vector&& other) noexcept;
  //Vector(Vector&& other, const Alloc& alloc );
  template< class InputIt >
  Vector(InputIt first, InputIt last, const Alloc& alloc = Alloc());
  ~Vector()
  {
    a.deallocate(buffer,m_reservedSize);
    //delete[] buffer;
  }

  //Element access
  T &at(uint32_t position) 
  { 
    if(position>=m_size)
    {
      throw std::out_of_range("Out of range while 'at' operation");
    }
    return buffer[position]; 
  };
  const T &at(uint32_t position) const 
  { 
        if(position>=m_size)
    {
      throw std::out_of_range("Out of range while 'at' operation");
    }
    return buffer[position]; 
  };
  T &front() { return buffer[0]; };
  const T &front() const { return buffer[0]; };
  T &back() { return buffer[m_size - 1]; };
  const T &back() const { return buffer[m_size - 1]; };
  T *data() { return buffer; };
  const T *data() const { return buffer; };

  //iterators
  iterator begin() noexcept
  {
    return iterator(buffer);
  };
  const_iterator begin() const noexcept
  {
    return const_iterator(buffer);
  };
  const_iterator cbegin()
  {
    return const_iterator(buffer);
  };
  iterator end() noexcept
  {
    return iterator(buffer + m_size);
  };
  const_iterator end() const noexcept
  {
    return const_iterator(buffer + m_size);
  };
  const_iterator cend() const noexcept
  {
    return const_iterator(buffer + m_size);
  };
  reverse_iterator rbegin();
  reverse_iterator rend();

  // capacity
  bool empty() const noexcept
  {
    return m_size == 0;
  };
  uint32_t size() const noexcept
  {
    return m_size;
  };
  uint32_t max_size() //TODO
  {
  return 0;
  }

  void reserve(uint32_t new_capability)
  {
   /* if(new_capability > max_size())
    {
      throw std::length_error("requested capability is greater than max size");
    } 
    */
    if (new_capability > m_reservedSize)
    {
      reallocate_memory(new_capability); //TODO: handle allocator exceptions
    }
  };
  uint32_t capacity() const noexcept { return m_reservedSize; };
  void shrink_to_fit();

  //modifiers
  void clear() noexcept //TODO: implement
  {

  }

  /*
    Inserts TODO: exceptions
*/
  iterator insert(const_iterator pos, const T &value)
  {
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    checkMemory();
    shiftContent(pos,1);
    insertToBuffer(position, value);
    m_size++;
    return iterator(buffer + position);
  }
  void insert(const_iterator pos, uint32_t count, const T &value)
  {
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    checkMemory(count);
    shiftContent(position, count);
    for (int i = position; i <= (position + count) - 1; i++)
    {
     // std::cout << "filling: " << i << "with value: " << value << std::endl;
      //buffer[i] = value;
      insertToBuffer(i, value);
      m_size++;
    }
  }

  iterator insert(const_iterator pos, T &&value)
  {
    return emplace(pos, std::move(value));
  }

  iterator insert(const_iterator pos, std::initializer_list<T> ilist)
  {
    checkMemory();
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    uint32_t elementCount = static_cast<uint32_t>(ilist.size());
    shiftContent(position, elementCount);
    uint32_t startInsertPosition = position;
    for (auto x : ilist)
    {
      //buffer[startInsertPosition] = x;
      insertToBuffer(startInsertPosition, x);
      startInsertPosition++;
    }
    return iterator(buffer + position);
  }
  
  template <class InputIterator>
  void insert(const_iterator pos, InputIterator first, InputIterator last)
  {
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    uint32_t count = std::distance(first,last);
    checkMemory(count);
    shiftContent(position,count);

    for(auto  i = first; i!=last; i++)
    {
      //std::cout<<"element: "<< (*i) << "inserting to pos: "<<position<<std::endl;
      buffer[position] = (*i);
      position++;
    }

  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args)
  {
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    checkMemory();
    //std::cout<<"shifting"<<std::endl;
    shiftContent(position, 1);
   // std::cout<<"constructing"<<std::endl;
    a.construct(buffer+position, args...);
    m_size++;
    return iterator(buffer+position);
  }

  template <class... Args>
  T& emplace_back(Args &&... args)
  {
    checkMemory();

    a.construct(buffer+m_size, args...);
   // ::new (buffer+m_size) T(std::forward<Args>(args)...);
    m_size++;
    return back();
    //push_back(std::move(a.construct<T>(args)))
  }

  void erase(const_iterator pos);
  void erase(const_iterator first, const_iterator last);

  void push_back(const T &object)
  {
    std::cout<<"&"<<std::endl;
    checkMemory();
  /*   if(isTrivial)
    {
     buffer[m_size++] = object;
    }
    else
    {
      a.construct(buffer+m_size++, object);
    }*/
    
   insertToBuffer(m_size, object);
   m_size++;
    

  };
  void push_back(T &&object)
  {
    emplace_back(std::move(object));
  }

  void pop_back()
  {
    if(m_size>0)
    {
    m_size--;
    buffer[m_size].~T();
    }
  }

  void resize(uint32_t new_size);
  void resize(uint32_t new_size, const T& value);

  void swap(Vector &other)
  {

  }

  //operators
  T &operator[](uint32_t element)
  {
    return buffer[element];
  };

  Vector<T> &operator=(const Vector<T> &other);         //copy
  Vector<T> &operator=(Vector<T> &&other);              //move
  Vector<T> &operator=(std::initializer_list<T> ilist); // replace with initializer list

private:
  void insertToBuffer(uint32_t& position, T& value)
  {
   // std::cout<<" insert to buffer &"<<std::endl;
    if(isTrivial)
    {
      buffer[position] = value;
    }
    else
    {
      a.construct(buffer+position, value);
    }
  }

  void insertToBuffer(uint32_t& position, T&& value)
  {
   // std::cout<<" insert to buffer &&"<<std::endl;
    if(isTrivial)
    {
      buffer[position] = std::move(value);
    }
    else
    {
      //std::cout<<"construct in &&"<<std::endl;
      a.construct(buffer+position, std::move(value));
    }
    
  }
  void shiftContent(uint32_t pos, uint32_t elementCount)
  {
    for (uint32_t i = (m_size + elementCount) - 1; i >= pos + elementCount; i--)
    {
      //std::cout<<"shift shift srutututu"<<std::endl;
      insertToBuffer(i, buffer[i - elementCount] );
     // buffer[i] = buffer[i - elementCount];
    }
  }
 bool checkMemory(uint32_t requiredSpace)
  {
   // std::cout << "check available memory" << std::endl;
    if (m_size + requiredSpace > m_reservedSize)
    {
     reallocate_memory([=]()->uint32_t {
       uint32_t retMem = (m_reservedSize == 0) ? 1 : m_reservedSize*2;
       while (requiredSpace + m_size > retMem)
       {
         retMem *= 2;
       }
       return retMem;
       //return m_reservedSize == 0 ? 1 : m_reservedSize*2;
     }());
    return true;
    }
    return false;
  }
  // TODO: sprawdzac czy doszlo do realokacji. jesli nie to iteratory sa valid i nie trzeba ich ponownie robic
   bool checkMemory()
  {
    return checkMemory(1);
  }

  /*
    1) void construct(U *p, Args &&... args)
    2) tmp[i] = buffer[i];
    3) buffer[i] = buffer[i - elementCount];
    4) 
   */
  //void tmp[i] = buffer[i];
  void reallocate_memory(uint32_t newSize)
  {
    //a.allocate(5);
  //  std::cout << "memory reallocation to size: " << newSize <<" m_size = "<<m_size<<" m_reservedSize: "<<m_reservedSize<<"buffer: "<<sizeof(buffer)<<std::endl;
  //  std::cout<< "size of T: "<<sizeof(T)<<std::endl;
    T *tmp = a.allocate(newSize);
  //  std::cout << "realocated"<<std::endl;
    //uint32_t copyRange = m_reservedSize;
    if (newSize < m_reservedSize)
    {
      if (newSize < m_size)
      {
        m_size = newSize;
      }
    }
    if (m_size > 0)
    {
    //memcpy( (void*)tmp, (void*)buffer, (m_size-1) * sizeof(T) );
     for (int i = 0; i < m_size; i++)
      {
        if(isTrivial)
        {
          tmp[i] = buffer[i];
        }
        else
        {
          a.construct(tmp+i, buffer[i]);
        }
        
        //TODO: SFINAE dla trywialnie konstruowanych  tak zeby tylko kopiowalo
       
      }
      

      a.deallocate(buffer, m_reservedSize);
    }
 //   std::cout << "replacing arrays"<<std::endl;
    buffer = tmp;
 //   std::cout << "all good"<<std::endl;
    m_reservedSize = newSize;
 //   std::cout << "end of reallocate memory"<<std::endl;

  };

  Alloc a;
  T *buffer;
  uint32_t m_size = 0;
  uint32_t m_reservedSize = 0;
  bool isTrivial = std::is_trivially_copyable<T>::value;
};