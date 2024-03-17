#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"
#include "allocator/TauAllocator.hpp"
#include "allocator/FixedBlockAllocator.hpp"
#include "allocator/PageAllocator.hpp"
#include "TreeUtils.hpp"

#ifndef SAVL_USE_TRACKING
  #define SAVL_USE_TRACKING !defined(TAU_PRODUCTION)
#endif

template<typename _T, typename _IndexT, typename _HeightT>
class StreamedAVLTreeAllocator final
{
    DELETE_CM(StreamedAVLTreeAllocator);
public:
    static constexpr _IndexT INVALID_VALUE = static_cast<_IndexT>(-1);
    static constexpr _IndexT DESTROYED_VALUE = static_cast<_IndexT>(0xCCCCCCCCCCCCCCCC);
private:
    uSys _allocPages;

    uSys _branchReservedPages;
    uSys _heightReservedPages;
    uSys _valueReservedPages;

    uSys _branchCommittedPages;
    uSys _heightCommittedPages;
    uSys _valueCommittedPages;

    uSys _allocIndex;
    uSys _firstFree;
    uSys _lastFree;

    _IndexT* _leftTree;
    _IndexT* _rightTree;
    _HeightT* _heightTree;
    _T* _valueTree;
public:
    StreamedAVLTreeAllocator(const uSys maxElements, const uSys allocPages = 4) noexcept
        : _allocPages(nextPowerOf2(allocPages))
        , _branchReservedPages(_alignTo((maxElements * sizeof(_IndexT)) / PageAllocator::pageSize() + 1, _allocPages))
        , _heightReservedPages(_alignTo((maxElements * sizeof(_HeightT)) / PageAllocator::pageSize() + 1, _allocPages))
        , _valueReservedPages(_alignTo((maxElements * sizeof(_T)) / PageAllocator::pageSize() + 1, _allocPages))
        , _branchCommittedPages(0)
        , _heightCommittedPages(0)
        , _valueCommittedPages(0)
        , _allocIndex(0)
        , _firstFree(INVALID_VALUE)
        , _lastFree(INVALID_VALUE)
        , _leftTree(reinterpret_cast<_IndexT*>(PageAllocator::reserve(_branchReservedPages)))
        , _rightTree(reinterpret_cast<_IndexT*>(PageAllocator::reserve(_branchReservedPages)))
        , _heightTree(reinterpret_cast<_HeightT*>(PageAllocator::reserve(_heightReservedPages)))
        , _valueTree(reinterpret_cast<_T*>(PageAllocator::reserve(_valueReservedPages)))
    { }

    ~StreamedAVLTreeAllocator() noexcept
    {
        PageAllocator::free(_leftTree);
        PageAllocator::free(_rightTree);
        PageAllocator::free(_heightTree);
        PageAllocator::free(_valueTree);
    }

    [[nodiscard]] _IndexT*    leftTree() noexcept { return _leftTree;   }
    [[nodiscard]] _IndexT*   rightTree() noexcept { return _rightTree;  }
    [[nodiscard]] _HeightT* heightTree() noexcept { return _heightTree; }
    [[nodiscard]] _T*        valueTree() noexcept { return _valueTree;  }

    [[nodiscard]] const _IndexT*    leftTree() const noexcept { return _leftTree;   }
    [[nodiscard]] const _IndexT*   rightTree() const noexcept { return _rightTree;  }
    [[nodiscard]] const _HeightT* heightTree() const noexcept { return _heightTree; }
    [[nodiscard]] const _T*        valueTree() const noexcept { return _valueTree;  }

    [[nodiscard]] uSys allocate() noexcept
    {
        if(_firstFree != INVALID_VALUE)
        {
            const uSys ret = _firstFree;
            if(_firstFree == _lastFree)
            {
                _firstFree = INVALID_VALUE;
                _lastFree = INVALID_VALUE;
            }
            else
            { _firstFree = _leftTree[_firstFree]; }

            return ret;
        }

        if(!assertSize())
        { return INVALID_VALUE; }

        return _allocIndex++;
    }

