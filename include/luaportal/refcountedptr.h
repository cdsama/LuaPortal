#pragma once
#include<unordered_map>

//==============================================================================
/**
 Support for our RefCountedPtr.
 */
struct RefCountedPtrBase
{
    // Declaration of container for the refcounts
    typedef std::unordered_map<const void *, int> RefCountsType;
    
protected:
    inline RefCountsType& getRefCounts()
    {
        static RefCountsType refcounts;
        return refcounts ;
    }
};

//==============================================================================
/**
 A reference counted smart pointer.
 
 The api is compatible with boost::RefCountedPtr and std::RefCountedPtr, in the
 sense that it implements a strict subset of the functionality.
 
 This implementation uses a hash table to look up the reference count
 associated with a particular pointer.
 
 @tparam T The class type.
 
 @todo Decompose RefCountedPtr using a policy. At a minimum, the underlying
 reference count should be policy based(to support atomic operations)
 and the delete behavior should be policy based(to support custom
 disposal methods).
 
 @todo Provide an intrusive version of RefCountedPtr.
 */
template<typename T>
class RefCountedPtr : private RefCountedPtrBase
{
public:
    template<typename Other>
    struct rebind
    {
        typedef RefCountedPtr<Other> other;
    };
    
    /** Construct as nullptr or from existing pointer to T.
     
     @param p The optional, existing pointer to assign from.
     */
    RefCountedPtr(T* p = 0) : m_p(p)
    {
        ++getRefCounts() [m_p];
    }
    
    /** Construct from another RefCountedPtr.
     
     @param rhs The RefCountedPtr to assign from.
     */
    RefCountedPtr(RefCountedPtr<T> const& rhs) : m_p(rhs.Get())
    {
        ++getRefCounts() [m_p];
    }
    
    /** Construct from a RefCountedPtr of a different type.
     
     @invariant A pointer to U must be convertible to a pointer to T.
     
     @param  rhs The RefCountedPtr to assign from.
     @tparam U   The other object type.
     */
    template<typename U>
    RefCountedPtr(RefCountedPtr<U> const& rhs) : m_p(static_cast<T*>(rhs.Get()))
    {
        ++getRefCounts() [m_p];
    }
    
    /** Release the object.
     
     If there are no more references then the object is deleted.
     */
    ~RefCountedPtr()
    {
        reset();
    }
    
    /** Assign from another RefCountedPtr.
     
     @param  rhs The RefCountedPtr to assign from.
     @return     A reference to the RefCountedPtr.
     */
    RefCountedPtr<T>& operator=(RefCountedPtr<T> const& rhs)
    {
        if(m_p != rhs.m_p)
        {
            reset();
            m_p = rhs.m_p;
            ++getRefCounts() [m_p];
        }
        return *this;
    }
    
    /** Assign from another RefCountedPtr of a different type.
     
     @note A pointer to U must be convertible to a pointer to T.
     
     @tparam U   The other object type.
     @param  rhs The other RefCountedPtr to assign from.
     @return     A reference to the RefCountedPtr.
     */
    template<typename U>
    RefCountedPtr<T>& operator=(RefCountedPtr<U> const& rhs)
    {
        reset();
        m_p = static_cast<T*>(rhs.Get());
        ++getRefCounts() [m_p];
        return *this;
    }
    
    /** Retrieve the raw pointer.
     
     @return A pointer to the object.
     */
    T* Get() const
    {
        return m_p;
    }
    
    /** Retrieve the raw pointer.
     
     @return A pointer to the object.
     */
    T* operator*() const
    {
        return m_p;
    }
    
    /** Retrieve the raw pointer.
     
     @return A pointer to the object.
     */
    T* operator->() const
    {
        return m_p;
    }
    
    /** Determine the number of references.
     
     @note This is not thread-safe.
     
     @return The number of active references.
     */
    long use_count() const
    {
        return getRefCounts() [m_p];
    }
    
    /** Release the pointer.
     
     The reference count is decremented. If the reference count reaches
     zero, the object is deleted.
     */
    void reset()
    {
        if(m_p != 0)
        {
            if(--getRefCounts() [m_p]<= 0)
                delete m_p;
            
            m_p = 0;
        }
    }
    
private:
    T* m_p;
};

//==============================================================================

namespace luaportal
{
    
    // forward declaration
    template<typename T>
    struct ContainerTraits;
    
    template<typename T>
    struct ContainerTraits<RefCountedPtr<T> >
    {
        typedef T Type;
        
        static T* Get(RefCountedPtr<T> const& c)
        {
            return c.Get();
        }
    };
    
}
