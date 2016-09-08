#include <iostream>
#include <lua.hpp>
#include <luaportal/luaportal.h>
#include <luaportal/refcountedobject.h>
#include <luaportal/refcountedptr.h>
using namespace luaportal;

class A {
public:
    virtual void print(lua_State* L) {
        luaL_dostring(L, "print ('called A::print')");
    }

    std::string name;
};

class B :  public A {
public:
    virtual void print(lua_State* L) {
        luaL_dostring(L, "print ('called B::print')");
    }

    static B* GetInstance()
    {
        static B b;
        return &b;
    }

    int GetID() const 
    {
        return id_;
    }

    void SetID(int id)
    {
        id_ = id;
    }

private:
    int id_;
};

struct C : RefCountedObject{

};

void TestNamespace(LuaState& ls)
{
    ls.GlobalContext()
        .BeginNamespace("test")
        .BeginClass <A>("A")
        .Def(Constructor<>())
        .AddData("name", &A::name)
        .AddFunction("print", &A::print)
        .EndClass()
        .DeriveClass <B, A>("B")
        .Def(Constructor<>())
        .AddProperty("id", &B::GetID, &B::SetID)
        .AddStaticFunction("GetInstance", &B::GetInstance)
        .EndClass()
        .EndNamespace();

    auto ErrorHandle = [](const std::string& error)
    {
        std::cout << error << std::endl;
    };

    ls.DoFile("main.lua", ErrorHandle);
    ls.DoString("local b = test.B.GetInstance() b:print()");
}

void TestStack(LuaState& ls)
{
    auto L = ls.GetState();
    int const idx = lua_gettop(L);

    int i = 0;
    long l = 0;
    bool b = false;
    short s = 0;
    float f = 0;
    double d = 0;
    unsigned int ui = 0;
    unsigned char uc = 0;
    unsigned short us = 0;
    unsigned long ul = 0;
    char c = 'a';
    char const* cc = "";
    std::string ss = "";

    Push(L, i);  lua_pop(L, 1);
    Push(L, l);  lua_pop(L, 1);
    Push(L, b);  lua_pop(L, 1);
    Push(L, s);  lua_pop(L, 1);
    Push(L, f);  lua_pop(L, 1);
    Push(L, d);  lua_pop(L, 1);
    Push(L, ui); lua_pop(L, 1);
    Push(L, uc); lua_pop(L, 1);
    Push(L, us); lua_pop(L, 1);
    Push(L, ul); lua_pop(L, 1);
    Push(L, c);  lua_pop(L, 1);
    Push(L, cc); lua_pop(L, 1);
    Push(L, ss); lua_pop(L, 1);

    A a;
    A const ac(a);

    Push(L, &a);
    Push(L, &ac);

    Push <A&>(L, a);
    Push <A const&>(L, a);

    RefCountedObjectPtr <C> pc;
    RefCountedObjectPtr <C const> pcc;

    Push(L, pc);   lua_pop(L, 1);
    Push(L, pcc);  lua_pop(L, 1);
    Push(L, a);    lua_pop(L, 1);
    Push(L, ac);   lua_pop(L, 1);

    Stack <A*>::Push(L, &a); lua_pop(L, 1);
    Stack <A* const>::Push(L, &a); lua_pop(L, 1);
    Stack <A const*>::Push(L, &a); lua_pop(L, 1);
    Stack <A const* const>::Push(L, &a); lua_pop(L, 1);

    lua_settop(L, idx);
}

int main(int argc, char* argv[])
{
    LuaState ls;
    
    TestNamespace(ls);
    TestStack(ls);

    return 0;
}