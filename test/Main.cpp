#include <iostream>
#include <lua.hpp>
#include <luaportal/luaportal.h>
#include <luaportal/refcountedobject.h>
#include <luaportal/refcountedptr.h>
using namespace luaportal;

class A {
public:
    virtual void Print(lua_State* L) {
        luaL_dostring(L, "print ('called A::Print')");
    }

    std::string name;
};

class B :  public A {
public:
    virtual void Print(lua_State* L) {
        luaL_dostring(L, "print ('called B::Print')");
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

    void RiseID()
    {
        id_++;
    }

    std::function<std::string(int, int)> TestSTDFunction;

private:
    int id_;
};

enum class TestEnum {
    Value1,
    Value2
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
        .AddFunction("print", &A::Print)
        .EndClass()
        .DeriveClass <B, A>("B")
        .Def(Constructor<>())
        .AddData("TestSTDFunction", &B::TestSTDFunction)
        .AddProperty("id", &B::GetID, &B::SetID)
        .AddProperty("readonlyid", &B::GetID)
        .AddFunction("RiseID",&B::RiseID)
        .AddStaticFunction("GetInstance", &B::GetInstance)
        .AddLambda("lambdatest1", [](B* B) {if (B != nullptr) { B->name = "newname"; }})
        .AddStaticLambda("lambdatest2", []()->std::string { return "B.lambdatest2()"; })
        .EndClass()
        .AddLambda("lambdatest3", []()->B* { return B::GetInstance(); })
        .EndNamespace()
        .BeginEnum<TestEnum>("TestEnum")
        .AddEnumValue("Value1",TestEnum::Value1)
        .AddEnumValue("Value2", TestEnum::Value2)
        .EndEnum()
        ;

    auto ErrorHandle = [](const std::string& error)
    {
        std::cout << error << std::endl;
    };

    ls.DoFile("main.lua", ErrorHandle);

    ls.DoString("local a = test.A() a:Print()");
    ls.DoString("b = test.B.GetInstance() b:Print()");
    ls.DoString("b.name = 'name' b.id = 12345 print('b.name = ',b.name,'b.id = ',b.id)");
    ls.DoString("b:lambdatest1() print('b.name = ',b.name)");
    assert(ls.GetGlobal("b").Cast<B*>() == B::GetInstance());
    ls.DoString("b:RiseID() print('b.readonlyid = ',b.readonlyid) b.readonlyid = 456789", ErrorHandle);
    assert(ls.GetGlobal("test")["B"]["lambdatest2"]().Cast<std::string>() == "B.lambdatest2()");
    assert(ls.GetGlobal("test")["lambdatest3"]().Cast<B*>() == B::GetInstance());

    ls.DoString("b.TestSTDFunction = function(a, b) return tostring(a) .. '&' .. tostring(b) end", ErrorHandle);
    assert(B::GetInstance()->TestSTDFunction(7, 8) == "7&8");

    ls.DoString("tb = { a = 1, b = 2, c = { d = 3, e = 'f', g = { h = 4, i = { 5 } } } }");
    auto tb = ls.GetGlobal("tb");
    assert(tb["a"].Cast<int>() == 1);
    assert(tb["b"].Cast<int>() == 2);
    assert(tb["c"]["d"].Cast<int>() == 3);
    assert(tb["c"]["e"].Cast<std::string>() == "f");

    std::function<void(LuaRef&, int)> TestIterator = [&](LuaRef& tableRef, int level)
    {
        for (auto itr = Iterator(tableRef); !itr.IsNil(); ++itr)
        {
            auto& TableKey = itr.Key();
            auto& TableValue = itr.Value();

            for (int i = 0; i < level; ++i)
            {
                std::cout << "\t";
            }

            std::cout << TableKey.ToString() << " " << TableValue.ToString() << std::endl;
            if (TableValue.IsTable())
            {
                TestIterator(TableValue, level + 1);
            }
        }
    };

    TestIterator(tb, 0);

    ls.DoString("function func() return test.B.GetInstance() end ");
    ls.DoString("function func2(a, b) return a*b end ");
    auto func = ls.GetGlobal("func");
    assert(func().Cast<B*>() == B::GetInstance());
    auto func2 = ls.GetGlobal("func2");
    assert(func2(3, 4).Cast<int>() == 12);
    
    assert(ls.GetGlobal("TestEnum")["Value1"].Cast<TestEnum>() == TestEnum::Value1);
    assert(ls.GetGlobal("TestEnum")["Value2"].Cast<TestEnum>() == TestEnum::Value2);

    // Should clear registed std::function before luastate closed. 
    B::GetInstance()->TestSTDFunction = nullptr;
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