    void deallocate(const uSys index) noexcept
    {
        if(index == INVALID_VALUE)
        { return; }

        _valueTree[index].~_T();

#if !defined(TAU_PRODUCITON)
        _leftTree[index] = DESTROYED_VALUE;
        _rightTree[index] = DESTROYED_VALUE;
#endif

        if(index == _allocIndex)
        {
            --_allocIndex;
            attemptRelease();
            return;
        }

        if(_lastFree != INVALID_VALUE)
        {
            leftTree()[_lastFree] = index;
            _lastFree = index;
            return;
        }

        _firstFree = index;
        _lastFree = index;
    }
private:
    [[nodiscard]] bool assertSize() noexcept
    {
        {
            const uSys branchPageBytes = _branchCommittedPages * PageAllocator::pageSize();
            if(_allocIndex + sizeof(_IndexT) > branchPageBytes)
            {
                if(_branchCommittedPages == _branchReservedPages)
                { return false; }

                (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_leftTree) + branchPageBytes, _allocPages);
                (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_rightTree) + branchPageBytes, _allocPages);
                _branchCommittedPages += _allocPages;
            }
        }

        {
            const uSys heightPageBytes = _heightCommittedPages * PageAllocator::pageSize();
            if(_allocIndex + sizeof(_HeightT) > heightPageBytes)
            {
                if(_heightCommittedPages == _heightReservedPages)
                { return false; }

                (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_heightTree) + heightPageBytes, _allocPages);
                _heightCommittedPages += _allocPages;
            }
        }

        {
            const uSys valuePageBytes = _valueCommittedPages * PageAllocator::pageSize();
            if(_allocIndex + sizeof(_T) > valuePageBytes)
            {
                if(_valueCommittedPages == _valueReservedPages)
                { return false; }

                (void) PageAllocator::commitPages(reinterpret_cast<u8*>(_valueTree) + valuePageBytes, _allocPages);
                _valueCommittedPages += _allocPages;
            }
        }

        return true;
    }

    void attemptRelease() noexcept
    {
        // If there are `_allocPages` empty pages, release the block of pages.
        {
            const uSys branchPageBytes = (_branchCommittedPages - _allocPages) * PageAllocator::pageSize();
            if(branchPageBytes - sizeof(_IndexT) <= (PageAllocator::pageSize() * _allocPages))
            {
                (void) PageAllocator::decommitPages(reinterpret_cast<u8*>(_leftTree) + branchPageBytes, _allocPages);
                (void) PageAllocator::decommitPages(reinterpret_cast<u8*>(_rightTree) + branchPageBytes, _allocPages);
                _branchCommittedPages -= _allocPages;
            }
        }

        {
            const uSys heightPageBytes = (_heightCommittedPages - _allocPages) * PageAllocator::pageSize();
            if(heightPageBytes - sizeof(_HeightT) <= (PageAllocator::pageSize() * _allocPages))
            {
                (void) PageAllocator::decommitPages(reinterpret_cast<u8*>(_heightTree) + heightPageBytes, _allocPages);
                _heightCommittedPages -= _allocPages;
            }
        }

        {
            const uSys valuePageBytes = (_valueCommittedPages - _allocPages) * PageAllocator::pageSize();
            if(valuePageBytes - sizeof(_T) <= (PageAllocator::pageSize() * _allocPages))
            {
                (void) PageAllocator::decommitPages(reinterpret_cast<u8*>(_valueTree) + valuePageBytes, _allocPages);
                _valueCommittedPages -= _allocPages;
            }
        }
    }
};

template<typename _T, typename _IndexT, typename _HeightT = _IndexT, InsertMethod _InsertMethod = InsertMethod::Ignore>
class StreamedAVLTree final
{
    DELETE_CM(StreamedAVLTree);
public:
#if SAVL_USE_TRACKING
    using FBAllocator = FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>;
#else
    using FBAllocator = FixedBlockAllocator<AllocationTracking::None>;
#endif

    static constexpr _IndexT INVALID_VALUE = static_cast<_IndexT>(-1);
private:
    StreamedAVLTreeAllocator<_T, _IndexT, _HeightT> _allocator;

    _IndexT _root;
public:
    StreamedAVLTree(const uSys maxElements) noexcept
        : _allocator(maxElements)
        , _root(INVALID_VALUE)
    { }

    ~StreamedAVLTree() noexcept
    { disposeTree(); }

    [[nodiscard]] _IndexT root() const noexcept { return _root; }

