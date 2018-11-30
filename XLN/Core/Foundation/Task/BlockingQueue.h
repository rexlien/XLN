#pragma once

namespace XCr
{

enum class QueueBehaviorIfFull 
{   THROW, 
    BLOCK
};

class QueueFullException : public std::runtime_error 
{
  using std::runtime_error::runtime_error; 
};

template<class T>
class XLN_CLASS_DECL BlockingQueue
{
    public:
        virtual ~BlockingQueue() = default;
        virtual void Add(T task) = 0;
        virtual T Take() = 0;

        virtual std::size_t GetSize() const = 0;

};


}