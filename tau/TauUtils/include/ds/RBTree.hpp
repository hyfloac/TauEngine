#pragma once

#include "Objects.hpp"
#include "allocator/TauAllocator.hpp"
#include "TreeUtils.hpp"

template<typename _T, typename _ColorT>
struct RBNode final
{
    DEFAULT_DESTRUCT(RBNode);
    DEFAULT_CM_PU(RBNode);
public:
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    _ColorT color;
    _T value;
public:
    RBNode(RBNode* const _left, RBNode* const _right, RBNode* const _parent, const _ColorT _height, const _T& _value) noexcept
        : left(_left)
        , right(_right)
        , parent(_parent)
        , color(_height)
        , value(_value)
    { }

    RBNode(RBNode* const _left, RBNode* const _right, RBNode* const _parent, const _ColorT _height, _T&& _value) noexcept
        : left(_left)
        , right(_right)
        , parent(_parent)
        , color(_height)
        , value(::_TauAllocatorUtils::_move(_value))
    { }

    template<typename... _Args>
    RBNode(RBNode* const _left, RBNode* const _right, RBNode* const _parent, const _ColorT _color, _Args&&... args) noexcept
        : left(_left)
        , right(_right)
        , parent(_parent)
        , color(_color)
        , value(_TauAllocatorUtils::_forward<_Args>(args)...)
    { }
};

template<typename _T, typename _ColorT, InsertMethod _InsertMethod = InsertMethod::Ignore>
class RBTree final
{
    DELETE_CM(RBTree);
public:
    using Node = RBNode<_T, _ColorT>;

    enum Color : bool
    {
        Red = false,
        Black = true
    };
private:
    TauAllocator& _allocator;
    Node* _root;
public:
    RBTree(TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept
        : _allocator(allocator)
        , _root(nullptr)
    { }

    ~RBTree() noexcept
    { disposeTree(); }
    
    [[nodiscard]]       Node* root()       noexcept { return _root; }
    [[nodiscard]] const Node* root() const noexcept { return _root; }

    void insert(const _T& value) noexcept
    {
        if(!_root)
        {
            _root = _allocator.allocateT<Node>(nullptr, nullptr, Black, value);
        }
        else
        {
            Node* newNode = _allocator.allocateT<Node>(nullptr, nullptr, Red, value);
            insert(root, newNode, _allocator);
        }
    }

    void disposeTree() noexcept
    { disposeTree(_root, _allocator); }
private:
    static constexpr Node* LEAF_FOUND_SIGNAL = 0x01;

    [[nodiscard]] static Node* bstInsert(Node* tree, Node* newNode, [[tau::out]] Node** parent, [[tau::out]] Node** grandparent, [[tau::out]] Node** uncle, TauAllocator& allocator) noexcept
    {
        if(!tree)
        {
            *parent = LEAF_FOUND_SIGNAL;
            *grandparent = LEAF_FOUND_SIGNAL;
            return newNode;
        }

        bool isLeft = false;

        if(newNode->value < tree->value)
        {
            tree->left = bstInsert(tree->left, newNode, parent, grandparent, allocator);
            isLeft = true;
        }
        else if(newNode->value > tree->value)
        {
            tree->right = bstInsert(tree->right, newNode, parent, grandparent, allocator);
            isLeft = false;
        }
        else
        {
            if constexpr(_InsertMethod == InsertMethod::Ignore)
            {
                allocator.deallocateT(newNode);
                return tree;
            }
            else if constexpr(_InsertMethod == InsertMethod::Replace)
            {
                newNode->left = tree->left;
                newNode->right = tree->right;
                newNode->color = tree->color;
                allocator.deallocateT(tree);
                return newNode;
            }
            else if constexpr(_InsertMethod == InsertMethod::Greater)
            {
                tree->right = insert(tree->right, newNode, allocator);
                isLeft = false;
            }
            else if constexpr(_InsertMethod == InsertMethod::Lesser)
            {
                tree->left = insert(tree->left, newNode, allocator);
                isLeft = true;
            }
            else
            {
                allocator.deallocateT(newNode);
                return tree;
            }
        }

        if(*parent == LEAF_FOUND_SIGNAL)
        { *parent = tree; }
        else if(*grandparent == LEAF_FOUND_SIGNAL)
        {
            *grandparent = tree;
            *uncle = isLeft ? tree->right : tree->left;
        }

        return newNode;
    }

    [[nodiscard]] static Node* insert(Node* tree, Node* newNode, TauAllocator& allocator) noexcept
    {
        Node* parent;
        Node* grandparent;
        Node* uncle;
        Node* root = bstInsert(tree, newNode, &parent, &grandparent, &uncle, allocator);

        if(root == newNode)
        { return root; }

        if(parent == LEAF_FOUND_SIGNAL)
        { parent = nullptr; }

        if(grandparent == LEAF_FOUND_SIGNAL)
        { grandparent = nullptr; }

        if(uncle == LEAF_FOUND_SIGNAL)
        { uncle = nullptr; }

        return root;
    }

    static void fixRedRed(Node* tree, Node* parent, Node* grandparent, Node* uncle) noexcept
    {
        if(parent->color == Red)
        {
            if(uncle && uncle->color == Red)
            {
                parent->color = Black;
                grandparent->color = Red;
                uncle->color = Black;
                fixRedRed()
            }
        }
    }

    static void disposeTree(Node* tree, TauAllocator& allocator) noexcept
    {
        if(!tree)
        { return; }

        disposeTree(tree->left, allocator);
        disposeTree(tree->right, allocator);

        allocator.deallocateT(tree);
    }
};

template<typename _T>
using PackedRBNode = RBNode<_T, bool>;

template<typename _T>
using FastRBNode = RBNode<_T, iSys>;

template<typename _T, InsertMethod _InsertMethod = InsertMethod::Ignore>
using PackedRBTree = RBTree<_T, bool, _InsertMethod>;

template<typename _T, InsertMethod _InsertMethod = InsertMethod::Ignore>
using FastRBTree = RBTree<_T, iSys, _InsertMethod>;

