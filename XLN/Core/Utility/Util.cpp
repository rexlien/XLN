#include <XLN/Core/Utility/Util.h>

using namespace XCr;
using namespace std::chrono;

std::string TimeUtil::CurTimeToString()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    
    char buf[256];
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S-", std::localtime(&in_time_t));
    
    std::stringstream ss;
    ss << std::string(buf);//std::put_time(std::localtime(&in_time_t), "%Y-%m-%d-%H:%M:%S-");
    ss << millis;
    return ss.str();
}

std::string TimeUtil::TimeToString( std::chrono::time_point<std::chrono::system_clock> time)
{
    auto in_time_t = std::chrono::system_clock::to_time_t(time);
    
    auto duration = time.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    auto milliseconds = millis - (seconds * 1000);
    
    
    char buf[256];
    //tstruct = std::localtime(&in_time_t)
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H:%M:%S", std::localtime(&in_time_t));
    
    std::stringstream ss;
    ss << std::string(buf);
    ss << "." << milliseconds;
    return ss.str();
}

XCr::UInt64 ThreadUtil::GetCurThreadID()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return std::stoull(ss.str());
}

XCr::UInt32 TimeUtil::GetTickCount() {

    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

}


std::chrono::steady_clock::time_point TimeUtil::GetCurSteadyTimePoint() {
    return steady_clock::now();
}

