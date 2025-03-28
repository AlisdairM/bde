// bslstl_treenodepool.h                                              -*-C++-*-
#ifndef INCLUDED_BSLSTL_TREENODEPOOL
#define INCLUDED_BSLSTL_TREENODEPOOL

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide efficient creation of nodes used in tree-based container.
//
//@CLASSES:
//   bslstl::TreeNodePool: memory manager to allocate tree nodes
//
//@SEE_ALSO: bslstl_simplepool
//
//@DESCRIPTION: This component implements a mechanism that creates and deletes
// `bslstl::TreeNode` objects for the (template parameter) type `VALUE` for use
// in a tree-based container.
//
// A `bslstl::TreeNodePool` contains a memory pool provided by the
// `bslstl_simplepool` component to provide memory for the nodes (see
// `bslstl_simplepool`).  When the pool is empty, a number of memory blocks is
// allocated and added to the pool, where each block is large enough to contain
// a `bslstl::TreeNode`.  The first allocation contains one memory block.
// Subsequent allocations double the number of memory blocks of the previous
// allocation up to an implementation defined maximum number of blocks.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Creating a `IntSet` Container
/// - - - - - - - - - - - - - - - - - - - -
// This example demonstrates how to create a container type, `IntSet` using
// `bslalg::RbTreeUtil`.
//
// First, we define a comparison functor for comparing a
// `bslstl::RbTreeNode<int>` object and an `int` value.  This functor conforms
// to the requirements of `bslalg::RbTreeUtil`:
// ```
// struct IntNodeComparator {
//     // This class defines a comparator providing comparison operations
//     // between 'bslstl::TreeNode<int>' objects, and 'int' values.
//
//   private:
//     // PRIVATE TYPES
//     typedef bslstl::TreeNode<int> Node;
//         // Alias for a node type containing an 'int' value.
//
//   public:
//     // CLASS METHODS
//     bool operator()(const bslalg::RbTreeNode& lhs, int rhs) const
//     {
//         return static_cast<const Node&>(lhs).value() < rhs;
//     }
//
//     bool operator()(int lhs, const bslalg::RbTreeNode& rhs) const
//     {
//         return lhs < static_cast<const Node&>(rhs).value();
//     }
// };
// ```
// Then, we define the public interface of `IntSet`.  Note that it contains a
// `TreeNodePool` that will be used by `bslalg::RbTreeUtil` as a `FACTORY` to
// create and delete nodes.  Also note that a number of simplifications have
// been made for the purpose of illustration.  For example, this implementation
// provides only a minimal set of critical operations, and it does not use the
// empty base-class optimization for the comparator.
// ```
// template <class ALLOCATOR = bsl::allocator<int> >
// class IntSet {
//     // This class implements a set of (unique) 'int' values.
//
//     // PRIVATE TYPES
//     typedef bslstl::TreeNodePool<int, ALLOCATOR> TreeNodePool;
//
//     // DATA
//     bslalg::RbTreeAnchor d_tree;      // tree of node objects
//     TreeNodePool         d_nodePool;  // allocator for node objects
//
//     // NOT IMPLEMENTED
//     IntSet(const IntSet&);
//     IntSet& operator=(const IntSet&);
//
//   public:
//     // CREATORS
//     IntSet(const ALLOCATOR& allocator = ALLOCATOR());
//         // Create an empty set.  Optionally specify an 'allocator' used to
//         // supply memory.  If 'allocator' is not specified, a default
//         // constructed 'ALLOCATOR' object is used.
//
//     //! ~IntSet() = 0;
//         // Destroy this object.
//
//     // MANIPULATORS
//     void insert(int value);
//         // Insert the specified 'value' into this set.
//
//     bool remove(int value);
//         // If 'value' is a member of this set, then remove it from the set
//         // and return 'true'.  Otherwise, return 'false' with no effect.
//
//     // ACCESSORS
//     bool isElement(int value) const;
//         // Return 'true' if the specified 'value' is a member of this set,
//         // and 'false' otherwise.
//
//     int numElements() const;
//         // Return the number of elements in this set.
// };
// ```
// Now, we implement the methods of `IntSet` using `RbTreeUtil`.
// ```
// // CREATORS
// template <class ALLOCATOR>
// inline
// IntSet<ALLOCATOR>::IntSet(const ALLOCATOR& allocator)
// : d_tree()
// , d_nodePool(allocator)
// {
// }
//
// // MANIPULATORS
// template <class ALLOCATOR>
// void IntSet<ALLOCATOR>::insert(int value)
// {
//     int comparisonResult;
//     bslalg::RbTreeNode *parent =
//         bslalg::RbTreeUtil::findUniqueInsertLocation(&comparisonResult,
//                                                      &d_tree,
//                                                      IntNodeComparator(),
//                                                      value);
// ```
// Here we use the `TreeNodePool` object, `d_nodePool`, to create the node that
// was inserted into the set.
// ```
//     if (0 != comparisonResult) {
//         bslalg::RbTreeNode *node = d_nodePool.emplaceIntoNewNode(value);
//         bslalg::RbTreeUtil::insertAt(&d_tree,
//                                      parent,
//                                      comparisonResult < 0,
//                                      node);
//     }
// }
//
// template <class ALLOCATOR>
// bool IntSet<ALLOCATOR>::remove(int value)
// {
//     IntNodeComparator comparator;
//     bslalg::RbTreeNode *node =
//               bslalg::RbTreeUtil::find(d_tree, comparator, value);
// ```
// Here we use the `TreeNodePool` object, `d_nodePool`, to delete a node that
// was removed from the set.
// ```
//     if (node) {
//         bslalg::RbTreeUtil::remove(&d_tree, node);
//         d_nodePool.deleteNode(node);
//     }
//     return node;
// }
//
// // ACCESSORS
// template <class ALLOCATOR>
// inline
// bool IntSet<ALLOCATOR>::isElement(int value) const
// {
//     return bslalg::RbTreeUtil::find(d_tree, IntNodeComparator(), value);
// }
//
// template <class ALLOCATOR>
// inline
// int IntSet<ALLOCATOR>::numElements() const
// {
//     return d_tree.numNodes();
// }
// ```
// Finally, we create a sample `IntSet` object and insert 3 values into the
// `IntSet`.  We verify the attributes of the `Set` before and after each
// insertion.
// ```
// bslma::TestAllocator defaultAllocator("defaultAllocator");
// bslma::DefaultAllocatorGuard defaultGuard(&defaultAllocator);
//
// bslma::TestAllocator objectAllocator("objectAllocator");
//
// IntSet<bsl::allocator<int> > set(&objectAllocator);
// assert(0 == defaultAllocator.numBytesInUse());
// assert(0 == objectAllocator.numBytesInUse());
// assert(0 == set.numElements());
//
// set.insert(1);
// assert(set.isElement(1));
// assert(1 == set.numElements());
//
// set.insert(1);
// assert(set.isElement(1));
// assert(1 == set.numElements());
//
// set.insert(2);
// assert(set.isElement(1));
// assert(set.isElement(2));
// assert(2 == set.numElements());
//
// assert(0 == defaultAllocator.numBytesInUse());
// assert(0 <  objectAllocator.numBytesInUse());
// ```

