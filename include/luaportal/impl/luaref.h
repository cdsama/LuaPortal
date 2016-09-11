/**/


struct Nil {};

/*
 * Always use LuaRef as a local variable.
 * NEVER use LuaRef as a member/global/static variable.
 */
class LuaRef {
    
    class Proxy;
    friend class Proxy;
    friend class Iterator;
    friend struct Stack<LuaRef> ;
    friend struct Stack<Proxy> ;
    friend std::ostream& operator<<(std::ostream&, LuaRef::Proxy const&);
    
private:
    
    /*
     * A proxy for assigning table element(operator [] of LuaRef).
     * NEVER use Proxy alone.
     */
    class Proxy {
        
        friend class LuaRef;
        friend struct Stack< Proxy > ;
        
    private:
        /*
         * The key is on the top of stack,
         * and the table is just below the top.
         * Stack:
         *   [TOP] key
         *   [ 2 ] table
         * The key and the table is popped off the stack
         */
        Proxy(lua_State* L)
        : L(L) {
            m_keyRef = luaL_ref(L, LUA_REGISTRYINDEX);
            m_tableRef = luaL_ref(L, LUA_REGISTRYINDEX);
        }
        
        
        /*
         * This function may trigger a metamethod for the "index" event.
         */
        int CreateRef() const {
            Push();
            return luaL_ref(L, LUA_REGISTRYINDEX);
        }
        
        /*
         * Push the value onto the Lua stack.
         * May invoke metamethods.
         */
        void Push() const {
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_tableRef);  
            if (lua_istable(L, -1) || lua_isuserdata(L, -1))
            {
                lua_rawgeti(L, LUA_REGISTRYINDEX, m_keyRef);
                lua_gettable(L, -2); // This may trigger the "index" event.
            }
            else 
            {
                lua_pushnil(L);
            }
            lua_remove(L, -2);
            
        }
        
    private:
        lua_State* L;
        int m_tableRef;
        int m_keyRef;
        
    public:
        ~Proxy() {
            luaL_unref(L, LUA_REGISTRYINDEX, m_tableRef);
            luaL_unref(L, LUA_REGISTRYINDEX, m_keyRef);
        }
        
        /*
         * Assign a value to this table key.
         * May invoke metamethods.
         */
        template<typename T>
        Proxy& operator=(T v) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_tableRef);
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_keyRef);
            Stack<T>::Push(L, v);
            lua_settable(L, -3); // This may trgger the "newindex" event
            lua_pop(L, 1);
            return *this;
        }
        
        Proxy& operator=(Proxy const& other) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_tableRef);
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_keyRef);
            other.Push();
            lua_settable(L, -3); // This may trgger the "newindex" event
            lua_pop(L, 1);
            return *this;
        }
        
        /*
         * Assign a value to this table key.
         * Will not invoke metamethods.
         */
        template<typename T>
        Proxy& RawSet(T v) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_tableRef);
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_keyRef);
            Stack<T>::Push(L, v);
            lua_rawset(L, -3); // This may trgger the "newindex" event
            lua_pop(L, 1);
            return *this;
        }
        
        Proxy& RawSet(Proxy const& other) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_tableRef);
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_keyRef);
            other.Push();
            lua_rawset(L, -3); // This may trgger the "newindex" event
            lua_pop(L, 1);
            return *this;
        }
        
        lua_State* GetState() const { return L; }
        
        int GetType() const {
            Push();
            int type = lua_type(L, -1);
            lua_pop(L, 1);
            return type;
        }
        
        const char * TypeName(){
            return lua_typename(L, GetType());
        }
        
        bool IsNil() const { return GetType() == LUA_TNIL; }
        bool IsNumber() const { return GetType() == LUA_TNUMBER; }
        bool IsString() const { return GetType() == LUA_TSTRING; }
        bool IsTable() const { return GetType() == LUA_TTABLE; }
        bool IsFunction() const { return GetType() == LUA_TFUNCTION; }
        bool IsUserdata() const { return GetType() == LUA_TUSERDATA; }
        bool IsThread() const { return GetType() == LUA_TTHREAD; }
        bool IsLightUserdata() const { return GetType() == LUA_TLIGHTUSERDATA; }
        
        /*
         * Explicit conversion.
         * May invoke metamethods.
         */
        template<typename T>
        T Cast() const {
            Push();
            T t = Stack<T>::Get(L, lua_gettop(L));
            lua_pop(L, 1);
            return t;
        }
        
        /*
         * Implicit conversion
         * May invoke metamethods.
         */
        template<typename T>
        operator T() const {
            return Cast<T>();
        }
        
        /*
         * Access a table value using a key.
         * May invoke metamethods.
         */
        template<typename T>
        Proxy operator[](T key) const {
            Push();
            Stack<T>::Push(L, key);
            return Proxy(L);
        }
        
        LuaRef operator()() const {
            lua_pushcfunction(L, &ShowDebugMessage);
            auto debugfunc = lua_gettop(L);
            Push();
            lua_pcall(L, 0, 1, debugfunc);
            lua_remove(L, debugfunc);
            return PopLuaRef(L);
        }
        
        template<typename... Args>
        LuaRef operator()(Args... args) const {
            lua_pushcfunction(L, &ShowDebugMessage);
            auto debugfunc = lua_gettop(L);
            Push();
            int nargs = PushArgs(L, args...);
            lua_pcall(L, nargs, 1, 0);
            lua_remove(L, debugfunc);
            return PopLuaRef(L);
        }
        
        int Length() const {
            Push(); // push 1
            lua_len(L, -1); // push 1
            int len =(int)luaL_checkinteger(L, -1);
            lua_pop(L, 2); // pop 2
            return len;
        }
        
        std::string ToString() const {
            lua_getglobal(L, "tostring");
            Push();
            lua_call(L, 1, 1);
            std::string str = lua_tostring(L, 1);
            lua_pop(L, 1);
            return str;
        }
        
    private:
        /*
         * Copy is NOT allowed.
         */
        Proxy(const Proxy& other);
        
    };
    
