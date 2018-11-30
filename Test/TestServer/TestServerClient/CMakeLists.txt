set(EXE_NAME TestServerClient)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/XLN/Core)
##include_directories(${PROJECT_SOURCE_DIR}/XLN/Core/folly)

add_executable(
  ${EXE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TestServerClientSystemFactoryService.cpp
)

target_link_libraries(${EXE_NAME} Core)
target_link_libraries(${EXE_NAME} XLNServer)
