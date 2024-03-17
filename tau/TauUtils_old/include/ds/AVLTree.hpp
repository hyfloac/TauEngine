#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"
#include "allocator/TauAllocator.hpp"
#include "TreeUtils.hpp"

template<typename _T, typename _HeightT>
struct AVLNode final
{
    DEFAULT_DESTRUCT(AVLNode);
    DEFAULT_CM_PU(AVLNode);
public:
    AVLNode* left;
    AVLNode* right;
    _HeightT height;
    _T value;
public:
    AVLNode(AVLNode* const _left, AVLNode* const _right, const _HeightT _height, const _T& _value) noexcept
        : left(_left)
        , right(_right)
        , height(_height)
        , value(_value)
    { }

    AVLNode(AVLNode* const _left, AVLNode* const _right, const _HeightT _height, _T&& _value) noexcept
        : left(_left)
        , right(_right)
        , height(_height)
        , value(::_TauAllocatorUtils::_move(_value))
    { }

    template<typename... _Args>
    AVLNode(AVLNode* const _left, AVLNode* const _right, const _HeightT _height, _Args&&... args) noexcept
        : left(_left)
        , right(_right)
        , height(_height)
        , value(_TauAllocatorUtils::_forward<_Args>(args)...)
    { }
};

template<typename _T, typename _HeightT, InsertMethod _InsertMethod = InsertMethod::Ignore>
class AVLTree final
{
    DELETE_CM(AVLTree);
public:
    using Node = AVLNode<_T, _HeightT>;
private:
    TauAllocator& _allocator;
    Node* _root;
public:
    AVLTree(TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept
        : _allocator(allocator)
        , _root(nullptr)
    { }

    ~AVLTree() noexcept
    { disposeTree(); }

    [[nodiscard]]       Node* root()       noexcept { return _root; }
    [[nodiscard]] const Node* root() const noexcept { return _root; }

    template<typename _Search>
    [[nodiscard]] _T* find(const _Search& search) noexcept
    {
        Node* const node = find(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _Search>
    [[nodiscard]] const _T* find(const _Search& search) const noexcept
    {
        const Node* const node = find(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _SearchT>
    [[nodiscard]] _T* findClosestMatchAbove(const _SearchT& search) noexcept
    {
        Node* const node = findClosestMatchAbove(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _SearchT>
    [[nodiscard]] const _T* findClosestMatchAbove(const _SearchT& search) const noexcept
    {
        const Node* const node = findClosestMatchAbove(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _SearchT>
    [[nodiscard]] _T* findClosestMatchBelow(const _SearchT& search) noexcept
    {
        Node* const node = findClosestMatchBelow(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _SearchT>
    [[nodiscard]] const _T* findClosestMatchBelow(const _SearchT& search) const noexcept
    {
        const Node* const node = findClosestMatchBelow(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }
    
    template<typename _Search>
    [[nodiscard]] Node* get(const _Search& search) noexcept
    { return find(_root, search); }

    template<typename _Search>
    [[nodiscard]] const Node* get(const _Search& search) const noexcept
    { return find(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] Node* getClosestMatchAbove(const _SearchT& search) noexcept
    { return findClosestMatchAbove(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] const Node* getClosestMatchAbove(const _SearchT& search) const noexcept
    { return findClosestMatchAbove(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] Node* getClosestMatchBelow(const _SearchT& search) noexcept
    { return findClosestMatchBelow(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] const Node* getClosestMatchBelow(const _SearchT& search) const noexcept
    { return findClosestMatchBelow(_root, search); }

    Node* insert(const _T& value) noexcept
    {
        if(!_root)
        {
            _root = _allocator.allocateT<Node>(nullptr, nullptr, 0, value);
            return _root;
        }
        else
        {
            Node* newNode = _allocator.allocateT<Node>(nullptr, nullptr, 0, value);
            _root = insert(_root, newNode, _allocator);
            return newNode;
        }
    }

    Node* insert(_T&& value) noexcept
    {
        if(!_root)
        {
            _root = _allocator.allocateT<Node>(nullptr, nullptr, 0, _TauAllocatorUtils::_move(value));
            return _root;
        }
        else
        {
            Node* newNode = _allocator.allocateT<Node>(nullptr, nullptr, 0, _TauAllocatorUtils::_move(value));
            _root = insert(_root, newNode, _allocator);
            return newNode;
        }
    }
    
    template<typename... _Args>
    Node* emplace(_Args&&... args) noexcept
    {
        if(!_root)
        {
            _root = _allocator.allocateT<Node>(nullptr, nullptr, 0, _TauAllocatorUtils::_forward<_Args>(args)...);
            return _root;
        }
        else
        {
            Node* newNode = _allocator.allocateT<Node>(nullptr, nullptr, 0, _TauAllocatorUtils::_forward<_Args>(args)...);
            _root = insert(_root, newNode, _allocator);
            return newNode;
        }
    }

    template<typename _SearchT>
    void remove(const _SearchT& search) noexcept
    { _root = remove(&_root, search, _allocator); }

    void remove(const Node* search) noexcept
    { _root = remove(&_root, search, _allocator); }

    void disposeTree() noexcept
    { disposeTree(_root, _allocator); }

    template<typename _F, IteratorMethod _IteratorMethod = IteratorMethod::TopDownLR>
    void iterate(const _F& func) noexcept
    { Iterate<Node, _F, _IteratorMethod>(_root, func); }

    template<typename _F, IteratorMethod _IteratorMethod = IteratorMethod::TopDownLR>
    void iterate(const _F& func) const noexcept
    { Iterate<Node, _F, _IteratorMethod>(_root, func); }

    template<typename _C, typename _F, IteratorMethod _IteratorMethod = IteratorMethod::TopDownLR>
    void iterate(_C* instance, const _F& func) noexcept
    { Iterate<Node, _C, _F, _IteratorMethod>(_root, instance, func); }

    template<typename _C, typename _F, IteratorMethod _IteratorMethod = IteratorMethod::TopDownLR>
    void iterate(_C* instance, const _F& func) const noexcept
    { Iterate<Node, _C, _F, _IteratorMethod>(_root, instance, func); }
private:
    template<typename _NodeT, typename _SearchT>
    [[nodiscard]] static _NodeT* find(_NodeT* const tree, const _SearchT& search) noexcept
    {
        _NodeT* node = tree;

        while(node)
        {
            if(search == node->value)
            { return node; }

            if(search > node->value)
            { node = node->right; }
            else
            { node = node->left; }
        }

        return nullptr;
    }

    template<typename _NodeT, typename _SearchT>
    [[nodiscard]] static _NodeT* findClosestMatchAbove(_NodeT* const tree, const _SearchT& search) noexcept
    {
        _NodeT* contender = tree;
        _NodeT* node = tree;

        while(node)
        {
            if(search == node->value)
            { return node; }

            if(search > node->value)
            { node = node->right; }
            else
            {
                if(contender->value > node->value)
                { contender = node; }
                node = node->left;
            }
        }

        return contender;
    }

    template<typename _NodeT, typename _SearchT>
    [[nodiscard]] static _NodeT* findClosestMatchBelow(_NodeT* const tree, const _SearchT& search) noexcept
    {
        _NodeT* contender = tree;
        _NodeT* node = tree;

        while(node)
        {
            if(search == node->value)
            { return node; }

            if(search > node->value)
            {
                if(contender->value < node->value)
                { contender = node; }
                node = node->right;
            }
            else
            { node = node->left; }
        }

        return contender;
    }

    [[nodiscard]] static Node* rotateRight(Node* const pivot) noexcept
    {
        Node* newRoot = pivot->left;
        Node* transferNode = newRoot->right;

        newRoot->right = pivot;
        pivot->left = transferNode;

        pivot->height = maxT(height(pivot->left), height(pivot->right)) + 1;
        newRoot->height = maxT(height(newRoot->left), height(newRoot->right)) + 1;

        return newRoot;
    }

    [[nodiscard]] static Node* rotateLeft(Node* const pivot) noexcept
    {
        Node* newRoot = pivot->right;
        Node* transferNode = newRoot->left;

        newRoot->left = pivot;
        pivot->right = transferNode;

        pivot->height = maxT(height(pivot->left), height(pivot->right)) + 1;
        newRoot->height = maxT(height(newRoot->left), height(newRoot->right)) + 1;

        return newRoot;
    }

    [[nodiscard]] static _HeightT height(const Node* const tree) noexcept
    {
        if(!tree)
        { return 0; }
        return tree->height;
    }

    [[nodiscard]] static int computeBalance(const Node* const tree) noexcept
    {
        if(!tree)
        { return 0; }
        return static_cast<int>(height(tree->left) - height(tree->right));
    }

    [[nodiscard]] static Node* insert(Node* const tree, Node* const newNode, TauAllocator& allocator) noexcept
    {
        if(!tree)
        { return newNode; }

        if(newNode->value < tree->value)
        { tree->left = insert(tree->left, newNode, allocator); }
        else if(newNode->value > tree->value)
        { tree->right = insert(tree->right, newNode, allocator); }
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
                newNode->height = tree->height;
                allocator.deallocateT(tree);
                return newNode;
            }
            else if constexpr(_InsertMethod == InsertMethod::Greater)
            { tree->right = insert(tree->right, newNode, allocator); }
            else if constexpr(_InsertMethod == InsertMethod::Lesser)
            { tree->left = insert(tree->left, newNode, allocator); }
            else
            { return tree; }
        }

        tree->height = maxT(height(tree->left), height(tree->right)) + 1;

        const _HeightT balance = computeBalance(tree);

        // Left Left
        if(balance > 1 && newNode->value < tree->left->value)
        { return rotateRight(tree); }

        // Right Right
        if(balance < -1 && newNode->value > tree->right->value)
        { return rotateLeft(tree); }

        // Left Right
        if(balance > 1 && newNode->value > tree->left->value)
        {
            tree->left = rotateLeft(tree->left);
            return rotateRight(tree);
        }

        // Right Left
        if(balance < -1 && newNode->value < tree->right->value)
        {
            tree->right = rotateRight(tree->right);
            return rotateLeft(tree);
        }

        return tree;
    }

    [[nodiscard]] static Node** minValueNode(Node** const tree) noexcept
    {
        Node* parent = nullptr;
        Node* current = *tree;
        while(current->left)
        {
            parent = current;
            current = current->left;
        }

        if(!parent)
        { return tree; }
        return &parent->left;
    }

    template<typename _SearchT>
    [[nodiscard]] static Node* remove(Node** const rootHolder, const _SearchT& search, TauAllocator& allocator) noexcept
    {
        if(!rootHolder || !(*rootHolder))
        { return nullptr; }

        Node* root = *rootHolder;

        if(search < root->value)
        { root->left = remove(&root->left, search, allocator); }
        else if(search > root->value)
        { root->right = remove(&root->right, search, allocator);}
        else
        {
            if(!root->left || !root->right)
            {
                Node* tmp = root->left ? root->left : root->right;
                if(!tmp)
                { *rootHolder = nullptr; }
                else
                { *rootHolder = tmp; }
                allocator.deallocateT(root);
                root = *rootHolder;
            }
            else
            {
                Node** tmp = minValueNode(&root->right);

                *rootHolder = *tmp;                 // Replace root
                *tmp = (*tmp)->right;               // Store tmp's right branch in tmp's parent left branch
                (*rootHolder)->left = root->left;   // Set tmp's left branch to the old root's left branch
                (*rootHolder)->right = root->right; // Set tmp's right branch to the old root's right branch
                allocator.deallocateT(root);        // Destroy root 
                root = *rootHolder;                 // Update the actual root variable
            }
        }

        if(!root)
        { return nullptr; }
        
        root->height = maxT(height(root->left), height(root->right)) + 1;

        const _HeightT balance = computeBalance(root);

        // Left Left
        if(balance > 1 && computeBalance(root->left) >= 0)
        { return rotateRight(root); }

        // Left Right
        if(balance > 1 && computeBalance(root->left) < 0)
        {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // Right Right
        if(balance < -1 && computeBalance(root->right) <= 0)
        { return rotateLeft(root); }

        // Right Left
        if(balance < -1 && computeBalance(root->right) > 0)
        {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    [[nodiscard]] static Node* remove(Node** const rootHolder, const Node* const search, TauAllocator& allocator) noexcept
    {
        if(!rootHolder || !(*rootHolder))
        { return nullptr; }

        Node* root = *rootHolder;

        if(search->value < root->value)
        { root->left = remove(&root->left, search, allocator); }
        else if(search->value > root->value)
        { root->right = remove(&root->right, search, allocator); }
        else if(search != root)
        {
            if constexpr(_InsertMethod == InsertMethod::Ignore || _InsertMethod == InsertMethod::Replace)
            { return root; }
            else if constexpr(_InsertMethod == InsertMethod::Greater)
            { root->right = remove(&root->right, search, allocator); }
            else if constexpr(_InsertMethod == InsertMethod::Lesser)
            { root->left = remove(&root->left, search, allocator); }
        }
        else
        {
            if(!root->left || !root->right)
            {
                Node* tmp = root->left ? root->left : root->right;
                if(!tmp)
                { *rootHolder = nullptr; }
                else
                { *rootHolder = tmp; }
                allocator.deallocateT(root);
                root = *rootHolder;
            }
            else
            {
                Node** tmp = minValueNode(&root->right);

                *rootHolder = *tmp;                 // Replace root
                *tmp = (*tmp)->right;               // Store tmp's right branch in tmp's parent left branch
                (*rootHolder)->left = root->left;   // Set tmp's left branch to the old root's left branch
                (*rootHolder)->right = root->right; // Set tmp's right branch to the old root's right branch
                allocator.deallocateT(root);        // Destroy root 
                root = *rootHolder;                 // Update the actual root variable
            }
        }

        if(!root)
        { return nullptr; }
        
        root->height = maxT(height(root->left), height(root->right)) + 1;

        const _HeightT balance = computeBalance(root);

        // Left Left
        if(balance > 1 && computeBalance(root->left) >= 0)
        { return rotateRight(root); }

        // Left Right
        if(balance > 1 && computeBalance(root->left) < 0)
        {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        // Right Right
        if(balance < -1 && computeBalance(root->right) <= 0)
        { return rotateLeft(root); }

        // Right Left
        if(balance < -1 && computeBalance(root->right) > 0)
        {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    static void disposeTree(Node* const tree, TauAllocator& allocator) noexcept
    {
        if(!tree)
        { return; }

        disposeTree(tree->left, allocator);
        disposeTree(tree->right, allocator);

        allocator.deallocateT(tree);
    }

    template<typename _Node, typename _F, IteratorMethod _IteratorMethod>
    static void Iterate(_Node* const tree, const _F& func) noexcept
    {
        if(!tree)
        { return; }

        if constexpr(_IteratorMethod == IteratorMethod::TopDownLR)
        {
            func(tree);
            Iterate<_Node, _F, _IteratorMethod>(tree->left, func);
            Iterate<_Node, _F, _IteratorMethod>(tree->right, func);
        }
        else if constexpr(_IteratorMethod == IteratorMethod::TopDownRL)
        {
            func(tree);
            Iterate<_Node, _F, _IteratorMethod>(tree->right, func);
            Iterate<_Node, _F, _IteratorMethod>(tree->left, func);
        }
        else if constexpr(_IteratorMethod == IteratorMethod::HighestToLowest)
        {
            Iterate<_Node, _F, _IteratorMethod>(tree->right, func);
            Iterate<_Node, _F, _IteratorMethod>(tree->left, func);
            func(tree);
        }
        else if constexpr(_IteratorMethod == IteratorMethod::LowestToHighest)
        {
            Iterate<_Node, _F, _IteratorMethod>(tree->left, func);
            Iterate<_Node, _F, _IteratorMethod>(tree->right, func);
            func(tree);
        }
    }

    template<typename _Node, typename _C, typename _F, IteratorMethod _IteratorMethod>
    static void Iterate(_Node* const tree, _C* const instance, const _F& func) noexcept
    {
        if(!tree)
        { return; }

        if constexpr(_IteratorMethod == IteratorMethod::TopDownLR)
        {
            (instance->*func)(tree);
            Iterate<_Node, _C, _F, _IteratorMethod>(tree->left, func);
            Iterate<_Node, _C, _F, _IteratorMethod>(tree->right, func);
        }
        else if constexpr(_IteratorMethod == IteratorMethod::TopDownRL)
        {
            (instance->*func)(tree);
            Iterate<_Node, _C, _F, _IteratorMethod>(tree->right, func);
            Iterate<_Node, _C, _F, _IteratorMethod>(tree->left, func);
        }
        else if constexpr(_IteratorMethod == IteratorMethod::HighestToLowest)
        {
            Iterate<_Node, _C, _F, _IteratorMethod>(tree->right, func);
            Iterate<_Node, _C, _F, _IteratorMethod>(tree->left, func);
            (instance->*func)(tree);
        }
        else if constexpr(_IteratorMethod == IteratorMethod::LowestToHighest)
        {
            Iterate<_Node, _C, _F, _IteratorMethod>(tree->left, func);
            Iterate<_Node, _C, _F, _IteratorMethod>(tree->right, func);
            (instance->*func)(tree);
        }
    }
};

template<typename _T>
using PackedAVLNode = AVLNode<_T, i8>;

template<typename _T>
using FastAVLNode = AVLNode<_T, iSys>;

template<typename _T, InsertMethod _InsertMethod = InsertMethod::Ignore>
using PackedAVLTree = AVLTree<_T, i8, _InsertMethod>;

template<typename _T, InsertMethod _InsertMethod = InsertMethod::Ignore>
using FastAVLTree = AVLTree<_T, iSys, _InsertMethod>;
