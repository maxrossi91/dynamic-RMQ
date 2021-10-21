////////////////////////////////////////////////////////////////////////////////
// avl_rmq_test.cpp
//   Test the avl rmq tree.
////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Massimiliano Rossi
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

/*!
   \file avl_rmq_test.cpp
   \brief avl_rmq_test.cpp Test the AVL rmq tree.
   \author Massimiliano Rossi
   \date 20/10/2021
*/

#include <iostream>
#include <avl_rmq.hpp>
#include <malloc_count.h>

int main(int argc, char *const argv[])
{

    int freq[] = {2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = sizeof(freq)/sizeof(freq[0]);

    avl_rmq<int,int> avl;

    for(size_t i = 0; i < n; ++i)
        avl.insert(i,freq[i]);

    avl.print();

    std::cout << "Min in arr[1..3) is " << avl(1,3) << std::endl; // 1
    std::cout << "Min in arr[3..7) is " << avl(3,7) << std::endl; // 2

    avl.insert(0,12);
    avl.print();

    avl.update(2,12);
    avl.print();

    std::cout << "Min in arr[1..3) is " << avl(1,3) << std::endl; // 2
    std::cout << "Min in arr[6..12) is " << avl(6,12) << std::endl; // 3
    std::cout << "Value at arr[1] is " << avl[1] << std::endl; // 2
    
    return 0;
}