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
    pointer buff;
    try
    {
      buff = static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }
    catch(const std::exception& e)
    {
      std::cout << "###########" <<e.what() << '\n';
    }
    
    return buff;
  }

  void deallocate(value_type *p, std::size_t) noexcept
  {
    ::operator delete(p);
  }

  template <class U, class... Args>
  void construct(U *p, Args &&... args)
  {
    //std::cout << "new element will be placed at address: " << static_cast<void *>(p) << std::endl;
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
    typedef std::forward_iterator_tag iterator_category;
    typedef int difference_type;

    //ctors
    iteratorBase(pointer ptr) : ptr_(ptr) {}
    template <bool WasConst, class = std::enable_if_t<isConst && !WasConst>>
    iteratorBase(const iteratorBase<WasConst> &rhs) : ptr_(rhs.ptr_){/*std::cout << "conversion to const" << std::endl;*/};
    iteratorBase() {}

    // operators
    ptrdiff_t operator-(const iteratorBase &it)
    {
      //std::cout << "ptrdiff" << std::endl;
      return ptr_ - it.ptr_;
    }
    self_type operator++()
    {
     // std::cout << "i++ in iterator" << std::endl;
      self_type i = *this;
      ptr_++;
      //std::cout << "++ pointer value: " << static_cast<const void *>(ptr_) << std::endl;
      return i;
    }
    self_type operator++(int junk)
    {
      //std::cout << "++itertor" << std::endl;
      ptr_++;
      return *this;
    }
    self_type operator=(const self_type &other)
    {
      //std::cout << "operator =" << std::endl;
      ptr_ = other.ptr_;
      return *this;
    }
    pointer operator->()
    {
      //std::cout << "wyluskanie" << std::endl;
      return ptr_;
    }
    reference operator*()
    {
      //std::cout << "dereferencja, pointer address: " << static_cast<void *>(ptr_) << std::endl;
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

  Vector(){};

  Vector(uint32_t count, const T &value, const Alloc &allocator = Alloc())
  {
    a = allocator;
    buffer = a.allocate(count);
    for (int i = 0; i <= count; i++)
    {
      push_back(value);
    }
  }

  Vector(std::initializer_list<T> ilist, const Alloc &allocator = Alloc()) //TODO: update to handle allocator
  {
    a = allocator;
    m_reservedSize = static_cast<int>(ilist.size());
    for (auto x : ilist)
    {
      push_back(x);
    }
  }
  explicit Vector(const Alloc &alloc) noexcept
  {
    a = alloc;
  }
  explicit Vector(uint32_t count, const Alloc &alloc = Alloc());
  Vector(const Vector &other)
  {
    a = other.a;
    m_size = other.m_size;
    m_reservedSize = other.m_reservedSize;
    buffer = a.allocate(m_reservedSize);

    for (int i = 0; i < m_size; i++)
    {
      buffer[i] = other.buffer[i];
    }
  }
  //Vector( const Vector& other, const Alloc& alloc);
  Vector(Vector &&other) noexcept;
  //Vector(Vector&& other, const Alloc& alloc );
  template <class InputIt>
  Vector(InputIt first, InputIt last, const Alloc &alloc = Alloc());
  ~Vector()
  {
    std::cout << "dstr" << std::endl;
    if (m_size > 0)
    {
      for(int i=0; i<m_size; i++)
      {
        buffer[i].~T();
      }
      a.deallocate(buffer, m_reservedSize);
    }
  }

  T &at(uint32_t position)
  {
    if (position >= m_size)
    {
      throw std::out_of_range("Out of range while 'at' operation");
    }
    return buffer[position];
  };
  const T &at(uint32_t position) const
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
    //std::cout << "begin called. iterator returned, should be ptr: " << static_cast<void *>(buffer) << std::endl;
    //std::cout << "last element ptrshould be: " << static_cast<void *>(buffer + (m_size - 1)) << std::endl;
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
    //std::cout << "end called. iterator returned, should be ptr: " << static_cast<void *>(buffer + m_size) << std::endl;
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
  void shrink_to_fit()
  {
    if(m_size>0)
    {
      reallocate_memory(m_size);
    }
  }

  //modifiers
  void clear() noexcept //TODO: implement
  {
    if (m_size > 0)
    {
      for(int i=0; i<m_size; i++)
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
    // std::cout<<"insert begin &"<<std::endl;
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
    // std::cout<<"check memory"<<std::endl;
    checkMemory();
    // std::cout<<"check memory"<<std::endl;
    shiftContent(position, 1);
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
    uint32_t count = std::distance(first, last);
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
    uint32_t position = std::distance(static_cast<const_iterator>(begin()), pos);
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
    //std::cout<<"&& new data will be placed on position: "<<m_size<<" at address: "<<static_cast<void*>(buffer + m_size)<<std::endl;
    a.construct(buffer + m_size, args...);
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
    if (m_size > 0)
    {
      m_size--;
      buffer[m_size].~T();
    }
  }

  void resize(uint32_t new_size);
  void resize(uint32_t new_size, const T &value);

  void swap(Vector &other)
  {
      Alloc _tmp_a = std::move(other.a);
      T *_tmp_buffer = std::move(other.buffer);
      uint32_t _tmp_m_size = std::move(other.m_size);
      uint32_t _tmp_m_reservedSize = std::move(other.m_reservedSize);
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
  T &operator[](uint32_t element)
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
    for (uint32_t i = 0; i < m_size; i++)
    {
      a.construct(buffer+i, other.buffer[i]);
    }

  }
  Vector<T> &operator=(Vector<T> &&other)              //move
  {
    m_size = other.m_size;
    other.m_size = 0;
    a = std::move(other.a);
    m_reservedSize = std::move(other.m_reservedSize);
    buffer = std::move(other.buffer);
    other.buffer = nullptr;
    isTrivial = std::move(other.isTrivial);
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

// TODO: perfect forwarding to avoid of implementation multiple overloads for l and r value parameters
   void insertToBuffer(uint32_t &position, const T &value)
  {
    if (isTrivial)
    {
      buffer[position] = value;
    }
    else
    {
      //std::cout<<"& non trival insert to buffer"<<std::endl;
      a.construct(buffer + position, value);
    }
  }

   void insertToBuffer(uint32_t &position, T &&value)
  {
    if (isTrivial)
    {
      buffer[position] = std::move(value);
    }
    else
    {
           // std::cout<<"&& non trival insert to buffer"<<std::endl;
      a.construct(buffer + position, std::move(value));
    }
  }
  inline void constructElementsShift(T *dst, T *src, const uint32_t count)
  {
      if(count == 0) return;
      for (uint32_t i = count - 1; i > 0; i--)
      {
        a.construct(&dst[i], std::move(src[i]));
      }
     // new (&dst[0]) T(std::move(src[0]));
    
  }
  inline void shiftContent(uint32_t position, uint32_t elementCount)
  {

   // constructElementsShift(buffer + position + elementCount, buffer + position, m_size - position);
     memmove(buffer + position + elementCount, buffer + position, (m_size-position) * sizeof(T));
 //   std::cout << "shiftin " << elementCount << " elements from starting ptr: "
  //            << static_cast<void *>(buffer + position) << " to dst ptr: " << static_cast<void *>(buffer + position + elementCount) << " size of T is: " << sizeof(T) << std::endl;
  }

  inline bool checkMemory(uint32_t requiredSpace)
  {
    if (m_size + requiredSpace > m_reservedSize)
    {
      reallocate_memory([=]() -> uint32_t {
        uint32_t retMem = (m_reservedSize == 0) ? 1 : m_reservedSize * 2;
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
  inline bool checkMemory()
  {
    return checkMemory(1);
  }

  inline void reallocate_memory(uint32_t newSize)
  {

    T *tmp = a.allocate(newSize);
      //  std::cout<<"--- memory reallocation to: "<<static_cast<void*>(tmp)<<" --- from "<< static_cast<void*>(buffer)<< "size: "<<newSize <<std::endl;
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
        //std::cout<<"constructing non-trival" <<std::endl;
       for (uint32_t i = 0; i < m_size; i++)
        {
         a.construct(tmp+i, std::move(buffer[i]));
        }
      }
      a.deallocate(buffer, m_reservedSize);
    }
    buffer = tmp;
    m_reservedSize = newSize;
  };
  void log(std::string logContent)
  {
    if(enableLogging)
    {
      std::cout<<logContent<<std::endl;
    }
  }
  Alloc a;
  T *buffer;
  uint32_t m_size = 0;
  uint32_t m_reservedSize = 0;
  bool isTrivial = std::is_trivially_copyable<T>::value;
  bool enableLogging = false;
};