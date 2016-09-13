#ifndef _LUA_PORTAL_H_
#define _LUA_PORTAL_H_
// All #include dependencies are listed here
// instead of in the individual header files.
//
#include<cassert>
#include<sstream>
#include<stdexcept>
#include<string>
#include<typeinfo>
#include<functional>
#include<memory>
#include<type_traits>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32


namespace luaportal
{
    
    // Forward declaration
    template<typename T>
    struct Stack;
#include "impl/utils.h"
#include "impl/luahelpers.h"
#include "impl/typetraits.h"
#include "impl/functraits.h"
#include "impl/callabletraits.h"
#include "impl/classinfo.h"
#include "impl/userdata.h"
#include "impl/constructor.h"
#include "impl/stack.h"
    
    class LuaRef;
    
#include "impl/luaref.h"
#include "impl/iterator.h"
    
    //------------------------------------------------------------------------------
    /**
     security options.
     */
    class Security
    {
    public:
        static bool HideMetatables()
        {
            return GetSettings().HideMetatables;
        }
        
        static void SetHideMetatables(bool shouldHide)
        {
            GetSettings().HideMetatables = shouldHide;
        }
        
    private:
        struct Settings
        {
            Settings() : HideMetatables(true)
            {
            }
            
            bool HideMetatables;
        };
        
        static Settings& GetSettings()
        {
            static Settings settings;
            return settings;
        }
    };
    
#include "impl/cfunctions.h"
#include "impl/namespace.h"
#include "impl/luastate.h"
    
    //------------------------------------------------------------------------------
    /**
     Change whether or not metatables are hidden(on by default).
     */
    inline void SetHideMetatables(bool shouldHide)
    {
        Security::SetHideMetatables(shouldHide);
    }
    
}

#endif
