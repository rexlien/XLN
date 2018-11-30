//
//  ScopeTimer.hpp
//  LGCocosSample
//
//  Created by Rex Lien on 3/24/16.
//
//

#ifndef __XLN_ScopeTimer_h__
#define __XLN_ScopeTimer_h__

#include <string>
#include <chrono>

namespace XFr
{

class ScopeTimer
{
public:
    ScopeTimer(const std::string& tag);
    ~ScopeTimer();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
    std::string m_Tag;
};

}


#endif /* ScopeTimer_hpp */
