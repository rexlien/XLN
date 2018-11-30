set(LIB_NAME XLNServer)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_BINARY_DIR}/XLN/Core/folly)

xln_auto_sources(SQUANGLE_SRC "*.cpp" "RECURSE" "${PROJECT_SOURCE_DIR}/thirdParty/squangle/")
xln_auto_sources(SQUANGLE_HEADER "*.h" "RECURSE" "${PROJECT_SOURCE_DIR}/thirdParty/squangle")

file(GLOB_RECURSE SERVER_SRC "*.cpp")

set(SOURCE
   ${SOURCE}
   ${SERVER_SRC} 
   ${SQUANGLE_SRC} 
)

file(GLOB_RECURSE SERVER_HEADER "*.h" )

set(HHEADERS
   ${HHEADERS}
   ${SERVER_HEADER}
   ${SQUANGLE_HEADER}
)
####



add_library(
${LIB_NAME}
STATIC
${HHEADERS} ${SOURCE}
)


xln_auto_source_group(${LIB_NAME} ${PROJECT_SOURCE_DIR}/XLN/Server ${SOURCE} ${HHEADERS})

if(LINUX_PC)
  add_dependencies(${LIB_NAME} webscalesqlclient)
endif()

target_compile_definitions(${LIB_NAME} PUBLIC -DNOMINMAX)

target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/thirdParty/squangle)
target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/thirdParty/myrocks/include)
if(MSVC)
  target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/thirdParty/build/msvc/libmysql/include)
else()
  target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/thirdParty/build/linux/x64/libmysql/include)
endif()
target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/thirdParty/re2)
target_link_libraries(${LIB_NAME} Core)

target_link_libraries(${LIB_NAME} libmysql)
target_link_libraries(${LIB_NAME} fbmysqlclient)

target_link_libraries(${LIB_NAME} re2)
target_link_libraries(${LIB_NAME} XLNNetwork)

if(MSVC)
  target_link_libraries(${LIB_NAME} mysql_auth_win)
  target_compile_definitions (XLNServer PUBLIC GLOG_NO_ABBREVIATED_SEVERITIES)
endif()







