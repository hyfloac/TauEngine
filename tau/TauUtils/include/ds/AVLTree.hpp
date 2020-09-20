#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"
#include "allocator/TauAllocator.hpp"

template<typename _T, typename _HeightT>
struct AVLTree final
{
    DEFAULT_DESTRUCT(AVLTree);
    DEFAULT_CM_PU(AVLTree);
public:
    AVLTree* left;
    AVLTree* right;
    _HeightT height;
    _T value;
public:
    AVLTree(AVLTree* const _left, AVLTree* const _right, const _HeightT _height, const _T& _value) noexcept
        : left(_left)
        , right(_right)
        , height(_height)
        , value(_value)
    { }

    AVLTree(AVLTree* const _left, AVLTree* const _right, const _HeightT _height, _T&& _value) noexcept
        : left(_left)
        , right(_right)
        , height(_height)
        , value(::_TauAllocatorUtils::_move(_value))
    { }

    template<typename... _Args>
    AVLTree(AVLTree* const _left, AVLTree* const _right, const _HeightT _height, _Args&&... args) noexcept
        : left(_left)
        , right(_right)
        , height(_height)
        , value(_TauAllocatorUtils::_forward<_Args>(args)...)
    { }
};

enum class InsertMethod
{
    Ignore = 0,
    Replace,
    Greater,
    Lesser
};

