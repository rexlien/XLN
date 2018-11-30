set(LIB_NAME XLNReact)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/xcode.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Core)

file(GLOB SRC "*.cpp")
file(GLOB HEADER "*.h")

set(SOURCE
   ${SOURCE}
   ${SRC}
)

set(HHEADERS
   ${HHEADERS}
   ${HEADER}
)

if(ANDROID)
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
  xcode_lib_standard_setting(${LIB_NAME})
endif()
target_link_libraries(${LIB_NAME} PUBLIC Core)