#include <bslscm_version.h>

#include <bslstl_simplepool.h>
#include <bslstl_treenode.h>

#include <bslalg_rbtreenode.h>

#include <bslma_allocatortraits.h>
#include <bslma_deallocatorproctor.h>

#include <bslmf_movableref.h>
#include <bslmf_util.h>    // 'forward(V)'

#include <bsls_compilerfeatures.h>
#include <bsls_util.h>     // 'forward<T>(V)'

#if BSLS_COMPILERFEATURES_SIMULATE_CPP11_FEATURES
// clang-format off
// Include version that can be compiled with C++03
// Generated on Mon Jan 13 08:31:39 2025
// Command line: sim_cpp11_features.pl bslstl_treenodepool.h

# define COMPILING_BSLSTL_TREENODEPOOL_H
# include <bslstl_treenodepool_cpp03.h>
# undef COMPILING_BSLSTL_TREENODEPOOL_H

// clang-format on
#else

namespace BloombergLP {
namespace bslstl {

                       // ==================
                       // class TreeNodePool
                       // ==================

/// This class provides methods for creating and deleting nodes using the
/// appropriate allocator traits of the (template parameter) type
/// `ALLOCATOR`.  This type is intended to be used as a private base-class
/// for a node-based container, in order to take advantage of the
/// empty-base-class optimization in the case where the base class has 0
/// size (as may be the case if the (template parameter) type `ALLOCATOR` is
/// not a `bslma::Allocator`).
template <class VALUE, class ALLOCATOR>
class TreeNodePool {