template<typename _T, typename _HeightT, InsertMethod _InsertMethod = InsertMethod::Ignore>
class AVLManager final
{
    DELETE_CM(AVLManager);
public:
    using Tree = AVLTree<_T, _HeightT>;
private:
    TauAllocator& _allocator;
    Tree* _root;
public:
    AVLManager(TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept
        : _allocator(allocator)
        , _root(nullptr)
    { }

    ~AVLManager() noexcept
    {
        disposeTree();   
    }

    [[nodiscard]] Tree* root() noexcept { return _root; }
    [[nodiscard]] const Tree* root() const noexcept { return _root; }

    template<typename _Search>
    [[nodiscard]] _T* find(const _Search& search) noexcept
    {
        Tree* const node = find(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _Search>
    [[nodiscard]] const _T* find(const _Search& search) const noexcept
    {
        const Tree* const node = find(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _SearchT>
    [[nodiscard]] _T* findClosestMatchAbove(const _SearchT& search) noexcept
    {
        Tree* const node = findClosestMatchAbove(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _SearchT>
    [[nodiscard]] const _T* findClosestMatchAbove(const _SearchT& search) const noexcept
    {
        const Tree* const node = findClosestMatchAbove(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _SearchT>
    [[nodiscard]] _T* findClosestMatchBelow(const _SearchT& search) noexcept
    {
        Tree* const node = findClosestMatchBelow(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }

    template<typename _SearchT>
    [[nodiscard]] const _T* findClosestMatchBelow(const _SearchT& search) const noexcept
    {
        const Tree* const node = findClosestMatchBelow(_root, search);
        if(!node)
        { return nullptr; }
        return &node->value;
    }
    
    template<typename _Search>
    [[nodiscard]] Tree* get(const _Search& search) noexcept
    { return find(_root, search); }

    template<typename _Search>
    [[nodiscard]] const Tree* get(const _Search& search) const noexcept
    { return find(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] Tree* getClosestMatchAbove(const _SearchT& search) noexcept
    { return findClosestMatchAbove(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] const Tree* getClosestMatchAbove(const _SearchT& search) const noexcept
    { return findClosestMatchAbove(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] Tree* getClosestMatchBelow(const _SearchT& search) noexcept
    { return findClosestMatchBelow(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] const Tree* getClosestMatchBelow(const _SearchT& search) const noexcept
    { return findClosestMatchBelow(_root, search); }

    void insert(const _T& value) noexcept
    {
        if(!_root)
        {
            _root = _allocator.allocateT<Tree>(nullptr, nullptr, 0, value);
        }
        else
        {
            Tree* newNode = _allocator.allocateT<Tree>(nullptr, nullptr, 0, value);
            _root = insert(_root, newNode, _allocator);
        }
    }

    void insert(const _T&& value) noexcept
    {
        if(!_root)
        {
            _root = _allocator.allocateT<Tree>(nullptr, nullptr, 0, _TauAllocatorUtils::_move(value));
        }
        else
        {
            Tree* newNode = _allocator.allocateT<Tree>(nullptr, nullptr, 0, _TauAllocatorUtils::_move(value));
            _root = insert(_root, newNode, _allocator);
        }
    }
    
    template<typename... _Args>
    void emplace(_Args&&... args) noexcept
    {
        if(!_root)
        {
            _root = _allocator.allocateT<Tree>(nullptr, nullptr, 0, _TauAllocatorUtils::_forward<_Args>(args)...);
        }
        else
        {
            Tree* newNode = _allocator.allocateT<Tree>(nullptr, nullptr, 0, _TauAllocatorUtils::_forward<_Args>(args)...);
            _root = insert(_root, newNode, _allocator);
        }
    }

    template<typename _SearchT>
    void remove(const _SearchT& search) noexcept
    { _root = remove(&_root, search, _allocator); }

    void remove(const Tree* search) noexcept
    { _root = remove(&_root, search, _allocator); }

    void disposeTree() noexcept
    { disposeTree(_root, _allocator); }
private:
    static void disposeTree(Tree* tree, TauAllocator& allocator) noexcept
    {
        if(!tree)
        { return; }

        disposeTree(tree->left, allocator);
        disposeTree(tree->right, allocator);

        allocator.deallocateT(tree);
    }

    template<typename _TreeT, typename _SearchT>
    [[nodiscard]] static _TreeT* find(_TreeT* tree, const _SearchT& search) noexcept
    {
        _TreeT* node = tree;

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

    template<typename _TreeT, typename _SearchT>
    [[nodiscard]] static _TreeT* findClosestMatchAbove(_TreeT* tree, const _SearchT& search) noexcept
    {
        _TreeT* contender = tree;
        _TreeT* node = tree;

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

    template<typename _TreeT, typename _SearchT>
    [[nodiscard]] static _TreeT* findClosestMatchBelow(_TreeT* tree, const _SearchT& search) noexcept
    {
        _TreeT* contender = tree;
        _TreeT* node = tree;

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

    [[nodiscard]] static Tree* rotateRight(Tree* pivot) noexcept
    {
        Tree* newRoot = pivot->left;
        Tree* transferNode = newRoot->right;

        newRoot->right = pivot;
        pivot->left = transferNode;

        pivot->height = maxT(height(pivot->left), height(pivot->right)) + 1;
        newRoot->height = maxT(height(newRoot->left), height(newRoot->right)) + 1;

        return newRoot;
    }

    [[nodiscard]] static Tree* rotateLeft(Tree* pivot) noexcept
    {
        Tree* newRoot = pivot->right;
        Tree* transferNode = newRoot->left;

        newRoot->left = pivot;
        pivot->right = transferNode;

        pivot->height = maxT(height(pivot->left), height(pivot->right)) + 1;
        newRoot->height = maxT(height(newRoot->left), height(newRoot->right)) + 1;

        return newRoot;
    }

    [[nodiscard]] static _HeightT height(const Tree* tree) noexcept
    {
        if(!tree)
        { return 0; }
        return tree->height;
    }

    [[nodiscard]] static _HeightT computeBalance(const Tree* tree) noexcept
    {
        if(!tree)
        { return 0; }
        return height(tree->left) - height(tree->right);
    }

    [[nodiscard]] static Tree* insert(Tree* tree, Tree* newNode, TauAllocator& allocator) noexcept
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
            { return tree; }
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

    template<typename _SearchT>
    [[nodiscard]] static Tree* remove(Tree** rootHolder, const _SearchT& search, TauAllocator& allocator) noexcept
    {
        if(!rootHolder || !(*rootHolder))
        { return nullptr; }

        Tree* root = *rootHolder;

        if(search < root->value)
        { root->left = remove(&root->left, search, allocator); }
        else if(search > root->value)
        { root->right = remove(&root->right, search, allocator);}
        else
        {
            if(!root->left || !root->right)
            {
                Tree* tmp = root->left ? root->left : root->right;
                if(!tmp)
                { *rootHolder = nullptr; }
                else
                { *rootHolder = tmp; }
                allocator.deallocateT(root);
                root = *rootHolder;
            }
            else
            {
                Tree** tmp = minValueNode(&root->right);

                *rootHolder = *tmp;           // Replace root
                *tmp = (*tmp)->right;         // Store tmp's right branch in tmp's parent left branch
                (*rootHolder)->left = root->left;    // Set tmp's left branch to the old root's left branch
                (*rootHolder)->right = root->right;  // Set tmp's right branch to the old root's right branch
                allocator.deallocateT(root); // Destroy root 
                root = *rootHolder;           // Update the actual root variable
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

    [[nodiscard]] static Tree* remove(Tree** rootHolder, const Tree* search, TauAllocator& allocator) noexcept
    {
        if(!rootHolder || !(*rootHolder))
        { return nullptr; }

        Tree* root = *rootHolder;

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
                Tree* tmp = root->left ? root->left : root->right;
                if(!tmp)
                { *rootHolder = nullptr; }
                else
                { *rootHolder = tmp; }
                allocator.deallocateT(root);
                root = *rootHolder;
            }
            else
            {
                Tree** tmp = minValueNode(&root->right);

                *rootHolder = *tmp;           // Replace root
                *tmp = (*tmp)->right;         // Store tmp's right branch in tmp's parent left branch
                (*rootHolder)->left = root->left;    // Set tmp's left branch to the old root's left branch
                (*rootHolder)->right = root->right;  // Set tmp's right branch to the old root's right branch
                allocator.deallocateT(root); // Destroy root 
                root = *rootHolder;           // Update the actual root variable
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

    [[nodiscard]] static Tree** minValueNode(Tree** tree) noexcept
    {
        Tree* parent = nullptr;
        Tree* current = *tree;
        while(current->left)
        {
            parent = current;
            current = current->left;
        }

        if(!parent)
        { return tree; }
        return &parent->left;
    }
};


template<typename _T>
using PackedAVLTree = AVLTree<_T, i8>;

template<typename _T>
using FastAVLTree = AVLTree<_T, iSys>;

template<typename _T, InsertMethod _InsertMethod = InsertMethod::Ignore>
using PackedAVLManager = AVLManager<_T, i8, _InsertMethod>;

template<typename _T, InsertMethod _InsertMethod = InsertMethod::Ignore>
using FastAVLManager = AVLManager<_T, iSys, _InsertMethod>;
