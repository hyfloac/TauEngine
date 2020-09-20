#include "UnitTest.hpp"
#include "AVLTreeTest.hpp"
#include <ConPrinter.hpp>
#include <ds/AVLTree.hpp>
#include <String.hpp>

#define SHOULD_PRINT 1
#define TEST_ALL 0

namespace AVLTreeUnitTest {

struct Trunk
{
	const Trunk *prev;
	DynString str;

	Trunk(const Trunk* const _prev, const DynString& _str) noexcept
	    : prev(_prev)
        , str(_str)
	{ }
};

void showTrunks(const Trunk* p) noexcept
{
	if(!p)
    { return; }

	showTrunks(p->prev);

	ConPrinter::print(p->str);
}

template<typename _T>
inline void printTree(const FastAVLTree<_T>* root, Trunk* const prev, const bool isRight) noexcept
{
	if(!root)
	{ return; }

	DynString prev_str("    ");
	Trunk trunk(prev, prev_str);
    
	printTree(root->right, &trunk, true);

	if(!prev)
    { trunk.str = "----"; }
	else if(isRight)
	{
		trunk.str = ".---";
		prev_str = "   |";
	}
	else
	{
		trunk.str = "`---";
		prev->str = prev_str;
	}

	showTrunks(&trunk);
	ConPrinter::print("%\n", root->value);

	if(prev)
    { prev->str = prev_str; }
	trunk.str = "   |";

	printTree(root->left, &trunk, false);
}

template<typename _T>
inline void printBT(const FastAVLTree<_T>* node) noexcept
{
#if SHOULD_PRINT
    // printBT("", node, false);
    printTree(node, nullptr, false);
    ConPrinter::print("\n");
#endif
}

template<typename _Manager>
void insertTest() noexcept
{
    UNIT_TEST();

    _Manager manager;

    manager.insert(12);
    manager.insert(17);
    manager.insert(7);
    manager.insert(23);
    manager.insert(64);
    manager.insert(19);
    manager.insert(3);
    manager.insert(50);
    manager.insert(13);
    manager.insert(21);
    manager.insert(72);
    manager.insert(53);
}

template<typename _Manager>
void insertDuplicateTest() noexcept
{
    UNIT_TEST();

    _Manager manager;

    manager.insert(12);
    manager.insert(17);
    manager.insert(12);
    manager.insert(7);
    manager.insert(23);
    manager.insert(64);
    manager.insert(19);
    manager.insert(3);
    manager.insert(7);
    manager.insert(23);
    manager.insert(50);
    manager.insert(13);
    manager.insert(21);
    manager.insert(3);
    manager.insert(7);
    manager.insert(72);
    manager.insert(53);
}

template<typename _Manager>
void emplaceTest() noexcept
{
    _Manager manager;

    manager.emplace(12);
    manager.emplace(17);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(64);
    manager.emplace(19);
    manager.emplace(3);
    manager.emplace(50);
    manager.emplace(13);
    manager.emplace(21);
    manager.emplace(72);
    manager.emplace(53);
}

template<typename _Manager>
void emplaceDuplicateTest() noexcept
{
    _Manager manager;

    manager.emplace(12);
    manager.emplace(17);
    manager.emplace(12);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(64);
    manager.emplace(19);
    manager.emplace(3);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(50);
    manager.emplace(13);
    manager.emplace(21);
    manager.emplace(3);
    manager.emplace(7);
    manager.emplace(72);
    manager.emplace(53);
}

template<typename _Manager>
void findTest() noexcept
{
    _Manager manager;

    manager.emplace(12);
    manager.emplace(17);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(64);
    manager.emplace(19);
    manager.emplace(3);
    manager.emplace(50);
    manager.emplace(13);
    manager.emplace(21);
    manager.emplace(72);
    manager.emplace(53);
    
    Assert(manager.find(12));
    Assert(manager.find(17));
    Assert(manager.find(7));
    Assert(manager.find(23));
    Assert(manager.find(64));
    Assert(manager.find(19));
    Assert(manager.find(3));
    Assert(manager.find(50));
    Assert(manager.find(13));
    Assert(manager.find(21));
    Assert(manager.find(72));
    Assert(manager.find(53));
    
#if TEST_ALL
    bool anyFound = false;
    for(int i = 73; i < IntMaxMin<int>::Max; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    for(int i = IntMaxMin<int>::Min; i < 0; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    Assert(!anyFound);
#else
    Assert(!manager.find(128));
    Assert(!manager.find(-14));
    Assert(!manager.find(-53));
    Assert(!manager.find(0));
    Assert(!manager.find(2893475));
    Assert(!manager.find(-2345897));
#endif
}

template<typename _Manager>
void findDuplicateTest() noexcept
{
    _Manager manager;

    manager.emplace(12);
    manager.emplace(17);
    manager.emplace(12);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(64);
    manager.emplace(19);
    manager.emplace(3);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(50);
    manager.emplace(13);
    manager.emplace(21);
    manager.emplace(3);
    manager.emplace(7);
    manager.emplace(72);
    manager.emplace(53);
    
    Assert(manager.find(12));
    Assert(manager.find(17));
    Assert(manager.find(7));
    Assert(manager.find(23));
    Assert(manager.find(64));
    Assert(manager.find(19));
    Assert(manager.find(3));
    Assert(manager.find(50));
    Assert(manager.find(13));
    Assert(manager.find(21));
    Assert(manager.find(72));
    Assert(manager.find(53));

#if TEST_ALL
    bool anyFound = false;
    for(int i = 73; i < IntMaxMin<int>::Max; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    for(int i = IntMaxMin<int>::Min; i < 0; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    Assert(!anyFound);
#else
    Assert(!manager.find(128));
    Assert(!manager.find(-14));
    Assert(!manager.find(-53));
    Assert(!manager.find(0));
    Assert(!manager.find(2893475));
    Assert(!manager.find(-2345897));
#endif
}

template<typename _Manager>
void removeTest() noexcept
{
    _Manager manager;

    manager.emplace(12);
    manager.emplace(17);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(64);
    manager.emplace(19);
    manager.emplace(3);
    manager.emplace(50);
    manager.emplace(13);
    manager.emplace(21);
    manager.emplace(72);
    manager.emplace(53);
    
    manager.remove(7);
    manager.remove(64);
    manager.remove(21);
    manager.remove(123);

    Assert(manager.find(12));
    Assert(manager.find(17));
    Assert(manager.find(23));
    Assert(manager.find(19));
    Assert(manager.find(3));
    Assert(manager.find(50));
    Assert(manager.find(13));
    Assert(manager.find(72));
    Assert(manager.find(53));

    Assert(!manager.find(7));
    Assert(!manager.find(64));
    Assert(!manager.find(21));

#if TEST_ALL
    bool anyFound = false;
    for(int i = 73; i < IntMaxMin<int>::Max; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    for(int i = IntMaxMin<int>::Min; i < 0; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    Assert(!anyFound);
#else
    Assert(!manager.find(128));
    Assert(!manager.find(-14));
    Assert(!manager.find(-53));
    Assert(!manager.find(0));
    Assert(!manager.find(2893475));
    Assert(!manager.find(-2345897));
#endif
}

template<typename _Manager>
void removeDuplicateTest() noexcept
{
    _Manager manager;

    manager.emplace(12);
    manager.emplace(17);
    manager.emplace(12);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(64);
    manager.emplace(19);
    manager.emplace(3);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(50);
    manager.emplace(13);
    manager.emplace(21);
    manager.emplace(3);
    manager.emplace(7);
    manager.emplace(72);
    manager.emplace(53);

    manager.remove(7);
    manager.remove(64);
    manager.remove(21);
    manager.remove(123);
    
    Assert(manager.find(12));
    Assert(manager.find(17));
    Assert(manager.find(23));
    Assert(manager.find(19));
    Assert(manager.find(3));
    Assert(manager.find(50));
    Assert(manager.find(13));
    Assert(manager.find(72));
    Assert(manager.find(53));

    Assert(!manager.find(7));
    Assert(!manager.find(64));
    Assert(!manager.find(21));

#if TEST_ALL
    bool anyFound = false;
    for(int i = 73; i < IntMaxMin<int>::Max; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    for(int i = IntMaxMin<int>::Min; i < 0; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    Assert(!anyFound);
#else
    Assert(!manager.find(128));
    Assert(!manager.find(-14));
    Assert(!manager.find(-53));
    Assert(!manager.find(0));
    Assert(!manager.find(2893475));
    Assert(!manager.find(-2345897));
#endif
}

template<typename _Manager>
void removeDuplicateMultipleTest() noexcept
{
    _Manager manager;

    manager.emplace(12);
    manager.emplace(17);
    manager.emplace(12);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(64);
    manager.emplace(19);
    manager.emplace(3);
    manager.emplace(7);
    manager.emplace(23);
    manager.emplace(50);
    manager.emplace(13);
    manager.emplace(21);
    manager.emplace(3);
    manager.emplace(7);
    manager.emplace(72);
    manager.emplace(53);

    manager.remove(7);
    manager.remove(64);
    manager.remove(21);
    manager.remove(123);
    
    Assert(manager.find(12));
    Assert(manager.find(17));
    Assert(manager.find(23));
    Assert(manager.find(19));
    Assert(manager.find(3));
    Assert(manager.find(50));
    Assert(manager.find(13));
    Assert(manager.find(72));
    Assert(manager.find(53));

    Assert(manager.find(7));

    Assert(!manager.find(64));
    Assert(!manager.find(21));

#if TEST_ALL
    bool anyFound = false;
    for(int i = 73; i < IntMaxMin<int>::Max; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    for(int i = IntMaxMin<int>::Min; i < 0; ++i)
    {
        if(manager.find(i))
        {
            anyFound = true;
            break;
        }
    }
    Assert(!anyFound);
#else
    Assert(!manager.find(128));
    Assert(!manager.find(-14));
    Assert(!manager.find(-53));
    Assert(!manager.find(0));
    Assert(!manager.find(2893475));
    Assert(!manager.find(-2345897));
#endif
}

void insertIgnoreTest() noexcept
{
    UNIT_TEST();
    insertTest<FastAVLManager<int, InsertMethod::Ignore>>();
}

void insertDuplicateIgnoreTest() noexcept
{
    UNIT_TEST();
    insertDuplicateTest<FastAVLManager<int, InsertMethod::Ignore>>();
}

void emplaceIgnoreTest() noexcept
{
    UNIT_TEST();
    emplaceTest<FastAVLManager<int, InsertMethod::Ignore>>();
}

void emplaceDuplicateIgnoreTest() noexcept
{
    UNIT_TEST();
    emplaceDuplicateTest<FastAVLManager<int, InsertMethod::Ignore>>();
}

void findIgnoreTest() noexcept
{
    UNIT_TEST();
    findTest<FastAVLManager<int, InsertMethod::Ignore>>();
}

void findDuplicateIgnoreTest() noexcept
{
    UNIT_TEST();
    findDuplicateTest<FastAVLManager<int, InsertMethod::Ignore>>();
}

void removeIgnoreTest() noexcept
{
    UNIT_TEST();
    removeTest<FastAVLManager<int, InsertMethod::Ignore>>();
}

void removeDuplicateIgnoreTest() noexcept
{
    UNIT_TEST();
    removeDuplicateTest<FastAVLManager<int, InsertMethod::Ignore>>();
}

void insertReplaceTest() noexcept
{
    UNIT_TEST();
    insertTest<FastAVLManager<int, InsertMethod::Replace>>();
}

void insertDuplicateReplaceTest() noexcept
{
    UNIT_TEST();
    insertDuplicateTest<FastAVLManager<int, InsertMethod::Replace>>();
}

void emplaceReplaceTest() noexcept
{
    UNIT_TEST();
    emplaceTest<FastAVLManager<int, InsertMethod::Replace>>();
}

void emplaceDuplicateReplaceTest() noexcept
{
    UNIT_TEST();
    emplaceDuplicateTest<FastAVLManager<int, InsertMethod::Replace>>();
}

void findReplaceTest() noexcept
{
    UNIT_TEST();
    findTest<FastAVLManager<int, InsertMethod::Replace>>();
}

void findDuplicateReplaceTest() noexcept
{
    UNIT_TEST();
    findDuplicateTest<FastAVLManager<int, InsertMethod::Replace>>();
}

void removeReplaceTest() noexcept
{
    UNIT_TEST();
    removeTest<FastAVLManager<int, InsertMethod::Replace>>();
}

void removeDuplicateReplaceTest() noexcept
{
    UNIT_TEST();
    removeDuplicateTest<FastAVLManager<int, InsertMethod::Replace>>();
}

void insertGreaterTest() noexcept
{
    UNIT_TEST();
    insertTest<FastAVLManager<int, InsertMethod::Greater>>();
}

void insertDuplicateGreaterTest() noexcept
{
    UNIT_TEST();
    insertDuplicateTest<FastAVLManager<int, InsertMethod::Greater>>();
}

void emplaceGreaterTest() noexcept
{
    UNIT_TEST();
    emplaceTest<FastAVLManager<int, InsertMethod::Greater>>();
}

void emplaceDuplicateGreaterTest() noexcept
{
    UNIT_TEST();
    emplaceDuplicateTest<FastAVLManager<int, InsertMethod::Greater>>();
}

void findGreaterTest() noexcept
{
    UNIT_TEST();
    findTest<FastAVLManager<int, InsertMethod::Greater>>();
}

void findDuplicateGreaterTest() noexcept
{
    UNIT_TEST();
    findDuplicateTest<FastAVLManager<int, InsertMethod::Greater>>();
}

void removeGreaterTest() noexcept
{
    UNIT_TEST();
    removeTest<FastAVLManager<int, InsertMethod::Greater>>();
}

void removeDuplicateGreaterTest() noexcept
{
    UNIT_TEST();
    removeDuplicateMultipleTest<FastAVLManager<int, InsertMethod::Greater>>();
}

void insertLesserTest() noexcept
{
    UNIT_TEST();
    insertTest<FastAVLManager<int, InsertMethod::Lesser>>();
}

void insertDuplicateLesserTest() noexcept
{
    UNIT_TEST();
    insertDuplicateTest<FastAVLManager<int, InsertMethod::Lesser>>();
}

void emplaceLesserTest() noexcept
{
    UNIT_TEST();
    emplaceTest<FastAVLManager<int, InsertMethod::Lesser>>();
}

void emplaceDuplicateLesserTest() noexcept
{
    UNIT_TEST();
    emplaceDuplicateTest<FastAVLManager<int, InsertMethod::Lesser>>();
}

void findLesserTest() noexcept
{
    UNIT_TEST();
    findTest<FastAVLManager<int, InsertMethod::Lesser>>();
}

void findDuplicateLesserTest() noexcept
{
    UNIT_TEST();
    findDuplicateTest<FastAVLManager<int, InsertMethod::Lesser>>();
}

void removeLesserTest() noexcept
{
    UNIT_TEST();
    removeTest<FastAVLManager<int, InsertMethod::Lesser>>();
}

void removeDuplicateLesserTest() noexcept
{
    UNIT_TEST();
    removeDuplicateMultipleTest<FastAVLManager<int, InsertMethod::Lesser>>();
}

}
