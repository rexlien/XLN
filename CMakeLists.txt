cmake_minimum_required(VERSION 3.4)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")



project( XLN )

set(OPERATING_SYSTEM "${CMAKE_SYSTEM_NAME}")
set(OS "\"${CMAKE_SYSTEM_NAME}\"")

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
add_compile_definitions(_ENABLE_EXTENDED_ALIGNED_STORAGE)
endif()

include(${PROJECT_SOURCE_DIR}/thirdParty/thirdParty.cmake)
include_directories(${PROJECT_SOURCE_DIR})



if(CMAKE_BUILD_TYPE MATCHES RELEASE)
#add_definitions(-DXLN_SHIPPING)
endif()

add_subdirectory(XLN/Core)

if(IOS)
    add_subdirectory(XLN/React)
elseif(ANDROID)
    add_subdirectory(XLN/React)
    add_subdirectory(XLN/Client)
elseif(MSVC)
    add_subdirectory(XLN/Server)
    add_subdirectory(XLN/Network)
    if(NOT DEFINED XLN_BUILD_SERVER)
        add_subdirectory(XLN/Client)
    endif()
elseif(LINUX_PC)
    add_subdirectory(XLN/Server)
    add_subdirectory(XLN/Network)
endif()


add_subdirectory(Test)