private:
    lua_State* L;
    int m_ref;
    
    template<typename T>
    static int PushArgs(lua_State* L, T t) {
        Stack<T>::Push(L, t);
        return 1;
    }
    
    template<typename Head, typename... Args>
    static int PushArgs(lua_State* L, Head h, Args... args) {
        Stack<Head>::Push(L, h);
        int nargs = 1 + PushArgs(L, args...);
        return nargs;
    }
    
    static LuaRef PopLuaRef(lua_State* L) {
        LuaRef v(L);
        v.Pop();
        return v;
    }
    
    int CreateRef() const {
        if(m_ref != LUA_REFNIL) {
            Push();
            return luaL_ref(L, LUA_REGISTRYINDEX);
        }
        else {
            return LUA_REFNIL;
        }
    }
    
    /*
     * Push the object onto the Lua stack
     */
    void Push() const {
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_ref);
    }
    
    /*
     * Pop the top of Lua stack and assign the ref to this->m_ref
     */
    void Pop() {
        luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }
    
public:
    explicit LuaRef(lua_State* L)
    : L(L),
    m_ref(LUA_REFNIL) {
    }
    
    template<typename T>
    LuaRef(lua_State* L, T v)
    : L(L) {
        Stack<T>::Push(L, v);
        m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }
    
    LuaRef(LuaRef const& other)
    : L(other.L),
    m_ref(other.CreateRef()) {
    }
    
    /*
     * Create a LuaRef from Proxy.
     * May invoke metamethods.
     */
    LuaRef(Proxy const& proxy)
    : L(proxy.GetState()),
    m_ref(proxy.CreateRef()) {
    }
    
    ~LuaRef() {
        luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
    }
    
    template<typename T>
    LuaRef& operator=(T rhs) {
        luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        Stack<T>::Push(L, rhs);
        m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
        return *this;
    }
    
    LuaRef& operator=(LuaRef const& rhs) {
        luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        L = rhs.GetState();
        rhs.Push();
        m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
        return *this;
    }
    
    /*
     * May invoke metamethod.
     */
    LuaRef& operator=(Proxy const& rhs) {
        luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        L = rhs.GetState();
        m_ref = rhs.CreateRef();
        return *this;
    }
    
    std::string ToString() const {
        lua_getglobal(L, "tostring");
        Push();
        lua_call(L, 1, 1);
        std::string str = lua_tostring(L, 1);
        lua_pop(L, 1);
        return str;
    }
    
    lua_State* GetState() const {
        return L;
    }
    
    /*
     * Return lua_type
     */
    int GetType() const {
        int type;
        if(m_ref != LUA_REFNIL) {
            Push();
            type = lua_type(L, -1);
            lua_pop(L, 1);
        }
        else {
            type = LUA_TNIL;
        }
        return type;
    }
    
    const char * TypeName() {
        return lua_typename(L, GetType());
    }
    
    bool IsNil() const { return GetType() == LUA_TNIL; }
    bool IsNumber() const { return GetType() == LUA_TNUMBER; }
    bool IsString() const { return GetType() == LUA_TSTRING; }
    bool IsTable() const { return GetType() == LUA_TTABLE; }
    bool IsFunction() const { return GetType() == LUA_TFUNCTION; }
    bool IsUserdata() const { return GetType() == LUA_TUSERDATA; }
    bool IsThread() const { return GetType() == LUA_TTHREAD; }
    bool IsLightUserdata() const { return GetType() == LUA_TLIGHTUSERDATA; }
    
    /*
     * Explicit conversion
     */
    template<typename T>
    T Cast() const {
        Push();
        T t = Stack<T>::Get(L, lua_gettop(L));
        lua_pop(L, 1);
        return t;
    }
    
    template<typename T>
    operator T() const {
        return Cast<T>();
    }
    
    int Length() const {
        Push(); // push 1
        lua_len(L, -1); // push 1
        int len =(int)luaL_checkinteger(L, -1);
        lua_pop(L, 2); // pop 2
        return len;
    }
    
    /*
     * [] for access to element table[key].
     * May invoke metamethods.
     * NOT USED
     template<typename T>
     LuaRef operator[](T key) const {
     Push();
     Stack<T>::Push(m_L, key);
     lua_gettable(m_L, -2);
     LuaRef v = LuaRef::PopLuaRef(m_L);
     lua_pop(m_L, 1);
     return v;
     }*/
    
    /*
     * Access to element table[key].
     */
    template<typename T>
    Proxy operator[](T key) const {
        Push();
        Stack<T>::Push(L, key);
        return Proxy(L);
    }
    
    LuaRef operator()() const {
        lua_pushcfunction(L, &ShowDebugMessage);
        auto debugfunc = lua_gettop(L);
        Push();
        lua_pcall(L, 0, 1, debugfunc);
        lua_remove(L, debugfunc);
        return PopLuaRef(L);
    }
    
    template<typename... Args>
    LuaRef operator()(Args... args) const {
        lua_pushcfunction(L, &ShowDebugMessage);
        auto debugfunc = lua_gettop(L);
        Push();
        int nargs = PushArgs(L, args...);
        lua_pcall(L, nargs, 1, debugfunc);
        lua_remove(L, debugfunc);
        return PopLuaRef(L);
    }
    
    static LuaRef GetGlobal(lua_State* L, char const* name) {
        lua_getglobal(L, name);
        return LuaRef::PopLuaRef(L);
    }
    
    static LuaRef getindex(lua_State* L, int index) {
        lua_pushvalue(L, index);
        return LuaRef::PopLuaRef(L);
    }
    
};