    [[nodiscard]] _IndexT*    leftTree() noexcept { return _allocator.leftTree();   }
    [[nodiscard]] _IndexT*   rightTree() noexcept { return _allocator.rightTree();  }
    [[nodiscard]] _HeightT* heightTree() noexcept { return _allocator.heightTree(); }
    [[nodiscard]] _T*        valueTree() noexcept { return _allocator.valueTree();  }

    [[nodiscard]] const _IndexT*    leftTree() const noexcept { return _allocator.leftTree();   }
    [[nodiscard]] const _IndexT*   rightTree() const noexcept { return _allocator.rightTree();  }
    [[nodiscard]] const _HeightT* heightTree() const noexcept { return _allocator.heightTree(); }
    [[nodiscard]] const _T*        valueTree() const noexcept { return _allocator.valueTree();  }

    template<typename _Search>
    [[nodiscard]] _T* find(const _Search& search) noexcept
    {
        const _IndexT node = find(_root, search);
        if(node == INVALID_VALUE)
        { return nullptr; }
        return &valueTree()[node];
    }

    template<typename _Search>
    [[nodiscard]] const _T* find(const _Search& search) const noexcept
    {
        const _IndexT node = find(_root, search);
        if(node == INVALID_VALUE)
        { return nullptr; }
        return &valueTree()[node];
    }

    template<typename _SearchT>
    [[nodiscard]] _T* findClosestMatchAbove(const _SearchT& search) noexcept
    {
        const _IndexT node = findClosestMatchAbove(_root, search);
        if(node == INVALID_VALUE)
        { return nullptr; }
        return &valueTree()[node];
    }

    template<typename _SearchT>
    [[nodiscard]] const _T* findClosestMatchAbove(const _SearchT& search) const noexcept
    {
        const _IndexT node = findClosestMatchAbove(_root, search);
        if(node == INVALID_VALUE)
        { return nullptr; }
        return &valueTree()[node];
    }

    template<typename _SearchT>
    [[nodiscard]] _T* findClosestMatchBelow(const _SearchT& search) noexcept
    {
        const _IndexT node = findClosestMatchBelow(_root, search);
        if(node == INVALID_VALUE)
        { return nullptr; }
        return &valueTree()[node];
    }

    template<typename _SearchT>
    [[nodiscard]] const _T* findClosestMatchBelow(const _SearchT& search) const noexcept
    {
        const _IndexT node = findClosestMatchBelow(_root, search);
        if(node == INVALID_VALUE)
        { return nullptr; }
        return &valueTree()[node];
    }
    
