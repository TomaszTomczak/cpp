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
    return static_cast<pointer>(::operator new(n * sizeof(value_type)));
  }

  void deallocate(value_type *p, std::size_t) noexcept
  {
    ::operator delete(p);
  }

  template <class U, class... Args>
  void construct(U *p, Args &&... args)
  {
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
  }

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
// TODO: Handle inserting aout of range <begin(),m_size) -> throw exception
  iterator insert(const_iterator pos, const T &value)
  {
   // std::cout<<"insert begin &"<<std::endl;
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
   // std::cout<<"check memory"<<std::endl;
    checkMemory();
   // std::cout<<"check memory"<<std::endl;
    shiftContent(position,1);
    //checkMemoryForShifting(1,position);
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
      insertToBuffer(i, value);
      m_size++;
    }
  }

  iterator insert(const_iterator pos, T &&value)
  {
    //std::cout<<"insert begin &&"<<std::endl;
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
      insertToBuffer(position, (*i));
      position++;
    }

  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args)
  {
   // std::cout<<"emplace && begin"<<std::endl;
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
   //     std::cout<<"check memory"<<std::endl;
    checkMemory();
   //     std::cout<<"shift content"<<std::endl;
    shiftContent(position, 1);
   //     std::cout<<"aaaand construct"<<std::endl;
    a.construct(buffer+position,std::forward<Args>(args)...);
   //     std::cout<<"increment"<<std::endl;
    m_size++;
    return iterator(buffer+position);
  }

  template <class... Args>
  T& emplace_back(Args &&... args)
  {
    checkMemory();
    a.construct(buffer+m_size, args...);
    m_size++;
    return back();
  }

  void erase(const_iterator pos);
  void erase(const_iterator first, const_iterator last);

  void push_back(const T &object)
  {
    checkMemory();
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
  void insertToBuffer(uint32_t& position, const T& value)
  {
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
    if(isTrivial)
    {
      buffer[position] = std::move(value);
    }
    else
    {
      a.construct(buffer+position, std::move(value));
    }
    
  }

 void shiftContent(uint32_t position, uint32_t elementCount)
  {
    memmove(buffer + position + elementCount, buffer + position, (m_size-position) * sizeof(T));  
  }

  bool checkMemory(uint32_t requiredSpace)
  {
    if (m_size + requiredSpace > m_reservedSize)
    {
     reallocate_memory([=]()->uint32_t {
       uint32_t retMem = (m_reservedSize == 0) ? 1 : m_reservedSize*2;
       while (requiredSpace + m_size > retMem)
       {
         retMem *= 2;
       }
       return retMem;
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

  void reallocate_memory(uint32_t newSize)
  {
    T *tmp = a.allocate(newSize);
    uint32_t gap = 0;
    if (newSize < m_reservedSize)
    {
      if (newSize < m_size)
      {
        m_size = newSize;
      }
    }
    if (m_size > 0)
    {
     /*for (int i = 0; i < m_size; i++)
      {
        if(gapSize != 0 && position - 1 == i)
        {
          gap = gapSize;
        }
        if(isTrivial)
        {
         // tmp[i+gap] = buffer[i];
         memmove(dst, src, n*sizeof(value_type));

        // memmove(&arr[0], &arr[1], sizeof(arr) - sizeof(*arr));
        }
        else
        {
          a.construct(tmp+i+gap, buffer[i]);
        }        
        //TODO: SFINAE
      }*/
      
        memmove(tmp, buffer, m_size*sizeof(T));
        a.deallocate(buffer, m_reservedSize);

     /* std::cout<<"memmove 1: m_size - position"<<m_size-position<<std::endl;
      memmove(tmp, buffer, (position - 1) * sizeof(T));
      std::cout<<"memmove 2; to position: "<<position+gap<<std::endl;
      memmove(tmp + position + gap, buffer + position, (m_size-position)*sizeof(T));
      a.deallocate(buffer, m_reservedSize);
      */
    }
    buffer = tmp;
    m_reservedSize = newSize;
  };

  Alloc a;
  T *buffer;
  uint32_t m_size = 0;
  uint32_t m_reservedSize = 0;
  bool isTrivial = std::is_trivially_copyable<T>::value;
};