inline LuaRef GetGlobal(lua_State* L, char const* name) {
    return LuaRef::GetGlobal(L, name);
}

inline LuaRef getIndex(lua_State* L, int index) {
    return LuaRef::getindex(L, index);
}

template<> struct Stack< Nil > {
    static void Push(lua_State* L, Nil) {
        lua_pushnil(L);
    }
};


template<> struct Stack< LuaRef::Proxy > {
    static void Push(lua_State* L, LuaRef::Proxy const& v) {
        assert(equalstates(L, v.GetState()));
        v.Push();
    }
};


template<> struct Stack< LuaRef > {
    static void Push(lua_State* L, LuaRef const& v) {
        assert(equalstates(L, v.GetState()));
        v.Push();
    }
    static LuaRef Get(lua_State* L, int index) {
        lua_pushvalue(L, index);
        return LuaRef::PopLuaRef(L);
    }
};



inline void printLuaRef(std::ostream& os, LuaRef const& v) {
    int type = v.GetType();
    switch(type) {
        case LUA_TNIL:
            os<< "nil";
            break;
        case LUA_TNUMBER:
            os<< v.Cast<lua_Number>();
            break;
        case LUA_TBOOLEAN:
            os<<(v.Cast<bool>() ? "true" : "false");
            break;
        case LUA_TSTRING:
            os<< '"'<< v.Cast<std::string>()<< '"';
            break;
        case LUA_TTABLE:
        case LUA_TFUNCTION:
        case LUA_TUSERDATA:
        case LUA_TTHREAD:
        case LUA_TLIGHTUSERDATA:
            os<< v.ToString();
            break;
        default:
            os<< "unknown";
            break;
    }
}

inline std::ostream& operator<<(std::ostream& os, LuaRef const& v) {
    printLuaRef(os, v);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, LuaRef::Proxy const& v) {
    printLuaRef(os, LuaRef(v));
    return os;
}

