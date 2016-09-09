#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32

class RedLog
{
public:

    RedLog(std::ostream& os)
    :os_(os)
    {
        #ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), 12);
        #else
		os_ << "\033[31m";
        #endif
    }
    ~RedLog()
	{
        #ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), 7);
        #else
		os_ << "\033[0m";
        #endif
	};
private:
    std::ostream& os_;
};

#define REDLOG(__CONTENT__) {RedLog rl(std::cerr); std::cerr << __CONTENT__ <<std::endl;}

class AutoClearStack
{
private:
    int top_;
    lua_State* L_;
public:
    AutoClearStack(lua_State* L)
    {
        L_ = L;
        top_ = lua_gettop(L);
    }

    ~AutoClearStack() 
    {
        lua_settop(L_, top_);
    }
};