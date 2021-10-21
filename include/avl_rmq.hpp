////////////////////////////////////////////////////////////////////////////////
// avl_rmq.hpp
//   RMQ ALV header file.
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
   \file avl_rmq.hpp
   \brief avl_rmq.hpp Compute a dynamic RMQ
   \author Massimiliano Rossi
   \date 20/10/2021
*/

#ifndef _AVL_RMQ_HH
#define _AVL_RMQ_HH

#include <vector>
#include <assert.h>
#include <typeinfo>
#include <type_traits>
#include <limits>



template< typename T>
constexpr bool in_range_unsigned(const long long int x)
{
    return  ((x-std::numeric_limits<T>::min()) <= (std::numeric_limits<T>::max()-std::numeric_limits<T>::min()));
}

/*!
* K is the type of the keys
* S is the type of the values
* inspired from https://www.softwaretestinghelp.com/avl-trees-and-heap-data-structure-in-cpp/
*/
template< typename K, typename S>
class avl_rmq{
public:

    typedef typename std::conditional<in_range_unsigned<uint8_t>(8*sizeof(K)),uint8_t,
                typename std::conditional<in_range_unsigned<uint16_t>(8*sizeof(K)), uint16_t ,
                    typename std::conditional<in_range_unsigned<uint32_t>(8*sizeof(K)), uint32_t ,
                        uint64_t
                >::type
            >::type
        >::type d_t;


    typedef struct node_t{
        K rank;           // The ranks of the node with respect to its subtree.
        S value;          // The value of the node.
        S min;            // The minimum value of the subtree.
        d_t depth;        // The depth of the node.
        node_t *left;     // The pointer to the left child of the node.
        node_t *right;    // The pointer to the right child of the node.

        /*!
        * Costructor
        */
        node_t(K rank, S value, S min, d_t depth = 1, node_t* left = nullptr, node_t* right = nullptr):
            rank(rank),
            value(value),
            min(min),
            depth(depth),
            left(left),
            right(right)
        {

        }

        /*!
        * Desctructor
        */
        ~node_t()
        {
            if(left != nullptr) delete left;
            if(right != nullptr) delete right;
        }

        /*!
        * Tells if a node is a leaf
        * @return true if the node is a leaf, false otherwise
        */
        inline const bool is_leaf()
        {
            return (left == nullptr and right == nullptr);
        }

        /*!
        * Return the depth of the node
        * @return true if the node is a leaf, false otherwise
        */
        inline S update_min()
        {
            min = value;
            if(left != nullptr)
                min = std::min(min, left->min);
            if(right != nullptr)
                min = std::min(min, right->min);
            return min;
        }
  
    }node_t;

    /*!
    * Costructor
    */
    avl_rmq():
        root(nullptr)
    {

    }

    /*!
    * Desctructor
    */
    ~avl_rmq()
    {
        if(root != nullptr) delete root;
    }

    /*!
     * Access the array. 
     * @param rank  the rank of the element to be accessed.
     */
    S operator[](K rank)
    {
        if(root == nullptr or rank >= n_nodes )
            return 0;
        auto ret = search(root,rank);
        return ret->value;
    }

    /*!
     * Insert the value in the tree with rank rank. The elemnt with the same rank 
     * is moved on the right. 
     * @param rank  the rank of the element that has to be inserted.
     * @param value the value information attached to the element.
     */
    void insert(K rank, S value)
    {
        root = insert(root,rank,value);
        n_nodes ++;
    }

    /*!
     * Update the value in the tree with rank rank. 
     * @param rank  the rank of the element that has to be inserted.
     * @param value the value information attached to the element.
     */
    void update(K rank, S value)
    {
        update(root,rank,value);
    }

    /*!
     * Computes the minimum in the interval [left, right).
     * @param left  the left boundary interval (includisve).
     * @param right the right boundary of the interval (exclusive).
     */
    S operator ()(K left, K right)
    {
        if(left == 0 and right > n_nodes)
            return get_min(root);
        return min_range(root, left, right, (right > n_nodes));
    }


    /*!
     * Access the i-th child of the node.
     * @param i the index of the child to be returned
     * @return  the i-th child of the node if it exists.
     */
    std::vector<S> to_vector()
    {
        std::vector<S> res;
        to_vector(root, res);
        return res;
    }

    /*!
     * Print the tree.
     */
    void print()
    {
        print(root);
        std::cout << std::endl;
    }

    bool check_integrity(){

    }

  protected:

 
    /*!
     * Return the depth of the node
     * @return true if the node is a leaf, false otherwise
     */
    inline const d_t get_depth(node_t* node)
    {
        if(node == nullptr)
            return 0;
        return node->depth;
    }

    /*!
     * Return the depth of the node
     * @return true if the node is a leaf, false otherwise
     */
    inline const S get_min(node_t* node)
    {
        if(node == nullptr)
            return std::numeric_limits<S>::max();
        return node->min;
    }
  
    /*!
     * Return the depth of the node
     * @return true if the node is a leaf, false otherwise
     */
    inline const int8_t get_balance(node_t* node)
    {
        if(node == nullptr)
            return 0;
        return get_depth(node->left) - get_depth(node->right);
    }

  
    /*!
     * Rotate the subtree to the right.
     * @return the new root.
     */
    node_t* right_rotate(node_t* y)
    {
        node_t *x = y->left;  
        node_t *tmp = x->right;  
    
        // Perform rotation  
        x->right = y;  
        y->left = tmp;  

        // Update ranks
        y->rank -= x->rank;

        // Update mins
        x->min = y->min;
        y->update_min();
    
        // Update heights  
        y->depth = std::max(get_depth(y->left), get_depth(y->right)) + 1;  
        x->depth = std::max(get_depth(x->left), get_depth(x->right)) + 1;  
    
        // Return new root  
        return x;   
    }

