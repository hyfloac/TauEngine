#include "RTTI.hpp"

#include <cstdio>

class Entity
{
public:
    RTTI_TYPES(Player, Enemy);
    RTTI_BASE(Entity);
public:
    virtual const char* getName() = 0;
};

class Player : public Entity
{
    RTTI_SUB(Entity, Player);
public:
    int subData = 42;

    const char* getName() override { return "Player"; }
};

class Enemy : public Entity
{
    RTTI_SUB(Entity, Enemy);
    RTTI_TYPES(Mutant, Warrior);
    RTTI_BASE(Enemy);
public:
    const char* getName() override { return "Enemy"; }
};

class Mutant : public Enemy
{
    RTTI_SUB(Enemy, Mutant);
public:
    const char* getName() override { return "Mutant"; }
};

class Warrior : public Enemy
{
    RTTI_SUB(Enemy, Warrior);
public:
    const char* getName() override { return "Warrior"; }
};

const char* bs(bool b)
{
    return b ? "True" : "False";
}

#define IS_A_TYPE_TEST(__BASE, __CAST, __VAR) printf(#__BASE " is a " #__CAST ": %s\n", bs(IS_TYPE(__BASE, __CAST, __VAR)))
#define IS_AN_TYPE_TEST(__BASE, __CAST, __VAR) printf(#__BASE " is an " #__CAST ": %s\n", bs(IS_TYPE(__BASE, __CAST, __VAR)))
#define IS_NOT_A_TYPE_TEST(__BASE, __CAST, __VAR) printf(#__BASE " is not a " #__CAST ": %s\n", bs(!IS_TYPE(__BASE, __CAST, __VAR)))
#define IS_NOT_AN_TYPE_TEST(__BASE, __CAST, __VAR) printf(#__BASE " is not an " #__CAST ": %s\n", bs(!IS_TYPE(__BASE, __CAST, __VAR)))

#define CAST_TO_TEST(__BASE, __CAST, __VAR) printf(#__BASE " cast to " #__CAST ": %p\n", __VAR)

void rttiTest1()
{
    Entity* ent = new Player;

    IS_A_TYPE_TEST(Entity, Player, ent);
    IS_NOT_AN_TYPE_TEST(Entity, Enemy, ent);

    Player* pl = DYN_CAST(Entity, Player, ent);
    Enemy* en = DYN_CAST(Entity, Enemy, ent);

    CAST_TO_TEST(Entity, Player, pl);
    CAST_TO_TEST(Entity, Enemy, en);

    printf("Player Sub Data: %d", pl->subData);
}

void rttiTest2()
{
    Entity* ent = new Warrior;

    IS_AN_TYPE_TEST(Entity, Enemy, ent);
    IS_NOT_A_TYPE_TEST(Entity, Player, ent);

    Player* pl = DYN_CAST(Entity, Player, ent);
    Enemy* en = DYN_CAST(Entity, Enemy, ent);

    CAST_TO_TEST(Entity, Player, pl);
    CAST_TO_TEST(Entity, Enemy, en);

    if(en)
    {
        IS_NOT_A_TYPE_TEST(Enemy, Mutant, en);
        IS_A_TYPE_TEST(Enemy, Warrior, en);

        Mutant* mu = DYN_CAST(Enemy, Mutant, en);
        Warrior* wa = DYN_CAST(Enemy, Warrior, en);

        CAST_TO_TEST(Enemy, Mutant, mu);
        CAST_TO_TEST(Enemy, Warrior, wa);
    }
    else
    {
        puts("Failed to cast to entity, __fastfail()");
    }
}
