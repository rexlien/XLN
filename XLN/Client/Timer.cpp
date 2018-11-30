//
//  Timer.cpp
//  LGCocosSample
//
//  Created by Rex Lien on 3/28/16.
//
//

#include "Timer.h"
#include <chrono>


using namespace LG::Core;

Timer::Timer()
{

}

Timer::~Timer()
{
    
}



void Timer::start()
{
 
    m_StartSystemTime = std::chrono::system_clock::now();
    m_StartTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    m_EndSystemTime =std::chrono::system_clock::now();
    m_EndTime = std::chrono::high_resolution_clock::now();

}

uint64_t Timer::getDuration(bool fromNow) const
{
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    if(fromNow)
        end = std::chrono::high_resolution_clock::now();
    else
        end = m_EndTime;
    
    std::chrono::duration<double> diff = (end - m_StartTime);
    return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

}

void Timer::printLog(const std::string& tag)
{
    //CCLOG("%s: duration: %d", tag.c_str(), (int)getDuration());
}

std::chrono::time_point<std::chrono::system_clock> Timer::getStartSystemTime() const
{
    return m_StartSystemTime;
}

std::chrono::time_point<std::chrono::system_clock> Timer::getEndSystemTime() const
{
    return m_EndSystemTime;
}