    /// Alias for the memory pool allocator.
    typedef SimplePool<TreeNode<VALUE>, ALLOCATOR> Pool;

    /// Alias for the allocator traits defined by `SimplePool`.
    typedef typename Pool::AllocatorTraits         AllocatorTraits;

    /// This typedef is a convenient alias for the utility associated with
    /// movable references.
    typedef bslmf::MovableRefUtil                  MoveUtil;

    // DATA
    Pool d_pool;  // pool for allocating memory

  private:
    // NOT IMPLEMENTED
    TreeNodePool(const TreeNodePool&);
    TreeNodePool& operator=(const TreeNodePool&);
    TreeNodePool& operator=(bslmf::MovableRef<TreeNodePool>);

  public:
    // PUBLIC TYPE

    /// Alias for the allocator type defined by `SimplePool`.
    typedef typename Pool::AllocatorType AllocatorType;

    /// Alias for the `size_type` of the allocator defined by `SimplePool`.
    typedef typename AllocatorTraits::size_type size_type;

  public:
    // CREATORS

    /// Create a node-pool that will use the specified `allocator` to supply
    /// memory for allocated node objects.
    explicit TreeNodePool(const ALLOCATOR& allocator);

    /// Create a node-pool, adopting all outstanding memory allocations
    /// associated with the specified `original` node-pool, that will use
    /// the allocator associated with `original` to supply memory for
    /// allocated node objects.  `original` is left in a valid but
    /// unspecified state.
    TreeNodePool(bslmf::MovableRef<TreeNodePool> original);

    // MANIPULATORS

    /// Adopt all outstanding memory allocations associated with the
    /// specified node `pool`.  The behavior is undefined unless this pool
    /// uses the same allocator as that associated with `pool`.  The
    /// behavior is also undefined unless this pool is in the
    /// default-constructed state.
    void adopt(bslmf::MovableRef<TreeNodePool> pool);

    /// Return a reference providing modifiable access to the rebound
    /// allocator traits for the node-type.  Note that this operation
    /// returns a base-class (`NodeAlloc`) reference to this object.
    AllocatorType& allocator();

    /// Allocate a node object and copy-construct an object of the (template
    /// parameter) type `VALUE` having the same value as the specified
    /// `original` at the `value` attribute of the node.  Return the address
    /// of the newly allocated node.  The behavior is undefined unless
    /// `original` refers to a `TreeNode<VALUE>` object holding a valid
    /// (initialized) value.
    bslalg::RbTreeNode *cloneNode(const bslalg::RbTreeNode& original);

#if !BSLS_COMPILERFEATURES_SIMULATE_CPP11_FEATURES
    /// Allocate a node with a newly created value object of the (template
    /// parameter) type `VALUE`, constructed by forwarding `allocator()` and
    /// the specified (variable number of) `arguments` to the corresponding
    /// constructor of `VALUE`.  Return the address of the newly allocated
    /// node.  This operation requires that `VALUE` be constructible from
    /// `arguments`.
    template <class... Args>
    bslalg::RbTreeNode *emplaceIntoNewNode(Args&&... args);
#endif

