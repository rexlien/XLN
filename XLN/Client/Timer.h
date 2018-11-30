//
//  Timer.hpp
//  LGCocosSample
//
//  Created by Rex Lien on 3/28/16.
//
//

#ifndef __Timer_h__
#define __Timer_h__

#include <string>
#include <chrono>

namespace LG
{
namespace Core
{
    class Timer
    {
        
        public:
            Timer();
            ~Timer();
        
            void start();
            void stop();
            uint64_t getDuration(bool fromNow = true) const;
        
            void printLog(const std::string& tag);
        
            std::chrono::time_point<std::chrono::system_clock> getStartSystemTime() const;
            std::chrono::time_point<std::chrono::system_clock> getEndSystemTime() const;
        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_EndTime;
        
            std::chrono::time_point<std::chrono::system_clock> m_StartSystemTime;
            std::chrono::time_point<std::chrono::system_clock> m_EndSystemTime;

        
    };



}
}

#endif /* Timer_hpp */
