### Vector

A sequence container that encapsulates dynamic size array.

```
template<
    class T,
    class Allocator = std::allocator<T>
> class vector;
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
| (constructor) | constructs the vector            |
| (destructor)  | destructs the vector             |
| operator=     | assigns values to the container  |
| assign        | assigns values to the container  |
| get_allocator | returns the associated allocator |

#### Element access
| | |
|------|------|
|at          | access specified element with bounds checking |
| operator[] | access specified element |
| front      | access the first element |
| back       | access the last element |
| data       | direct access to the underlying array |

#### Iterators
| | |
|------|------|
| begin/cbegin| returns an iterator to the beginning |
| end/cend | returns an iterator to the end |
| rbegin/crbegin | returns a reverse iterator to the beginning (To be implemented)|
| rend/crend | return a reverse iterator to the end (To be implemented)|

#### Capacity
|||
|-|-|
| empty | checks if container is empty |
| size | returns the number of elements |
| max_size | not implemented |
| reserve | reserves storage |
| capacity | returns the number of elements that can be held in current allocated storage |
| shrink_to_fit | freeing unused memory |

#### Modifiers 
|||
|-|-|
| clear | clears the contents |
| insert | inserts elements |
| emplace | constructs element in-place |
| erase | erases elements |
| push_back | adds an element to the end |
| emplace_back | constructs an element in-place at the end |
| pop_back | removes last element |
| resize | changes the number of elements stored |
| swap | swaps the contents |
