#ifndef __XPECTOOL_H__
#define __XPECTOOL_H__

#include <boost/lockfree/stack.hpp>
#include <boost/thread/mutex.hpp>

namespace XCr
{

class IFreeList
{
public:
    virtual ~IFreeList() = default;
};

template <class T, int N = 1024>
class FreeList : public IFreeList
{
public:

    FreeList(bool preAllocate = true)
        :m_CurSize(0)
    {
        if(preAllocate)
            Grow(ms_Reserved);

    }

    virtual ~FreeList()
    {

        Free();
    }

    T* Aquire()
    {   
        T* ret = 0;
        if(m_FreeList.empty())
        {
            T* newObj = XLN_OBJ_NEW T();
            m_FreeList.push(newObj);
            newObj = new (newObj) T;
            return newObj;
        }

        m_FreeList.pop(ret);
        ret = new (ret) T;

        return ret;
    }
    void Release(T* releaseObj)
    {
        releaseObj->~T();
        m_FreeList.push(releaseObj);
        
    }
    void Free()
    {
        while(!m_FreeList.empty())
        {

            T* ret = 0;
            m_FreeList.pop(ret);
            if(ret)
            {   ret->~T();
                XLN_OBJ_DELETE (ret);
            }

        }

    }
private:

    //not thread safe, only called in constructor
    void Grow(unsigned int targetSize)
    {

        unsigned int sizeNeeded = targetSize - m_CurSize.load();
        for(unsigned int i = 0; i < sizeNeeded; i++)
        {
            T* newObj = XLN_OBJ_NEW T();
            m_FreeList.push(newObj);
        }
        m_CurSize = targetSize;

    }
    static const int ms_Reserved = N;

    std::atomic<unsigned int> m_CurSize;
    boost::lockfree::stack<T*, boost::lockfree::capacity<N>> m_FreeList;


};

}

#endif