    template<typename _Search>
    [[nodiscard]] uSys get(const _Search& search) const noexcept
    { return find(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] uSys getClosestMatchAbove(const _SearchT& search) const noexcept
    { return findClosestMatchAbove(_root, search); }

    template<typename _SearchT>
    [[nodiscard]] uSys getClosestMatchBelow(const _SearchT& search) const noexcept
    { return findClosestMatchBelow(_root, search); }

    void insert(const _T& value) noexcept
    {
        if(_root == INVALID_VALUE)
        {
            _root = _allocator.allocate();
            leftTree()[_root] = INVALID_VALUE;
            rightTree()[_root] = INVALID_VALUE;
            heightTree()[_root] = 0;
            new(&valueTree()[_root]) _T(value);
        }
        else
        {
            const _IndexT newNode = _allocator.allocate();
            leftTree()[newNode] = INVALID_VALUE;
            rightTree()[newNode] = INVALID_VALUE;
            heightTree()[newNode] = 0;
            new(&valueTree()[newNode]) _T(value);

            _root = insert(_root, newNode);
        }
    }

    void insert(_T&& value) noexcept
    {
        if(_root == INVALID_VALUE)
        {
            _root = _allocator.allocate();
            leftTree()[_root] = INVALID_VALUE;
            rightTree()[_root] = INVALID_VALUE;
            heightTree()[_root] = 0;
            new(&valueTree()[_root]) _T(_TauAllocatorUtils::_move(value));
        }
        else
        {
            const _IndexT newNode = _allocator.allocate();
            leftTree()[newNode] = INVALID_VALUE;
            rightTree()[newNode] = INVALID_VALUE;
            heightTree()[newNode] = 0;
            new(&valueTree()[newNode]) _T(_TauAllocatorUtils::_move(value));

            _root = insert(_root, newNode);
        }
    }
    
    template<typename... _Args>
    void emplace(_Args&&... args) noexcept
    {
        if(_root == INVALID_VALUE)
        {
            _root = _allocator.allocate();
            leftTree()[_root] = INVALID_VALUE;
            rightTree()[_root] = INVALID_VALUE;
            heightTree()[_root] = 0;
            new(&valueTree()[_root]) _T(_TauAllocatorUtils::_forward<_Args>(args)...);
        }
        else
        {
            const _IndexT newNode = _allocator.allocate();
            leftTree()[newNode] = INVALID_VALUE;
            rightTree()[newNode] = INVALID_VALUE;
            heightTree()[newNode] = 0;
            new(&valueTree()[newNode]) _T(_TauAllocatorUtils::_forward<_Args>(args)...);

            _root = insert(_root, newNode);
        }
    }

    template<typename _SearchT>
    void remove(const _SearchT& search) noexcept
    { _root = remove(&_root, search); }

    void remove(const _IndexT search) noexcept
    { _root = remove(&_root, search); }

    void disposeTree() noexcept
    { disposeTree(_root); }
private:
    template<typename _SearchT>
    [[nodiscard]] _IndexT find(const _IndexT tree, const _SearchT& search) const noexcept
    {
        _IndexT node = tree;

        while(node != INVALID_VALUE)
        {
            const _T& value = valueTree()[node];
            if(search == value)
            { return node; }

            if(search > value)
            { node = rightTree()[node]; }
            else
            { node = leftTree()[node]; }
        }

        return INVALID_VALUE;
    }

    template<typename _SearchT>
    [[nodiscard]] _IndexT findClosestMatchAbove(const _IndexT tree, const _SearchT& search) const noexcept
    {
        _IndexT contender = tree;
        _IndexT node = tree;

        const _T* contenderValue = valueTree()[contender];

        while(node != INVALID_VALUE)
        {
            const _T& value = &valueTree()[node];
            if(search == value)
            { return node; }

            if(search > value)
            { node = rightTree()[node]; }
            else
            {
                if(*contenderValue > value)
                {
                    contender = node;
                    contenderValue = value;
                }
                node = leftTree()[node];
            }
        }

        return contender;
    }

    template<typename _SearchT>
    [[nodiscard]] _IndexT findClosestMatchBelow(const _IndexT tree, const _SearchT& search) const noexcept
    {
        _IndexT contender = tree;
        _IndexT node = tree;
        
        const _T* contenderValue = valueTree()[contender];

        while(node != INVALID_VALUE)
        {
            const _T& value = valueTree()[node];
            if(search == *value)
            { return node; }

            if(search > value)
            {
                if(*contenderValue < value)
                {
                    contender = node;
                    contenderValue = value;
                }
                node = rightTree()[node];
            }
            else
            { node = leftTree()[node]; }
        }

        return contender;
    }

    [[nodiscard]] _IndexT rotateRight(const _IndexT pivot) noexcept
    {
        const _IndexT newRoot = leftTree()[pivot];
        const _IndexT transferNode = rightTree()[newRoot];

        rightTree()[newRoot] = pivot;
        leftTree()[pivot] = transferNode;

        heightTree()[pivot] = maxT(height(leftTree()[pivot]), height(rightTree()[pivot])) + 1;
        heightTree()[newRoot] = maxT(height(leftTree()[newRoot]), height(rightTree()[newRoot])) + 1;

        return newRoot;
    }

    [[nodiscard]] _IndexT rotateLeft(const _IndexT pivot) noexcept
    {
        const _IndexT newRoot = rightTree()[pivot];
        const _IndexT transferNode = leftTree()[newRoot];

        leftTree()[newRoot] = pivot;
        rightTree()[pivot] = transferNode;

        heightTree()[pivot] = maxT(height(leftTree()[pivot]), height(rightTree()[pivot])) + 1;
        heightTree()[newRoot] = maxT(height(leftTree()[newRoot]), height(rightTree()[newRoot])) + 1;

        return newRoot;
    }

    [[nodiscard]] _HeightT height(const _IndexT tree) const noexcept
    {
        if(tree == INVALID_VALUE)
        { return 0; }
        return heightTree()[tree];
    }

    [[nodiscard]] int computeBalance(const _IndexT tree) const noexcept
    {
        if(tree == INVALID_VALUE)
        { return 0; }
        return static_cast<int>(height(leftTree()[tree]) - height(rightTree()[tree]));
    }

    [[nodiscard]] _IndexT insert(const _IndexT tree, const _IndexT newNode) noexcept
    {
        if(tree == INVALID_VALUE)
        { return newNode; }

        const _T& newValue = valueTree()[newNode];
        const _T& treeValue = valueTree()[tree];

        if(newValue < treeValue)
        { leftTree()[tree] = insert(leftTree()[tree], newNode); }
        else if(newValue > treeValue)
        { rightTree()[tree] = insert(rightTree()[tree], newNode); }
        else
        {
            if constexpr(_InsertMethod == InsertMethod::Ignore)
            {
                _allocator.deallocate(newNode);
                return tree;
            }
            else if constexpr(_InsertMethod == InsertMethod::Replace)
            {
                leftTree()[newNode] = leftTree()[tree];
                rightTree()[newNode] = rightTree()[tree];
                heightTree()[newNode] = heightTree()[tree];
                _allocator.deallocate(tree);
                return newNode;
            }
            else if constexpr(_InsertMethod == InsertMethod::Greater)
            { rightTree()[tree] = insert(rightTree()[tree], newNode); }
            else if constexpr(_InsertMethod == InsertMethod::Lesser)
            { leftTree()[tree] = insert(leftTree()[tree], newNode); }
            else
            { return tree; }
        }

        const _IndexT leftBranch = leftTree()[tree];
        const _IndexT rightBranch = rightTree()[tree];

        const _T* lValue = &valueTree()[leftBranch];
        const _T* rValue = &valueTree()[rightBranch];

        heightTree()[tree] = maxT(height(leftBranch), height(rightBranch)) + 1;

        const int balance = computeBalance(tree);

        // Left Left
        if(balance > 1 && newValue < *lValue)
        { return rotateRight(tree); }

        // Right Right
        if(balance < -1 && newValue > *rValue)
        { return rotateLeft(tree); }

        // Left Right
        if(balance > 1 && newValue > *lValue)
        {
            leftTree()[tree] = rotateLeft(leftBranch);
            return rotateRight(tree);
        }
        
        // Right Left
        if(balance < -1 && newValue < *rValue)
        {
            rightTree()[tree] = rotateRight(rightBranch);
            return rotateLeft(tree);
        }

        return tree;
    }

    [[nodiscard]] _IndexT* minValueNode(_IndexT* tree) noexcept
    {
        _IndexT parent = INVALID_VALUE;
        _IndexT current = *tree;
        while(leftTree()[current] != INVALID_VALUE)
        {
            parent = current;
            current = leftTree()[current];
        }

        if(parent == INVALID_VALUE)
        { return tree; }
        return &leftTree()[parent];
    }

    template<typename _SearchT>
    [[nodiscard]] _IndexT remove(_IndexT* rootHolder, const _SearchT& search) noexcept
    {
        if(!rootHolder || *rootHolder == INVALID_VALUE)
        { return INVALID_VALUE; }

        _IndexT root = *rootHolder;

        _T& rValue = valueTree()[root];

        _IndexT* rLeft = &leftTree()[root];
        _IndexT* rRight = &rightTree()[root];

        if(search < rValue)
        { *rLeft = remove(rLeft, search); }
        else if(search > rValue)
        { *rRight = remove(rRight, search);}
        else
        {
            if(*rLeft == INVALID_VALUE || *rRight == INVALID_VALUE)
            {
                _IndexT tmp = *rLeft != INVALID_VALUE ? *rLeft : *rRight;
                if(tmp == INVALID_VALUE)
                { *rootHolder = INVALID_VALUE; }
                else
                { *rootHolder = tmp; }
                _allocator.deallocate(root);
                root = *rootHolder;
            }
            else
            {
                _IndexT* tmp = minValueNode(rRight);

                *rootHolder = *tmp;                 // Replace root
                *tmp = rightTree()[*tmp];           // Store tmp's right branch in tmp's parent left branch
                leftTree()[*rootHolder] = *rLeft;   // Set tmp's left branch to the old root's left branch
                rightTree()[*rootHolder] = *rRight; // Set tmp's right branch to the old root's right branch
                _allocator.deallocate(root);        // Destroy root 
                root = *rootHolder;                 // Update the actual root variable
            }

            rLeft = &leftTree()[root];
            rRight = &rightTree()[root];
        }

        if(root == INVALID_VALUE)
        { return INVALID_VALUE; }
        
        heightTree()[root] = maxT(height(*rLeft), height(*rRight)) + 1;

        const int balance = computeBalance(root);

        // Left Left
        if(balance > 1 && computeBalance(*rLeft) >= 0)
        { return rotateRight(root); }

        // Left Right
        if(balance > 1 && computeBalance(*rLeft) < 0)
        {
            *rLeft = rotateLeft(*rLeft);
            return rotateRight(root);
        }

        // Right Right
        if(balance < -1 && computeBalance(*rRight) <= 0)
        { return rotateLeft(root); }

        // Right Left
        if(balance < -1 && computeBalance(*rRight) > 0)
        {
            *rRight = rotateRight(*rRight);
            return rotateLeft(root);
        }

        return root;
    }

    [[nodiscard]] _IndexT remove(_IndexT* rootHolder, const _IndexT search) noexcept
    {
        if(!rootHolder || *rootHolder == INVALID_VALUE)
        { return INVALID_VALUE; }

        _IndexT root = *rootHolder;

        _T& rValue = valueTree()[root];

        _IndexT* rLeft = &leftTree()[root];
        _IndexT* rRight = &rightTree()[root];

        if(search->value < rValue)
        { *rLeft = remove(rLeft, search); }
        else if(search->value > rValue)
        { *rRight = remove(rRight, search); }
        else if(search != root)
        {
            if constexpr(_InsertMethod == InsertMethod::Ignore || _InsertMethod == InsertMethod::Replace)
            { return root; }
            else if constexpr(_InsertMethod == InsertMethod::Greater)
            { *rRight = remove(rRight, search); }
            else if constexpr(_InsertMethod == InsertMethod::Lesser)
            { *rLeft = remove(rLeft, search); }
        }
        else
        {
            if(*rLeft == INVALID_VALUE || *rRight == INVALID_VALUE)
            {
                _IndexT tmp = *rLeft != INVALID_VALUE ? *rLeft : *rRight;
                if(tmp == INVALID_VALUE)
                { *rootHolder = INVALID_VALUE; }
                else
                { *rootHolder = tmp; }
                _allocator.deallocate(root);
                root = *rootHolder;
            }
            else
            {
                _IndexT* tmp = minValueNode(rRight);

                *rootHolder = *tmp;                 // Replace root
                *tmp = rightTree()[*tmp];           // Store tmp's right branch in tmp's parent left branch
                leftTree()[*rootHolder] = *rLeft;   // Set tmp's left branch to the old root's left branch
                rightTree()[*rootHolder] = *rRight; // Set tmp's right branch to the old root's right branch
                _allocator.deallocate(root);        // Destroy root 
                root = *rootHolder;                 // Update the actual root variable
            }
        }
        
        if(root == INVALID_VALUE)
        { return INVALID_VALUE; }
        
        heightTree()[root] = maxT(height(*rLeft), height(*rRight)) + 1;

        const int balance = computeBalance(root);
        
        // Left Left
        if(balance > 1 && computeBalance(*rLeft) >= 0)
        { return rotateRight(root); }

        // Left Right
        if(balance > 1 && computeBalance(*rLeft) < 0)
        {
            *rLeft = rotateLeft(*rLeft);
            return rotateRight(root);
        }

        // Right Right
        if(balance < INVALID_VALUE && computeBalance(*rRight) <= 0)
        { return rotateLeft(root); }

        // Right Left
        if(balance < INVALID_VALUE && computeBalance(*rRight) > 0)
        {
            *rRight = rotateRight(*rRight);
            return rotateLeft(root);
        }

        return root;
    }

    void disposeTree(const uSys tree) noexcept
    {
        if(tree == INVALID_VALUE)
        { return; }

        disposeTree(leftTree()[tree]);
        disposeTree(rightTree()[tree]);

        _allocator.deallocate(tree);
    }
};
