set(EXE_NAME TestServer)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/XLN)

include(${PROJECT_SOURCE_DIR}/thirdParty/thirdParty_include.cmake)
include(${PROJECT_SOURCE_DIR}/thirdParty/app_include.cmake)

set(THRIFT_SRC "")
  set(LIB_CPP2_HOME ${CMAKE_CURRENT_SOURCE_DIR})
  thrift_generate(
    "EntityService" #file_name
    "EntityService" #services
    "cpp2" #language
    "" #options
    "${LIB_CPP2_HOME}" #file_path
    "${LIB_CPP2_HOME}" #output_path
  )
  
list(APPEND THRIFT_SRC ${EntityService-cpp2-SOURCES})



add_executable(
  ${EXE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestServerSystemFactoryService.cpp
  ${THRIFT_SRC}
)

if(MSVC)
target_compile_definitions(${EXE_NAME} PUBLIC GOOGLE_GLOG_DLL_DECL=)
target_compile_definitions(${EXE_NAME} PUBLIC GLOG_NO_ABBREVIATED_SEVERITIES)
endif()

target_link_libraries(${EXE_NAME} Core)
target_link_libraries(${EXE_NAME} XLNServer)

if(MSVC)
  add_custom_command ( TARGET ${EXE_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:pthread> 
    $<TARGET_FILE_DIR:${EXE_NAME}> 
  )
  ##TODO: really don't know why VS throws pthread.lib not found error. and there's no pthread.lib set in the link commands.
  SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"pthread.lib\") 
endif()