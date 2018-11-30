set(LIB_NAME XLNClient)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_BINARY_DIR}/XLN/Core/folly)

file(GLOB CLIENT_SRC "*.cpp")

set(SOURCE
   ${SOURCE}
   ${CLIENT_SRC}
  
)


file(GLOB CLIENT_HEADER "*.h")

set(HHEADERS
   ${HHEADERS}
   ${CLIENT_HEADER}
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

target_link_libraries(${LIB_NAME} Core)






