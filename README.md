# dynamic-RMQ
Dynamic array supporting Range Minimum Queries.

Data structure that represent a dynamic array supporting Range Minimum Queries. The data structure is built over an AVL tree [1] (code inspired by [1]). Each node stores its rank in the sub-array represented by its subtree, and the minimum value in the same sub-array. 

# Supported operations

In all operations `rank` is 0-based.

- `[](rank)`: Access the value in position `rank` in the array.
- `insert(rank, value)`: Inserts the value `value` before the element in position `rank` in the array.
- `update(rank, value)`: Updates the value in position `rank` in the array to `value`.
- `(left,right)`: Returns the value of the minimum in the interval [`left`,`right`) of the array.
- `to_vector()`: Returns an std::vector containing the array.

# Compile the test executable

```console
git clone https://github.com/maxrossi91/dynamic-RMQ.git
cd dynamic-RMQ
mkdir build
cd build
cmake ..
make
```
# Run the text executable
```console
./test/avl_rmq_test
```

# Linking to your progect using `CMake` and `FetchContent`

```cmake

...

include(FetchContent)

## Add dynamic-RMQ
FetchContent_Declare(
  dynamic_rmq
  GIT_REPOSITORY https://github.com/maxrossi91/dynamic-RMQ.git
  )
  
FetchContent_GetProperties(dynamic_rmq)
if(NOT dynamic_rmq_POPULATED)
  FetchContent_Populate(dynamic_rmq)
  add_subdirectory(${dynamic_rmq_SOURCE_DIR} ${dynamic_rmq_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

...

add_executable(avl_rmq_test avl_rmq_test.cpp)
target_link_libraries(avl_rmq_test avl_rmq)

```
# Usage

The class `avl_rmq<typename K, typename S>` has two template parameters determining the type to store the key, and the type to store the values. If you want to add **satellite information** to the tree, you can safely use `std::pair<T1, T2>` as value type, as long as the `std::min()` function is properly defined.

## Example of usage

```c++
#include <iostream>
#include <avl_rmq.hpp>

int main(int argc, char *const argv[])
{


    int freq[] = {2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = sizeof(freq)/sizeof(freq[0]);

    // Instantiate the calss
    avl_rmq<int,int> avl;

    // Populate the avl_rmq
    for(size_t i = 0; i < n; ++i)
        avl.insert(i,freq[i]);

    avl.print(); // 2 1 1 3 2 3 4 5 6 7 8 9 

    std::cout << "Min in arr[1..3) is " << avl(1,3) << std::endl; // 1
    std::cout << "Min in arr[3..7) is " << avl(3,7) << std::endl; // 2

    avl.insert(0,12);
    avl.print(); // 12 2 1 1 3 2 3 4 5 6 7 8 9

    avl.update(3,12);
    avl.print(); // 12 2 12 1 3 2 3 4 5 6 7 8 9

    std::cout << "Min in arr[1..3) is " << avl(1,3) << std::endl; // 2
    std::cout << "Min in arr[6..12) is " << avl(6,12) << std::endl; // 3
    std::cout << "Value at arr[1] is " << avl[1] << std::endl; // 2

    return 0;
}
```

## Caveat

At the moment the data structure does not supports `deletions`. 

The data structure support only *minimum* queries, but can be easily adapted to supoprt also *maximum* queries. 

# Authors

### Implementation:

* [Massimiliano Rossi](https://github.com/maxrossi91)

# References

[1] Adelson-Velskii, George M., and Evgenii Mikhailovich Landis. "An algorithm for organization of information." Doklady Akademii Nauk. Vol. 146. No. 2. Russian Academy of Sciences, 1962.

[2] AVL-tree in https://www.softwaretestinghelp.com/avl-trees-and-heap-data-structure-in-cpp/