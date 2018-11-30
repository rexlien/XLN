#pragma once

#include <XLN/Core/Config/Config.h>

namespace XCr
{

//typedef boost::shared_ptr<T> SharedPtr<T> 
	/*
template<class T>
class SharedPtr : public std::shared_ptr<T>
{
    public:
        SharedPtr(T* ptr)
            :std::shared_ptr<T>(ptr)
        {
        
        }
        
        SharedPtr()
            :std::shared_ptr<T>()
        {
        
        }
        
        SharedPtr(const std::shared_ptr<T>& ptr)
            :std::shared_ptr<T>(ptr)
        {
        
        }
        
        
};
*/
template<class T>
using SharedPtr = std::shared_ptr<T>;

}

#define XLN_SHARED_PTR(classname) typedef XCr::SharedPtr<classname> classname##Ptr;