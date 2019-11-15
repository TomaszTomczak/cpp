#pragma once
#include <stdint.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <type_traits>


/*

Check why if destructor is defined, so class is trivialy destructible there are some problems with memory

*/


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

  template <class U>
  struct rebind
  {
    typedef allocator<U> other;
  };
  allocator() noexcept {}
  template <class U>
  allocator(allocator<U> const &) noexcept {}

  pointer allocate(std::size_t n)
  {
    return  static_cast<pointer>(::operator new(n * sizeof(value_type)));    
  }

  void deallocate(value_type *p, std::size_t size)  noexcept
  {
    ::operator delete(p,size);
  }

  template <class U, class... Args>
  void construct(U *p, Args &&... args)
  {
    try
    {
      ::new (p) U(std::forward<Args>(args)...);
    }
    catch(const std::exception& e)
    {
      std::cout << "konstr. cos nie tak z pointerem:  "<<static_cast<void*>(p)<<" "<<e.what() << '\n';
    }
    
    
  }
  void destroy(pointer p) { p->~value_type(); }
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
    iteratorBase(const iteratorBase<WasConst> &rhs) : ptr_(rhs.ptr_){/*std::cout << "conversion to const" << std::endl;*/};
    iteratorBase() {}

    // operators
    ptrdiff_t operator-(const iteratorBase &it)
    {
      return ptr_ - it.ptr_;
    }
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
    pointer operator->()
    {
      return ptr_;
    }
    reference operator*()
    {
      return *ptr_;
    }
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

  typedef T value_type;
  typedef Alloc allocator_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typename std::allocator_traits<Alloc>::pointer pointer;
  typename std::allocator_traits<Alloc>::const_pointer const_pointer;
  Vector(){};

  Vector(std::size_t count, const T &value, const Alloc &allocator = Alloc())
  {
    a = allocator;
    buffer = a.allocate(count);
    for (std::size_t i = 0; i <= count; i++)
    {
      push_back(value);
    }
  }

  Vector(std::initializer_list<T> ilist, const Alloc &allocator = Alloc()) //TODO: update to handle allocator
  {
    a = allocator;
    m_reservedSize = static_cast<std::size_t>(ilist.size());
    for (auto x : ilist)
    {
      push_back(x);
    }
  }
  explicit Vector(const Alloc &alloc) noexcept
  {
    a = alloc;
  }
  explicit Vector(std::size_t count, const Alloc &alloc = Alloc());
  Vector(const Vector &other)
  {
    a = other.a;
    m_size = other.m_size;
    m_reservedSize = other.m_reservedSize;
    buffer = a.allocate(m_reservedSize);

    for (std::size_t i = 0; i < m_size; i++)
    {
      buffer[i] = other.buffer[i];
    }
  }
  //Vector( const Vector& other, const Alloc& alloc);
  //Vector(Vector &&other) noexcept;
  //Vector(Vector&& other, const Alloc& alloc );
  template <class InputIt>
  Vector(InputIt first, InputIt last, const Alloc &alloc = Alloc());
  ~Vector()
  {
    if (m_size > 0)
    {
      for(std::size_t i=0; i<m_size; i++)
      {
        buffer[i].~T();
      }
      a.deallocate(buffer, m_reservedSize);
    }
  }

  T &at(std::size_t position)
  {
    if (position >= m_size)
    {
      throw std::out_of_range("Out of range while 'at' operation");
    }
    return buffer[position];
  };
  const T &at(std::size_t position) const
  {
    if (position >= m_size)
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
  std::size_t size() const noexcept
  {
    return m_size;
  };
  std::size_t max_size() //TODO
  {
    return 0;
  }

  void reserve(std::size_t new_capability)
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
  std::size_t capacity() const noexcept { return m_reservedSize; };
  void shrink_to_fit()
  {
    if(m_size>0)
    {
      reallocate_memory(m_size);
    }
  }

  //modifiers
  void clear() noexcept
  {
    if (m_size > 0)
    {
      for(std::size_t i=0; i<m_size; i++)
      {
        buffer[i].~T();
      }
      a.deallocate(buffer, m_reservedSize);
      m_size = 0;
      m_reservedSize = 0;
      buffer = nullptr;
    }
    
  }

  /*
    Inserts TODO: exceptions
*/
  // TODO: Handle inserting aout of range <begin(),m_size) -> throw exception
  iterator insert(const_iterator pos, const T &value)
  {
    //std::cout<<"insert copy &"<<std::endl;
    //std::size_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    std::size_t position = std::distance(cbegin(), pos);
    checkMemory();
    shiftContent(position, 1);
    insertToBuffer(position, value);
    m_size++;
    return iterator(buffer + position);
  }

  void insert(const_iterator pos, std::size_t count, const T &value)
  {
    //std::size_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    std::size_t position = std::distance(cbegin(), pos);
    checkMemory(count);
    shiftContent(position, count);
    for (std::size_t i = position; i <= (position + count) - 1; i++)
    {
      insertToBuffer(i, value);
      m_size++;
    }
  }

  iterator insert(const_iterator pos, T &&value)
  {
    return emplace(pos, std::forward<T>(value));
  }

  iterator insert(const_iterator pos, std::initializer_list<T> ilist)
  {

  //  std::size_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    std::size_t position = std::distance(cbegin(), pos);
    checkMemory();
    std::size_t elementCount = static_cast<std::size_t>(ilist.size());
    shiftContent(position, elementCount);
    std::size_t startInsertPosition = position;
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
    std::size_t position = std::distance(cbegin(), pos);
    //std::size_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    std::size_t count = std::distance(first, last);
    checkMemory(count);
    shiftContent(position, count);

    for (auto i = first; i != last; i++)
    {
      insertToBuffer(position, (*i));
      position++;
    }
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&... args)
  {
    std::size_t position = std::distance(cbegin(), pos);
   // std::size_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    checkMemory();
    shiftContent(position, 1);
    a.construct(buffer + position, std::forward<Args>(args)...);
    m_size++;
    return iterator(buffer + position);
  }

  template <class... Args>
  T &emplace_back(Args &&... args)
  {
    checkMemory();
    a.construct(buffer + m_size, args...);
    m_size++;
    return back();
  }

  void erase(const_iterator pos)
  {
    if(m_size == 0) return;
    std::size_t position = std::distance(cbegin(), pos);
    buffer[position].~T();
    shiftContent(position, -1);
    m_size--;
  }
  void erase(const_iterator first, const_iterator last)
  {
    std::size_t elementCount = std::distance(first,last);
    std::size_t position = std::distance(cbegin(), first);
    shiftContent(position, elementCount*-1);
    m_size-=elementCount;
  }

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
    if (m_size > 0)
    {
      m_size--;
      buffer[m_size].~T();
    }
  }

  void resize(std::size_t new_size);
  void resize(std::size_t new_size, const T &value);

  void swap(Vector &other)
  {
      Alloc _tmp_a = std::move(other.a);
      T *_tmp_buffer = std::move(other.buffer);
      std::size_t _tmp_m_size = std::move(other.m_size);
      std::size_t _tmp_m_reservedSize = std::move(other.m_reservedSize);
      other.a = std::move(a);
      other.buffer = std::move(buffer);
      other.m_size = std::move(m_size);
      other.m_reservedSize = std::move(m_reservedSize);
      a = std::move(_tmp_a);
      buffer = std::move(_tmp_buffer);
      m_size = std::move(_tmp_m_size);
      m_reservedSize = std::move(_tmp_m_reservedSize);
  }

  //operators
  T &operator[](std::size_t element)
  {
    return buffer[element];
  };

  Vector<T> &operator=(const Vector<T> &other)         //copy
  {
    m_size = other.m_size;
    a = other.a;
    m_reservedSize = other.m_reservedSize;
    buffer = a.allocate(m_reservedSize);
    isTrivial = other.isTrivial;
    for (std::size_t i = 0; i < m_size; i++)
    {
          std::cout<<"copy operator"<<std::endl;
      a.construct(buffer+i, other.buffer[i]);
    }
    return *this; 
  }
  Vector<T> &operator=(Vector<T> &&other)              //move
  {
    m_size = std::move(other.m_size);
    other.m_size = 0;
    a = std::move(other.a);
    m_reservedSize = std::move(other.m_reservedSize);
    buffer = std::move(other.buffer);
    other.buffer = nullptr;
    isTrivial = std::move(other.isTrivial);
    return *this;
  }
  Vector<T> &operator=(std::initializer_list<T> ilist) // replace with initializer list
  {
    if(m_size>0)
    {
       a.deallocate(buffer, m_reservedSize);
    }
    m_reservedSize = 0;
    m_size = 0;

    for (auto x : ilist)
    {
      push_back(x);
    }
  }