    /// Destroy the `VALUE` value of the specified `node` and return the
    /// memory footprint of `node` to this pool for potential reuse.  The
    /// behavior is undefined unless `node` refers to a `TreeNode<VALUE>`.
    void deleteNode(bslalg::RbTreeNode *node);

    /// Allocate a node of the type `TreeNode<VALUE>`, and move-construct an
    /// object of the (template parameter) type `VALUE` with the (explicitly
    /// moved) value indicated by the `value` attribute of the specified
    /// `original` node.  Return the address of the newly allocated node.
    /// The object referred to by the `value` attribute of `original` is
    /// left in a valid but unspecified state.  The behavior is undefined
    /// unless `original` refers to a `TreeNode<VALUE>` object holding a
    /// valid (initialized) value.
    bslalg::RbTreeNode *moveIntoNewNode(bslalg::RbTreeNode *original);

    /// Add to this pool sufficient memory to satisfy memory requests for at
    /// least the specified `numNodes`.  The additional memory is added
    /// irrespective of the amount of free memory when called.  The behavior
    /// is undefined unless `0 < numNodes`.
    void reserveNodes(size_type numNodes);

    /// Efficiently exchange the nodes of this object with those of the
    /// specified `other` object.  This method provides the no-throw
    /// exception-safety guarantee.  The behavior is undefined unless
    /// `allocator() == other.allocator()`.
    void swap(TreeNodePool& other);

    /// Efficiently exchange the nodes and allocator of this object with
    /// those of the specified `other` object.  This method provides the
    /// no-throw exception-safety guarantee, *unless* swapping the
    /// (user-supplied) allocator objects can throw.
    void swapExchangeAllocators(TreeNodePool& other);

    /// Efficiently exchange the nodes of this object with those of the
    /// specified `other` object.  This method provides the no-throw
    /// exception-safety guarantee.  The behavior is undefined unless
    /// `allocator() == other.allocator()`.
    void swapRetainAllocators(TreeNodePool& other);

    // ACCESSORS

    /// Return a reference providing non-modifiable access to the rebound
    /// allocator traits for the node-type.  Note that this operation
    /// returns a base-class (`NodeAlloc`) reference to this object.
    const AllocatorType& allocator() const;

    /// Return `true` if this object holds free (currently unused) nodes,
    /// and `false` otherwise.
    bool hasFreeNodes() const;
};

// ============================================================================
//                  TEMPLATE AND INLINE FUNCTION DEFINITIONS
// ============================================================================