    /*!
     * Rotate the subtree to the left.
     * @return the new root.
     */
    node_t* left_rotate(node_t* x)
    {
        node_t *y = x->right;  
        node_t *tmp = y->left;  
    
        // Perform rotation  
        y->left = x;  
        x->right = tmp;  

        // Update ranks
        y->rank += x->rank;

        // Update mins
        y->min = x->min;
        x->update_min();

        // Update heights  
        x->depth = std::max(get_depth(x->left), get_depth(x->right)) + 1;  
        y->depth = std::max(get_depth(y->left), get_depth(y->right)) + 1;  
    
        // Return new root  
        return y;   

    }

    /*!
     * Finds the element of key rank in the subtree rooted in this node.
     * @param  rank the rank of the element we look for.
     * @return      the element in the tree.
     */
    node_t* search(node_t* node, K rank)
    {
        // If node is null, create one
        if (node == nullptr)  
            return nullptr;  
    
        if (rank < node->rank)
            return search(node->left, rank);
        else if(rank > node->rank)
            return search(node->right, rank - node->rank);
        else
            return node;
    }
    

    /*!
     * Update the element of key rank in the subtree rooted in this node.
     * @param  rank  the rank of the element we look for.
     * @param  value the new value of the element.
     * @return       the element in the tree.
     */
    void update(node_t* node, K rank, const S value)
    {
        // If node is null, create one
        if (node == nullptr)  
            return;  

        const K node_rank = node->rank;

        node_t* ret = node;

        if (rank < node_rank)
            update(node->left, rank, value);
        else if(rank > node_rank)
            update(node->right, rank - node_rank, value);
        else
            node->value = value;

        // Update minimum
        node->update_min();

        return;
    }
    

    /*!
     * Insert the value in the tree with rank rank. The elemnt with the same rank 
     * is moved on the right. 
     * @param rank  the rank of the element that has to be inserted.
     * @param value the value information attached to the element.
     */
    node_t* insert(node_t* node, K rank, const S value)
    {
        // If node is null, create one
        if (node == nullptr)  
            return (new node_t(rank, value, value));  
    
        if (rank <= node->rank)
        {
            node->left = insert(node->left, rank, value);  
            node->rank++;
        }  
        else
            node->right = insert(node->right, rank - node->rank, value);  // The rank is decreased to match the one of the child

        // Update min
        node->min = std::min(node->min, value);

        // Update depth
        node->depth = std::max(get_depth(node->left),  get_depth(node->right)) + 1;  
    
        int8_t balance = get_balance(node);
    
        // Rotate if unbalanced 
    
        if (balance > 1)
        {
            // Left Right Case  
            if(rank > node->left->rank)  
                node->left = left_rotate(node->left);  

            // Left Left Case  
            return right_rotate(node);  
        } 

        if (balance < -1)
        {  
            rank -= node->rank; // Update the rank value
            // Right Left Case  
            if ( rank < node->right->rank )
                node->right = right_rotate(node->right);  

            // Right Right Case  
            return left_rotate(node);  
        }  

        return node;  
    }

    /*!
     * Computes the minimum in the interval [left, right).
     * @param node  the root of the tree.
     * @param left  the left boundary interval (includisve).
     * @param right the right boundary of the interval (exclusive).
     * @param include_right tells if the subtree has to include its right subtree.
     */
    S min_range(node_t* node, K left, K right, bool include_right = false)
    {
        // If node is null, create one
        if (node == nullptr or left == right)  
            return std::numeric_limits<S>::max();  

        const K node_rank = node->rank;
        // If the rank of the current node is larger than right, 
        // the answer is in the left child.
        if (node_rank >= right)
            return min_range(node->left,left,right);
        // If the rank of the current node is smaller than left, 
        // the answer is in the right child.
        if (node_rank < left)
            return min_range(node->right,left - node_rank,right - node_rank);

        // The range includes the current node.
        S min = node->value;
        if(include_right)
            min = std::min(min, get_min(node->right));
        // Check if the range cover the left child, use left child's min.
        if (left == 0)
            min = std::min(min, get_min(node->left));
        else
            min = std::min(min, min_range(node->left,left,node_rank, true));

        min = std::min(min, min_range(node->right,0,right - node_rank, false));


        return min;  
    }


    /*!
     * Converts the tree into a sdt::vector of the values. 
     * @param node  the root of the subtree to append.
     * @param vec   the vector to append the array.
     */
    void to_vector(node_t* node, std::vector<S>& vec)
    {
        if(node == nullptr) return;

        to_vector(node->left, vec);
        vec.push_back(node->value);
        to_vector(node->right,vec);
    }

    /*!
     * Prints the values of the tree. 
     * @param node  the root of the subtree to be print.
     */
    void print(node_t* node)
    {
        if(node == nullptr) return;

        print(node->left);
        std::cout << node->value << " ";
        print(node->right);
    }

  private:
    node_t* root;
    K n_nodes;

}; // avl_rmq



#endif /* end of include guard: _AVL_RMQ_HH */