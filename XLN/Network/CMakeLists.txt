set(LIB_NAME XLNNetwork)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_BINARY_DIR}/XLN/Core/folly)

set(THRIFT1 ${CMAKE_BINARY_DIR}/bin/thrift1)
set(THRIFT_TEMPLATES ${PROJECT_SOURCE_DIR}/thirdParty/fbthrift/thrift/compiler/generate/templates)

set(THRIFT_SRC "")
set(LIB_CPP2_HOME ${CMAKE_CURRENT_SOURCE_DIR})
  thrift_generate(
    "ActorService" #file_name
    "ActorService" #services
    "cpp2" #language
    "" #options
    "${CMAKE_CURRENT_SOURCE_DIR}/../../XLNProtocol/thrift" #file_path
    "${LIB_CPP2_HOME}" #output_path
  )

bypass_source_check("{ActorService-cpp2-SOURCES}")
list(APPEND THRIFT_SRC ${ActorService-cpp2-SOURCES})

set(LIB_CPP2_HOME ${CMAKE_CURRENT_SOURCE_DIR})
  thrift_generate(
    "NetworkService" #file_name
    "NetworkService" #services
    "cpp2" #language
    "" #options
    "${CMAKE_CURRENT_SOURCE_DIR}/../../XLNProtocol/thrift" #file_path
    "${LIB_CPP2_HOME}" #output_path
  )

bypass_source_check("{NetworkService-cpp2-SOURCES}")
list(APPEND THRIFT_SRC ${NetworkService-cpp2-SOURCES})

file(GLOB_RECURSE NETWORK_SRC "*.cpp")

set(SOURCE
   ${SOURCE}
   ${NETWORK_SRC}  
   ${THRIFT_SRC}
)

file(GLOB_RECURSE NETWORK_HEADER "*.h" )

set(HHEADERS
   ${HHEADERS}
   ${NETWORK_HEADER}
)

if(IOS)
add_library(
  ${LIB_NAME}
  STATIC
  ${HHEADERS} ${SOURCE}
  )
elseif(ANDROID)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wall -v -Wl,--no-warn-shared-textrel")
  add_library(
  ${LIB_NAME}
  SHARED
  ${HHEADERS} ${SOURCE}
  )
else()
  add_library(
  ${LIB_NAME}
  STATIC
  ${HHEADERS} ${SOURCE}
  )
endif()



xln_auto_source_group(${LIB_NAME} ${PROJECT_SOURCE_DIR}/XLN/Network ${SOURCE} ${HHEADERS})

target_link_libraries(${LIB_NAME} Core)

if(MSVC)
  target_compile_definitions (${LIB_NAME} PUBLIC GLOG_NO_ABBREVIATED_SEVERITIES)
endif()







