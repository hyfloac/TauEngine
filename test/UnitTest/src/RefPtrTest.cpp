#include <ReferenceCountingPointer.hpp>
#include "UnitTest.hpp"

static int _dataIndex = 0;

class PtrData final
{
    DELETE_CM(PtrData);
private:
    int _x, _y, _index;
public:
    PtrData() noexcept
        : _x(0), _y(0), _index(_dataIndex++)
    { printf("Default Ptr[%d] Created\n", _index); }
    PtrData(int x, int y) noexcept
        : _x(x), _y(y), _index(_dataIndex++)
    { printf("Ptr[%d] Created: (%d, %d)\n", _index, x, y); }

    ~PtrData() noexcept
    { printf("Ptr[%d] Destroyed: (%d, %d)\n", _index, _x, _y); }

    [[nodiscard]] inline int x() const noexcept { return _x; }
    [[nodiscard]] inline int y() const noexcept { return _y; }
    [[nodiscard]] inline int index() const noexcept { return _index; }
    [[nodiscard]] inline int& x() noexcept { return _x; }
    [[nodiscard]] inline int& y() noexcept { return _y; }
};

class Child;

class Parent final
{
    DELETE_CM(Parent);
private:
    StrongReferenceCountingPointer<Child> _child;
    int _x;
public:
    Parent(const int x) noexcept
        : _child(nullptr)
        , _x(x)
    { printf("Created Parent: %d\n", x); }

    ~Parent() noexcept
    { printf("Destroying Parent: %d\n", _x); }

    void initChild(const StrongReferenceCountingPointer<Parent>& thisParent, const int x) noexcept
    { _child.reset(thisParent, x); }

    void destroyChild()
    { _child = nullptr; }
};

class Child final
{
    DELETE_CM(Child);
private:
    WeakReferenceCountingPointer<Parent> _parent;
    int _x;
public:
    Child(const StrongReferenceCountingPointer<Parent>& parent, const int x) noexcept
        : _parent(parent)
        , _x(x)
    { printf("Created child: %d\n", x); }

    ~Child() noexcept
    { printf("Destroying Child: %d\n", _x); }
};