private:

   template <class U, typename std::enable_if<std::is_same<typename std::decay<U>::type,T>::value, T>::type* = nullptr>
   inline void insertToBuffer(const std::size_t &position, U&& value)
  {
    if (isTrivial)
    {
      buffer[position] = std::forward<U>(value);
    }
    else
    {
      a.construct(buffer + position, std::forward<U>(value));
    }
  }

inline void shiftContent(const std::size_t& position, const int32_t& elementCount)
  {

    if(m_size == 0 || elementCount == 0) return;

     //insertToBuffer(m_size, std::move(buffer[m_size-1]));
    if(elementCount>0)
    {
      a.construct(&buffer[m_size], std::move(buffer[m_size-1]));
     // a.destroy(&buffer[m_size-1]); // remove old data to avoid memory leaks
      for (int32_t i = (m_size + elementCount) - 2; i >= position + elementCount; i--)
      {
        buffer[i] = std::move(buffer[i-elementCount]);
      }
    }
    else
    {
      int elementsToCover = elementCount *-1;
      int constructedElementNo = 0;
      for(int i = position; i<m_size-elementsToCover; i++)
      {
        if(constructedElementNo<=elementsToCover)
        {
        constructedElementNo++;
        buffer[i].~T();
        a.construct(&buffer[i], std::move(buffer[i+elementsToCover]));
        }
        else
        {
          buffer[i] = buffer[i+elementsToCover];
        }

        //
      }     
    }
  }

  inline void checkMemory()
  {
    checkMemory(1);
  }

  inline void checkMemory(const std::size_t& requiredSpace)
  {
    if (m_size + requiredSpace > m_reservedSize)
    {
      reallocate_memory([=]() -> std::size_t {
        std::size_t retMem = (m_reservedSize == 0) ? 1 : m_reservedSize * 2;
        while (requiredSpace + m_size > retMem)
        {
          retMem *= 2;
        }
        return retMem;
      }());
      
    }
  }

  inline void reallocate_memory(const std::size_t& newSize)
  {   
    T* tmp = a.allocate(newSize);
    if (newSize < m_reservedSize)
    {
      if (newSize < m_size)
      {
        m_size = newSize;
      }
    }
    if (m_size > 0)
    {

      if(isTrivial)
      {
        memcpy(tmp, buffer, m_size * sizeof(T));
      }
      else
      {
       for (std::size_t i = 0; i < m_size; i++)
        {            
            a.construct(tmp+i, std::move(buffer[i]));
            a.destroy(&buffer[i]);                          
         
        }
      }
    a.deallocate(buffer, m_reservedSize);
      
    }
    buffer = tmp;
    m_reservedSize = newSize;
  };

  void destroy(T* p)
	{
    try
    {
      	a.destroy(p);
    }
    catch(const std::exception& e)
    {
      std::cerr<<"destroy " << e.what() << '\n';
    }
    
	
	}
  void log(std::string logContent)
  {
    if(enableLogging)
    {
      std::cout<<logContent<<std::endl;
    }
  }
  Alloc a;
  T *buffer;
  std::size_t m_size = 0;
  std::size_t m_reservedSize = 0;
  bool isTrivial = std::is_trivially_copyable<T>::value;
  bool enableLogging = false;
};