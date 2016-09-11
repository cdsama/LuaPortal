/**/

/* A wrapper for lua_State */
class LuaState {
private:
    lua_State *L;
    
public:
        
    LuaState()
    : L(luaS_newstate()) {
    }
    
    ~LuaState() {
        luaS_close(L);
    }
    
    lua_State* GetState() const {
        return L;
    }
    
    void DoFile(const std::string &path) 
    {
        lua_pushcfunction(L, &ShowDebugMessage);
        auto debugfunc = lua_gettop(L);
        luaL_loadfile(L, path.c_str());
        if (lua_pcall(L, 0, 0, debugfunc))
        {
            lua_pop(L, 1);
        }
        
        lua_remove(L, debugfunc);
    }
    
    void DoString(const std::string &content) 
    {
        lua_pushcfunction(L, &ShowDebugMessage);
        auto debugfunc = lua_gettop(L);
        luaL_loadstring(L, content.c_str());
        if (lua_pcall(L, 0, 0, debugfunc))
        {
            lua_pop(L, 1);
        }
        lua_remove(L, debugfunc);
    }
    
    void AddSearcher(lua_CFunction func)
    {
        luaS_addSearcher(L, func);
    }
    
    template<typename T>
    void SetGlobal(char const* name, T t)
    {
        Push(L, t);
        lua_setglobal(L, name);
    }
    
    LuaRef GetGlobal(const char *name) {
        return LuaRef::GetGlobal(L, name);
    }
    
    Namespace GlobalContext() {
        return Namespace::GetGlobalNamespace(L);
    }
    
    LuaRef NewNil() {
        return LuaRef(L);
    }
    
    template<typename T>
    LuaRef NewLuaRef(T v) {
        return LuaRef(L, v);
    }
    
private:
    /*
     * Copy is not allowed.
     */
    LuaState(const LuaState &other);
    LuaState& operator=(const LuaState &rhs);
    
};

