//
//  ScopeTimer.cpp
//  LGCocosSample
//
//  Created by Rex Lien on 3/24/16.
//
//

#include "ScopeTimer.h"


using namespace XFr;

ScopeTimer::ScopeTimer(const std::string& tag)
:m_Tag(tag)
{
    m_StartTime = std::chrono::high_resolution_clock::now();
    
}

ScopeTimer::~ScopeTimer()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = (end - m_StartTime);
    uint64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    //CCLOG("%s: duration: %d", m_Tag.c_str(), (int)duration);
    
}



