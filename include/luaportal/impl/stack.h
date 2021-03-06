//------------------------------------------------------------------------------
/**
 Receive the lua_State* as an argument.
 */
template<>
struct Stack<lua_State*>
{
    static lua_State* Get(lua_State* L, int)
    {
        return L;
    }

    static inline bool CheckType(lua_State* L, int)
    {
        return true;
    }

    static inline lua_State* DefaultValue(lua_State* L, int)
    {
        return L;
    }

    static inline const char * RequireType()
    {
        return "lua_State*";
    }
};

//------------------------------------------------------------------------------
/**
 Push a lua_CFunction.
 */
template<>
struct Stack<lua_CFunction>
{
    static void Push(lua_State* L, lua_CFunction f)
    {
        lua_pushcfunction(L, f);
    }
    
    static lua_CFunction Get(lua_State* L, int index)
    {
        return lua_tocfunction(L, index);
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_iscfunction(L, index) ? true : false;
    }

    static inline lua_CFunction DefaultValue(lua_State* L, int)
    {
        return nullptr;
    }

    static inline const char * RequireType()
    {
        return "lua_CFunction";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `int`.
 */
template<>
struct Stack<int>
{
    static inline void Push(lua_State* L, int value)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
    }
    
    static inline int Get(lua_State* L, int index)
    {
        return static_cast<int>(luaL_checkinteger(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isinteger(L, index) ? true : false;
    }

    static inline int DefaultValue(lua_State* L, int)
    {
        return 0;
    }

    static inline const char * RequireType()
    {
        return "int";
    }
};

template<>
struct Stack<int const&>
{
    static inline void Push(lua_State* L, int value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline int Get(lua_State* L, int index)
    {
        return static_cast<int >(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline int DefaultValue(lua_State* L, int)
    {
        return 0;
    }

    static inline const char * RequireType()
    {
        return "int const &";
    }
};
//------------------------------------------------------------------------------
/**
 Stack specialization for `unsigned int`.
 */
template<>
struct Stack<unsigned int>
{
    static inline void Push(lua_State* L, unsigned int value)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
    }
    
    static inline unsigned int Get(lua_State* L, int index)
    {
        return static_cast<unsigned int>(luaL_checkinteger(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isinteger(L, index) ? true : false;
    }

    static inline unsigned int DefaultValue(lua_State* L, int)
    {
        return 0;
    }

    static inline const char * RequireType()
    {
        return "unsigned int";
    }
};

template<>
struct Stack<unsigned int const&>
{
    static inline void Push(lua_State* L, unsigned int value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline unsigned int Get(lua_State* L, int index)
    {
        return static_cast<unsigned int >(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline unsigned int DefaultValue(lua_State* L, int)
    {
        return 0;
    }

    static inline const char * RequireType()
    {
        return "unsigned int const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `unsigned char`.
 */
template<>
struct Stack<unsigned char>
{
    static inline void Push(lua_State* L, unsigned char value)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
    }
    
    static inline unsigned char Get(lua_State* L, int index)
    {
        return static_cast<unsigned char>(luaL_checkinteger(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isinteger(L, index) ? true : false;
    }

    static inline unsigned char DefaultValue(lua_State* L, int)
    {
        return static_cast<unsigned char>(0);
    }

    static inline const char * RequireType()
    {
        return "unsigned char";
    }
};

template<>
struct Stack<unsigned char const&>
{
    static inline void Push(lua_State* L, unsigned char value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline unsigned char Get(lua_State* L, int index)
    {
        return static_cast<unsigned char>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline unsigned char DefaultValue(lua_State* L, int)
    {
        return static_cast<unsigned char>(0);
    }

    static inline const char * RequireType()
    {
        return "unsigned char const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `short`.
 */
template<>
struct Stack<short>
{
    static inline void Push(lua_State* L, short value)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
    }
    
    static inline short Get(lua_State* L, int index)
    {
        return static_cast<short>(luaL_checkinteger(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isinteger(L, index) ? true : false;
    }

    static inline short DefaultValue(lua_State* L, int)
    {
        return static_cast<short>(0);
    }

    static inline const char * RequireType()
    {
        return "short";
    }
};

template<>
struct Stack<short const&>
{
    static inline void Push(lua_State* L, short value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline short Get(lua_State* L, int index)
    {
        return static_cast<short>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline short DefaultValue(lua_State* L, int)
    {
        return static_cast<short>(0);
    }

    static inline const char * RequireType()
    {
        return "shor const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `unsigned short`.
 */
template<>
struct Stack<unsigned short>
{
    static inline void Push(lua_State* L, unsigned short value)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
    }
    
    static inline unsigned short Get(lua_State* L, int index)
    {
        return static_cast<unsigned short>(luaL_checkinteger(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isinteger(L, index) ? true : false;
    }

    static inline unsigned short DefaultValue(lua_State* L, int)
    {
        return static_cast<unsigned short>(0);
    }

    static inline const char * RequireType()
    {
        return "unsigned short";
    }
};

template<>
struct Stack<unsigned short const&>
{
    static inline void Push(lua_State* L, unsigned short value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline unsigned short Get(lua_State* L, int index)
    {
        return static_cast<unsigned short>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline unsigned short DefaultValue(lua_State* L, int)
    {
        return static_cast<unsigned short>(0);
    }

    static inline const char * RequireType()
    {
        return "unsigned short const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `long`.
 */
template<>
struct Stack<long>
{
    static inline void Push(lua_State* L, long value)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
    }
    
    static inline long Get(lua_State* L, int index)
    {
        return static_cast<long>(luaL_checkinteger(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isinteger(L, index) ? true : false;
    }

    static inline long DefaultValue(lua_State* L, int)
    {
        return static_cast<long>(0);
    }

    static inline const char * RequireType()
    {
        return "long";
    }
};

template<>
struct Stack<long const&>
{
    static inline void Push(lua_State* L, long value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline long Get(lua_State* L, int index)
    {
        return static_cast<long>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline long DefaultValue(lua_State* L, int)
    {
        return static_cast<long>(0);
    }

    static inline const char * RequireType()
    {
        return "long const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `unsigned long`.
 */
template<>
struct Stack<unsigned long>
{
    static inline void Push(lua_State* L, unsigned long value)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
    }
    
    static inline unsigned long Get(lua_State* L, int index)
    {
        return static_cast<unsigned long>(luaL_checkinteger(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isinteger(L, index) ? true : false;
    }

    static inline unsigned long DefaultValue(lua_State* L, int)
    {
        return static_cast<unsigned long>(0);
    }

    static inline const char * RequireType()
    {
        return "unsigned long";
    }
};

template<>
struct Stack<unsigned long const&>
{
    static inline void Push(lua_State* L, unsigned long value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline unsigned long Get(lua_State* L, int index)
    {
        return static_cast<unsigned long>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline unsigned long DefaultValue(lua_State* L, int)
    {
        return static_cast<unsigned long>(0);
    }

    static inline const char * RequireType()
    {
        return "unsigned long const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `float`.
 */
template<>
struct Stack<float>
{
    static inline void Push(lua_State* L, float value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline float Get(lua_State* L, int index)
    {
        return static_cast<float>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline float DefaultValue(lua_State* L, int)
    {
        return static_cast<float>(0);
    }

    static inline const char * RequireType()
    {
        return "float";
    }
};

template<>
struct Stack<float const&>
{
    static inline void Push(lua_State* L, float value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline float Get(lua_State* L, int index)
    {
        return static_cast<float>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline float DefaultValue(lua_State* L, int)
    {
        return static_cast<float>(0);
    }

    static inline const char * RequireType()
    {
        return "float const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `double`.
 */
template<> struct Stack<double>
{
    static inline void Push(lua_State* L, double value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline double Get(lua_State* L, int index)
    {
        return static_cast<double>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline double DefaultValue(lua_State* L, int)
    {
        return static_cast<double>(0);
    }

    static inline const char * RequireType()
    {
        return "double";
    }
};

template<> 
struct Stack<double const&>
{
    static inline void Push(lua_State* L, double value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }
    
    static inline double Get(lua_State* L, int index)
    {
        return static_cast<double>(luaL_checknumber(L, index));
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isnumber(L, index) ? true : false;
    }

    static inline double DefaultValue(lua_State* L, int)
    {
        return static_cast<double>(0);
    }

    static inline const char * RequireType()
    {
        return "double const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `bool`.
 */
template<>
struct Stack<bool> {
    static inline void Push(lua_State* L, bool value)
    {
        lua_pushboolean(L, value ? 1 : 0);
    }
    
    static inline bool Get(lua_State* L, int index)
    {
        return lua_toboolean(L, index) ? true : false;
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isboolean(L, index) ? true : false;
    }

    static inline bool DefaultValue(lua_State* L, int)
    {
        return false;
    }

    static inline const char * RequireType()
    {
        return "bool";
    }

};

template<>
struct Stack<bool const&> {
    static inline void Push(lua_State* L, bool value)
    {
        lua_pushboolean(L, value ? 1 : 0);
    }
    
    static inline bool Get(lua_State* L, int index)
    {
        return lua_toboolean(L, index) ? true : false;
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_isboolean(L, index) ? true : false;
    }

    static inline bool DefaultValue(lua_State* L, int)
    {
        return false;
    }

    static inline const char * RequireType()
    {
        return "bool const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `char`.
 */
template<>
struct Stack<char>
{
    static inline void Push(lua_State* L, char value)
    {
        char str [2] = { value, 0 };
        lua_pushstring(L, str);
    }
    
    static inline char Get(lua_State* L, int index)
    {
        return luaL_checkstring(L, index) [0];
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TSTRING ? true : false;
    }

    static inline char DefaultValue(lua_State* L, int)
    {
        return '\0';
    }

    static inline const char * RequireType()
    {
        return "char";
    }
};

template<>
struct Stack<char const&>
{
    static inline void Push(lua_State* L, char value)
    {
        char str [2] = { value, 0 };
        lua_pushstring(L, str);
    }
    
    static inline char Get(lua_State* L, int index)
    {
        return luaL_checkstring(L, index) [0];
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TSTRING ? true : false;
    }

    static inline char DefaultValue(lua_State* L, int)
    {
        return '\0';
    }

    static inline const char * RequireType()
    {
        return "char const &";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `float`.
 */
template<>
struct Stack<char const*>
{
    static inline void Push(lua_State* L, char const* str)
    {
        if (str != 0)
        {
            lua_pushstring(L, str);
        }
        else
        {
            lua_pushnil(L);
        }
    }
    
    static inline char const* Get(lua_State* L, int index)
    {
        return lua_isnil(L, index) ? 0 : luaL_checkstring(L, index);
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TSTRING || lua_isnil(L, index) ? true : false;
    }

    static inline char const* DefaultValue(lua_State* L, int)
    {
        return nullptr;
    }

    static inline const char * RequireType()
    {
        return "char const *";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `std::string`.
 */
template<>
struct Stack<std::string>
{
    static inline void Push(lua_State* L, std::string const& str)
    {
        lua_pushlstring(L, str.c_str(), str.size());
    }
    
    static inline std::string Get(lua_State* L, int index)
    {
        size_t len;
        const char *str = luaL_checklstring(L, index, &len);
        return std::string(str, len);
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TSTRING;
    }

    static inline std::string DefaultValue(lua_State* L, int)
    {
        return "";
    }

    static inline const char * RequireType()
    {
        return "std::string";
    }
};

//------------------------------------------------------------------------------
/**
 Stack specialization for `std::string const&`.
 */
template<>
struct Stack<std::string const&>
{
    static inline void Push(lua_State* L, std::string const& str)
    {
        lua_pushlstring(L, str.c_str(), str.size());
    }
    
    static inline std::string Get(lua_State* L, int index)
    {
        size_t len;
        const char *str = luaL_checklstring(L, index, &len);
        return std::string(str, len);
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TSTRING;
    }

    static inline std::string DefaultValue(lua_State* L, int)
    {
        return "";
    }

    static inline const char * RequireType()
    {
        return "std::string const &";
    }
};

static inline int PushArgs(lua_State *L)
{
    return 0;
}
template<typename H,typename... P>
int PushArgs(lua_State *L, H h, P... p) {
    Stack<H>::Push(L, h);
    return 1 + PushArgs(L, p...);
}

class FunctionTransfer {
    lua_State *state;
    int ref;
public:
    FunctionTransfer(lua_State *L, int index)
    {
        state = L;
        ref = 0;
        if(state) {
            lua_pushvalue(state, index);
            ref = luaL_ref(state, LUA_REGISTRYINDEX);
        }
    }
    
    template<typename R , typename... P>
    static void create(lua_State *L, int index, std::function<R(P...)> &func)
    {
        auto auf = std::make_shared<FunctionTransfer>(L, index);
        func = [auf](P... p)->R{
            lua_State *L = auf->getState();
            lua_pushcfunction(L, &ShowDebugMessage);
            auto debugfunc = lua_gettop(L);
            lua_rawgeti(L, LUA_REGISTRYINDEX, auf->getRef());
            int nargs = PushArgs(L, p...);
            lua_pcall(L, nargs, 1, debugfunc);
            lua_remove(L, debugfunc);
            if (!Stack<R>::CheckType(L, -1))
            {
                REDLOG("return type error: expected '" << Stack<R>::RequireType() << "', but got '" << lua_typename(L, lua_type(L, -1)) << "'\n\t in function "<<typeid(R(P...)).name());
                R value = Stack<R>::DefaultValue(L, -1);
                lua_pop(L, 1);
                lua_getglobal(L, "debug");
                lua_getfield(L, -1, "traceback");
                lua_pcall(L, 0, 1, 0);
                REDLOG(Stack<std::string>::Get(L, -1));
                lua_pop(L, 2);
                return value;
            }
            R value = Stack<R>::Get(L, -1);
            lua_pop(L, 1);
            return value;
        };
    }
    
    template<typename R = void, typename... P>
    static void create(lua_State *L, int index, std::function<void(P...)> &func)
    {
        auto auf = std::make_shared<FunctionTransfer>(L, index);
        func = [auf](P... p)->void{
            lua_State *L = auf->getState();
            lua_pushcfunction(L, &ShowDebugMessage);
            auto debugfunc = lua_gettop(L);
            lua_rawgeti(L, LUA_REGISTRYINDEX, auf->getRef());
            int nargs = PushArgs(L, p...);
            lua_pcall(L, nargs, 0, debugfunc);
            lua_remove(L, debugfunc);
        };
    }
    
    template<typename R, typename... P>
    static void create(lua_State *L, int index, std::function<void(void)> &func)
    {
        auto auf = std::make_shared<FunctionTransfer>(L, index);
        func = [auf]()->void{
            lua_State *L = auf->getState();
            lua_pushcfunction(L, &ShowDebugMessage);
            auto debugfunc = lua_gettop(L);
            lua_rawgeti(L, LUA_REGISTRYINDEX, auf->getRef());
            lua_pcall(L, 0, 0, debugfunc);
            lua_remove(L, debugfunc);
        };
    }
    
    template<typename FT>
    static void Get(lua_State *L, int index, std::function<FT> &func)
    {
        if(lua_isfunction(L, index))
        {
            create(L, index, func);
        } 
        else if(lua_isuserdata(L, index))
        {
            func = *((std::function<FT>*)(luaL_checkudata(L, index, typeid(func).name())));
        } else if(lua_isnil(L, index)) {
            func = nullptr;
        } else {
            luaL_checktype(L, index, LUA_TFUNCTION);
        }
    }
    
    int getRef(){return ref;}
    lua_State* getState(){return state;}
    
    ~FunctionTransfer()
    {
        if(state) {            
            luaL_unref(state, LUA_REGISTRYINDEX, ref);
        }
    }
};


template<typename FT>
struct Stack<std::function<FT> >
{
    static inline void Push(lua_State* L, std::function<FT> func)
    {
        if(func) {
            new(lua_newuserdata(L, sizeof(func))) std::function<FT>(func);
            luaL_newmetatable(L, typeid(func).name());
            lua_pushvalue(L, -2);
            lua_pushcclosure(L, &StaticLambda<std::function<FT>>::Call, 1);
            rawsetfield(L, -2, "__call");
            lua_setmetatable(L, -2);            
        } else {
            lua_pushnil(L);
        }
    }
    
    static inline std::function<FT> Get(lua_State* L, int index)
    {
        std::function<FT> func;
        FunctionTransfer::Get(L, index, func);
        return func;
    }

    static inline bool CheckType(lua_State* L, int index)
    {
        if (lua_isfunction(L, index))
        {
            return true;
        }
        else if (lua_isuserdata(L, index))
        {
            if (lua_getmetatable(L, index))
            {
                luaL_getmetatable(L, typeid(std::function<FT>).name());
                bool ret = lua_rawequal(L, -1, -2) ? true : false;
                lua_pop(L, 2);
                return ret;
            }
        }
        return false;
    }

    static inline std::function<FT> DefaultValue(lua_State* L, int)
    {
        return nullptr;
    }

    static inline const char * RequireType()
    {
        return typeid(std::function<FT>).name();
    }
};