                       // ------------------
                       // class TreeNodePool
                       // ------------------

// CREATORS
template <class VALUE, class ALLOCATOR>
inline
TreeNodePool<VALUE, ALLOCATOR>::TreeNodePool(const ALLOCATOR& allocator)
: d_pool(allocator)
{
}

template <class VALUE, class ALLOCATOR>
inline
TreeNodePool<VALUE, ALLOCATOR>::TreeNodePool(
                                      bslmf::MovableRef<TreeNodePool> original)
: d_pool(MoveUtil::move(MoveUtil::access(original).d_pool))
{
}

// MANIPULATORS
template <class VALUE, class ALLOCATOR>
inline
void
TreeNodePool<VALUE, ALLOCATOR>::adopt(bslmf::MovableRef<TreeNodePool> pool)
{
    TreeNodePool& lvalue = pool;
    d_pool.adopt(MoveUtil::move(lvalue.d_pool));
}

template <class VALUE, class ALLOCATOR>
inline
typename SimplePool<TreeNode<VALUE>, ALLOCATOR>::AllocatorType&
TreeNodePool<VALUE, ALLOCATOR>::allocator()
{
    return d_pool.allocator();
}

template <class VALUE, class ALLOCATOR>
inline
bslalg::RbTreeNode *TreeNodePool<VALUE, ALLOCATOR>::cloneNode(
                                            const bslalg::RbTreeNode& original)
{
    return emplaceIntoNewNode(
                        static_cast<const TreeNode<VALUE>&>(original).value());
}

#if !BSLS_COMPILERFEATURES_SIMULATE_CPP11_FEATURES
template <class VALUE, class ALLOCATOR>
template <class... Args>
inline
bslalg::RbTreeNode *
TreeNodePool<VALUE, ALLOCATOR>::emplaceIntoNewNode(Args&&... args)
{
    TreeNode<VALUE> *node = d_pool.allocate();
    bslma::DeallocatorProctor<Pool> proctor(node, &d_pool);

    AllocatorTraits::construct(allocator(),
                               BSLS_UTIL_ADDRESSOF(node->value()),
                               BSLS_COMPILERFEATURES_FORWARD(Args,args)...);
    proctor.release();
    return node;
}
#endif

template <class VALUE, class ALLOCATOR>
inline
void TreeNodePool<VALUE, ALLOCATOR>::deleteNode(bslalg::RbTreeNode *node)
{
    BSLS_ASSERT(node);

    TreeNode<VALUE> *treeNode = static_cast<TreeNode<VALUE> *>(node);
    AllocatorTraits::destroy(allocator(),
                             BSLS_UTIL_ADDRESSOF(treeNode->value()));
    d_pool.deallocate(treeNode);
}

template <class VALUE, class ALLOCATOR>
inline
bslalg::RbTreeNode *
TreeNodePool<VALUE, ALLOCATOR>::moveIntoNewNode(bslalg::RbTreeNode *original)
{
    return emplaceIntoNewNode(
            MoveUtil::move(static_cast<TreeNode<VALUE> *>(original)->value()));
}

template <class VALUE, class ALLOCATOR>
inline
void TreeNodePool<VALUE, ALLOCATOR>::reserveNodes(size_type numNodes)
{
    BSLS_ASSERT_SAFE(0 < numNodes);

    d_pool.reserve(numNodes);
}

template <class VALUE, class ALLOCATOR>
inline
void TreeNodePool<VALUE, ALLOCATOR>::swap(
                                         TreeNodePool<VALUE, ALLOCATOR>& other)
{
    BSLS_ASSERT_SAFE(allocator() == other.allocator());

    d_pool.swap(other.d_pool);
}

template <class VALUE, class ALLOCATOR>
inline
void TreeNodePool<VALUE, ALLOCATOR>::swapExchangeAllocators(
                                         TreeNodePool<VALUE, ALLOCATOR>& other)
{
    d_pool.quickSwapExchangeAllocators(other.d_pool);
}

template <class VALUE, class ALLOCATOR>
inline
void TreeNodePool<VALUE, ALLOCATOR>::swapRetainAllocators(
                                         TreeNodePool<VALUE, ALLOCATOR>& other)
{
    BSLS_ASSERT_SAFE(allocator() == other.allocator());

    d_pool.quickSwapRetainAllocators(other.d_pool);
}

// ACCESSORS
template <class VALUE, class ALLOCATOR>
inline
const typename SimplePool<TreeNode<VALUE>, ALLOCATOR>::AllocatorType&
TreeNodePool<VALUE, ALLOCATOR>::allocator() const
{
    return d_pool.allocator();
}

template <class VALUE, class ALLOCATOR>
inline
bool TreeNodePool<VALUE, ALLOCATOR>::hasFreeNodes() const
{
    return d_pool.hasFreeBlocks();
}

}  // close package namespace
}  // close enterprise namespace

#endif // End C++11 code

#endif

// ----------------------------------------------------------------------------
// Copyright 2019 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