namespace RefPtrTest {

template<typename _PtrType>
void _refCountTest() noexcept
{
    const _PtrType p0(DefaultTauAllocator::Instance(), 1, 3);
    Assert(p0.refCount() == 1);
    {
        const _PtrType p1 = p0;
        Assert(p1.refCount() == 2);
        const _PtrType p2 = p0;
        Assert(p2.refCount() == 3);
        const _PtrType p3 = p0;

        printf("Data: [%d, %d, %d]\n", p1->index(), p2->index(), p3->index());

        Assert(p1->index() == p2->index() && p3->index() == p1->index());
        Assert(p0.refCount() == p3.refCount());
        Assert(p0.refCount() == 4);
    }
    Assert(p0.refCount() == 1);
}

template<typename _PtrType>
void _refRewriteTest() noexcept
{
    _PtrType p0(DefaultTauAllocator::Instance(), 2, 4);
    Assert(p0.refCount() == 1);
    {
        const _PtrType p1 = p0;
        Assert(p1.refCount() == 2);
        const _PtrType p2(DefaultTauAllocator::Instance(), 7, 7);
        Assert(p2.refCount() == 1);
        const _PtrType p3 = p0;
        Assert(p0.refCount() == 3);

        printf("Data: [%d, %d, %d]\n", p1->index(), p2->index(), p3->index());

        Assert(p1->index() == p3->index());
        Assert(p0->index() == p1->index());
        Assert(p2->index() != p0->index());
        p0 = p2;
        Assert(p1->index() == p3->index());
        Assert(p2->index() == p0->index());
    }
}

template<typename _PtrType>
void _refTmpTest() noexcept
{
    _PtrType p0(DefaultTauAllocator::Instance(), 2, 4);
    Assert(p0.refCount() == 1);
    {
        const _PtrType p1 = p0;
        Assert(p1.refCount() == 2);
        const _PtrType p2 = p0;
        Assert(p0.refCount() == 3);

        printf("Data: [%d, %d]\n", p1->index(), p2->index());

        Assert(p1->index() == p2->index());
        Assert(p0->index() == p1->index());
        p0 = _PtrType(DefaultTauAllocator::Instance(), 7, 7);
        Assert(p1->index() == p2->index());
        Assert(p0->index() != p1->index());
        Assert(p0.refCount() == 1);
        Assert(p1.refCount() == 2);
        Assert(p1.refCount() == p2.refCount());
    }
}

template<typename _PtrType>
void _nullableRefNullSetTest() noexcept
{
    _PtrType p0(DefaultTauAllocator::Instance(), 2, 4);
    Assert(p0.refCount() == 1);
    {
        const _PtrType p1 = p0;
        Assert(p1.refCount() == 2);
        const _PtrType p2 = p0;
        Assert(p0.refCount() == 3);

        printf("Data: [%d, %d]\n", p1->index(), p2->index());

        Assert(p1->index() == p2->index());
        Assert(p0->index() == p1->index());
        p0 = nullptr;
        Assert(p1->index() == p2->index());
        Assert(!p0.get());
        Assert(p1.refCount() == 2);
        Assert(p1.refCount() == p2.refCount());
    }
}

template<typename _PtrType>
void _resetTest() noexcept
{
    _PtrType p0(DefaultTauAllocator::Instance(), 72, 64);
    p0.reset(DefaultTauAllocator::Instance(), 16, 13);
    Assert(p0->x() == 16 && p0->y() == 13);
}

void refCountTest() noexcept
{
    UNIT_TEST();
    _refCountTest<ReferenceCountingPointer<PtrData>>();
}

void refRewriteTest() noexcept
{
    UNIT_TEST();
    _refRewriteTest<ReferenceCountingPointer<PtrData>>();
}

void refTmpTest() noexcept
{
    UNIT_TEST();
    _refTmpTest<ReferenceCountingPointer<PtrData>>();
}

void nullableRefCountTest() noexcept
{
    UNIT_TEST();
    _refCountTest<ReferenceCountingPointer<PtrData>>();
}

void nullableRefRewriteTest() noexcept
{
    UNIT_TEST();
    _refRewriteTest<ReferenceCountingPointer<PtrData>>();
}

void nullableRefTmpTest() noexcept
{
    UNIT_TEST();
    _refTmpTest<ReferenceCountingPointer<PtrData>>();
}

void nullableRefNullSetTest() noexcept
{
    UNIT_TEST();
    _nullableRefNullSetTest<ReferenceCountingPointer<PtrData>>();
}

void strongRefCountTest() noexcept
{
    UNIT_TEST();
    _refCountTest<StrongReferenceCountingPointer<PtrData>>();
}

void strongRefRewriteTest() noexcept
{
    UNIT_TEST();
    _refRewriteTest<StrongReferenceCountingPointer<PtrData>>();
}

void strongRefTmpTest() noexcept
{
    UNIT_TEST();
    _refTmpTest<StrongReferenceCountingPointer<PtrData>>();
}

void nullableStrongRefCountTest() noexcept
{
    UNIT_TEST();
    _refCountTest<StrongReferenceCountingPointer<PtrData>>();
}

void nullableStrongRefRewriteTest() noexcept
{
    UNIT_TEST();
    _refRewriteTest<StrongReferenceCountingPointer<PtrData>>();
}

void nullableStrongRefTmpTest() noexcept
{
    UNIT_TEST();
    _refTmpTest<StrongReferenceCountingPointer<PtrData>>();
}

void nullableStrongRefNullSetTest() noexcept
{
    UNIT_TEST();
    _nullableRefNullSetTest<StrongReferenceCountingPointer<PtrData>>();
}

void refResetTest() noexcept
{
    UNIT_TEST();
    _resetTest<ReferenceCountingPointer<PtrData>>();
}

void nullableRefResetTest() noexcept
{
    UNIT_TEST();
    _resetTest<ReferenceCountingPointer<PtrData>>();
}

void strongRefResetTest() noexcept
{
    UNIT_TEST();
    _resetTest<StrongReferenceCountingPointer<PtrData>>();
}

void nullableStrongRefResetTest() noexcept
{
    UNIT_TEST();
    _resetTest<StrongReferenceCountingPointer<PtrData>>();
}

void strongParentChildTest() noexcept
{
    UNIT_TEST();

    StrongReferenceCountingPointer<Parent> parent(13);
    parent->initChild(parent, 72);
}

void strongParentChildReverseTest() noexcept
{
    UNIT_TEST();

    StrongReferenceCountingPointer<Parent> parent(13);
    parent->initChild(parent, 72);
    parent->destroyChild();
}

}
