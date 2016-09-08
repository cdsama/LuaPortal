#include <iostream>
#include <lua.hpp>
#include <luaportal/luaportal.h>
#include <luaportal/refcountedobject.h>
#include <luaportal/refcountedptr.h>
using namespace luaportal;

class Foo
{    
public:
    Foo()
    {
    }

    virtual ~Foo()
    {
    }

    static Foo* GetFoo()
    {
        static Foo f;
        return &f;
    }

    void Test()
    {
        std::cout << name << " " << ID << std::endl;
    }

    std::string name;
    int GetID() const
    {
        return ID;
    }

    void SetID(int id)
    {
        ID = id;
    }

private:
    int ID;
};

class Bar : public Foo
{
public:
    Bar() {
        name = "bar";
        SetID(888);
    }
    ~Bar() {}
    static Bar* GetBar()
    {
        static Bar bar;
        return &bar;
    }
};

struct A : RefCountedObject {
    virtual void print(lua_State* L) {
        luaL_dostring(L, "print ('called A::print')");
    }
};

struct B : A {
    virtual void print(lua_State* L) {
        luaL_dostring(L, "print ('called B::print')");
    }
};

void TestNamespace(LuaState& ls)
{
    ls.GlobalContext()
        .BeginNamespace("test")
        .BeginClass<Foo>("Foo")
        .AddData("name", &Foo::name)
        .AddProperty("ID", &Foo::GetID, &Foo::SetID)
        .AddProperty("ConstID", &Foo::GetID)
        .AddStaticFunction("GetFoo", &Foo::GetFoo)
        .AddFunction("Test", &Foo::Test)
        .EndClass()
        .DeriveClass<Bar, Foo>("Bar")
        .AddStaticFunction("GetBar", &Bar::GetBar)
        .EndClass()
        .BeginClass <A>("A")
        .Def<RefCountedObjectPtr<A>>(Constructor<>())
        .AddFunction("print", &A::print)
        .EndClass()
        .DeriveClass <B, A>("B")
        .Def<RefCountedObjectPtr<B>>(Constructor<>())
        .EndClass()
        .EndNamespace();

    auto ErrorHandle = [](const std::string& error)
    {
        std::cout << error << std::endl;
    };



    ls.DoString("f = test.Foo.GetFoo() f.name = 'xiaoming' f:Test()", ErrorHandle);
    ls.DoFile("main.lua", ErrorHandle);

    auto add = ls.GetGlobal("add");
    std::cout << add(1, 2) << std::endl;
    auto vbar = ls.GetGlobal("vbar");

    std::cout << vbar["name"] << vbar["ID"] << std::endl;

    ls.DoString("vbar:Test()", ErrorHandle);

    std::cout << "End" << std::endl;
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

    RefCountedObjectPtr <A> pa;
    RefCountedObjectPtr <A const> pac;

    Push(L, pa);   lua_pop(L, 1);
    Push(L, pac);  lua_pop(L, 1);
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