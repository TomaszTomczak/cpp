### Vector

A sequence container that encapsulates dynamic size array.

```
template<
    class T,
    class Allocator = std::allocator<T>
> class Vector;
```

The elements are stored contiguously, which means that elements can be accessed not only through iterators, but also using offsets to regular pointers to elements. This means that a pointer to an element of a vector may be passed to any function that expects a pointer to an element of an array.

Reallocations are usually costly operations in terms of performance. The reserve() function can be used to eliminate reallocations if the number of elements is known beforehand.

The complexity (efficiency) of common operations on vectors is as follows:

Random access - constant O(1)
Insertion or removal of elements at the end - amortized constant O(1)
Insertion or removal of elements - linear in the distance to the end of the vector O(n)

## Template parameters
T	-	The type of the elements.
T must meet the requirements of CopyAssignable and CopyConstructible.

Allocator	-	An allocator that is used to acquire/release memory and to construct/destroy the elements in that memory. The type must meet the requirements of Allocator. The behavior is undefined if Allocator::value_type is not the same as T.


### Member functions

| Name          | Description |
|---------------|----------------------------------|
| [Constructor](#Constructor) | constructs the vector            |
| [Destructor](#Destructor)  | destructs the vector             |
| [operator=](#operator=)     | assigns values to the container  |
| [assign](#assign)        | assigns values to the container  |
| [get_allocator](#get_allocator) | returns the associated allocator |

#### Element access
| | |
|------|------|
|[at](#at)          | access specified element with bounds checking |
| [operator[]](operator[]) | access specified element |
| [front](#front)      | access the first element |
| [back](#back)       | access the last element |
| [data](#data)       | direct access to the underlying array |

#### Iterators
| | |
|------|------|
| [begin/cbegin](#begin)| returns an iterator to the beginning |
| [end/cend](#end) | returns an iterator to the end |
| [rbegin/crbegin](#rbegin) | returns a reverse iterator to the beginning (To be implemented)|
| [rend/crend](#rend) | return a reverse iterator to the end (To be implemented)|

#### Capacity
|||
|-|-|
| [empty](#empty) | checks if container is empty |
| [size](#size) | returns the number of elements |
| [max_size](#max_size) | not implemented |
| [reserve](#reserve) | reserves storage |
| [capacity](#capacity) | returns the number of elements that can be held in current allocated storage |
| [shrink_to_fit](#shrink_to_fit) | freeing unused memory |

#### Modifiers 
|||
|-|-|
| [clear](#clear) | clears the contents |
| [insert](#insert) | inserts elements |
| [emplace](#emplace) | constructs element in-place |
| [erase](#erase) | erases elements |
| [push_back](#push_back) | adds an element to the end |
| [emplace_back](#emplace_back) | constructs an element in-place at the end |
| [pop_back](#pop_back) | removes last element |
| [resize](#resize) | changes the number of elements stored |
| [swap](#swap) | swaps the contents |


## Details
### Constructor
1) 
```
Vector()
```
```
Vector(std::size_t count, const T &value, const Alloc &allocator = Alloc())
Vector(std::initializer_list<T> ilist, const Alloc &allocator = Alloc())
explicit Vector(const Alloc &alloc) noexcept
explicit Vector(std::size_t count, const Alloc &alloc = Alloc());
Vector(const Vector &other)
Vector(const Vector& other, const Alloc& alloc);
Vector(Vector &&other) noexcept;
Vector(Vector&& other, const Alloc& alloc );
template <class InputIt>
Vector(InputIt first, InputIt last, const Alloc &alloc = Alloc());
```
### Destructor
### operator=
### assign
### get_allocator
### at
### operator[]
### front
### back
### data
### begin
### end
### rbegin
### rend
### empty
### size
### max_size
### reserve
### capacity
### shrink_to_fit
### clear
### insert
### emplace
### erase
### push_back
### emplace_back
### pop_back
### resize
### swap
