set(EXE_NAME XLNGTest)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../thirdParty/app_include.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/XLN/Core)

set(PLATFORM_SRC "")
if(IOS)
  list(APPEND PLATFORM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.mm)
else()
  list(APPEND PLATFORM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
endif()

set(SERVER_SRC "")
if(XLN_BUILD_SERVER)
  list(APPEND SERVER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestThrift.cpp)
endif()

set(THRIFT_SRC "")
if(XLN_BUILD_SERVER)
  set(LIB_CPP2_HOME ${CMAKE_CURRENT_SOURCE_DIR}/Test)
  thrift_generate(
    "TestThrift" #file_name
    "TestService" #services
    "cpp2" #language
    "" #options
    "${LIB_CPP2_HOME}" #file_path
    "${LIB_CPP2_HOME}" #output_path
  )
  thrift_generate(
    "FutureThrift" #file_name
    "FutureService" #services
    "cpp2" #language
    "" #options
    "${LIB_CPP2_HOME}" #file_path
    "${LIB_CPP2_HOME}" #output_path
  )

  list(APPEND THRIFT_SRC ${TestThrift-cpp2-SOURCES} ${FutureThrift-cpp2-SOURCES})
  
endif()

if(NOT ANDROID)
add_executable(
    ${EXE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/GTestApplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestFolly.cpp
    ${PLATFORM_SRC}
    ${SERVER_SRC}
    ${THRIFT_SRC}
  )
else()
  add_library(
    ${EXE_NAME}
    SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/GTestApplication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Test/TestFolly.cpp
    ${PLATFORM_SRC}
    ${SERVER_SRC}
    ${THRIFT_SRC}
  )
endif()

target_include_directories(${EXE_NAME} PRIVATE ${ThirdPartyRoot}/gtest/googletest/include)
target_link_libraries(${EXE_NAME} Core)
target_link_libraries(${EXE_NAME} gtest)
target_link_libraries(${EXE_NAME} gmock)

if(XLN_BUILD_SERVER)
  target_link_libraries(${EXE_NAME} XLNServer)
else()
  target_link_libraries(${EXE_NAME} XLNClient)
endif()

if(MSVC)
  add_custom_command ( TARGET ${EXE_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:pthread> 
    $<TARGET_FILE_DIR:${EXE_NAME}> 
  )
  ##TODO: really don't know why VS throws pthread.lib not found error. and there's no pthread.lib set in the link commands.
  SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"pthread.lib\") 
endif()

