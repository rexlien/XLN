set(LIB_NAME Core)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/thirdParty/folly)

file(GLOB CORE_SRC 
  "Foundation/*.cpp" "Foundation/*/*.cpp" "Network/*.cpp" "Network/*/*.cpp" "Platform/*.cpp" "Framework/*.cpp" "Framework/**/*.cpp" "Utility/*.cpp" "DebugHelper/*.cpp" "DebugHelper/Memory/*.cpp" "*.cpp")

if(IOS)
file(GLOB PLATFORM_SRC "Platform/ios/*.mm")
file(GLOB PLATFORM_HEADER "Platform/ios/*.h")
elseif(ANDROID)
file(GLOB PLATFORM_SRC "Platform/android/src/main/cpp/*.cpp")
file(GLOB PLATFORM_HEADER "Platform/android/src/main/cpp/*.h")
elseif(MSVC)
file(GLOB PLATFORM_SRC "Platform/win/*.cpp")
file(GLOB PLATFORM_HEADER "Platform/win/*.h")
elseif(LINUX_PC)
file(GLOB PLATFORM_SRC "Platform/linux/*.cpp")
file(GLOB PLATFORM_HEADER "Platform/linux/*.h")
endif()

find_package(Protobuf REQUIRED)
##PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER Proto/message.proto)


set(SOURCE
   ${SOURCE}
   ${PLATFORM_SRC}
   ${CORE_SRC}
   ${PROTO_SRC}
  
)
if(NOT LINUX_PC)
  ##list(REMOVE_ITEM SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
endif()

file(GLOB CORE_HEADER "Foundation/*.h" "Foundation/*/*.h" "Framework/*.h" "Network/*.h" "Network/*/*.h"
   "DebugHelper/*.h" "DebugHelper/Memory/*.h" "Utility/*.h" "*.h" "Config/*.h" "Platform/*.h")




set(HHEADERS
   ${HHEADERS}
   ${PLATFORM_HEADER}
   ${CORE_HEADER}
   ${PROTO_HEADER}
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

xln_auto_source_group(${LIB_NAME} ${PROJECT_SOURCE_DIR}/XLN/Core ${SOURCEfiles} ${HHEADERS})

target_link_libraries(${LIB_NAME} PUBLIC libevent)
target_link_libraries(${LIB_NAME} PUBLIC double-conversion)
target_link_libraries(${LIB_NAME} PUBLIC glog)
if(MSVC)
target_compile_definitions(${LIB_NAME} PUBLIC GOOGLE_GLOG_DLL_DECL=)
target_compile_definitions(${LIB_NAME} PUBLIC GLOG_NO_ABBREVIATED_SEVERITIES)
endif()
target_link_libraries(${LIB_NAME} PUBLIC ${Protobuf_LIBRARIES})

if(XLN_BUILD_SERVER)
  target_link_libraries(${LIB_NAME} PUBLIC thriftcpp2)
  
  if(MSVC)
    target_link_libraries(${LIB_NAME} PUBLIC gss)
  endif()
else()
  target_link_libraries(${LIB_NAME} PUBLIC libcurl)
  target_link_libraries(${LIB_NAME} PUBLIC nghttp2)
endif()

target_link_libraries(${LIB_NAME} PUBLIC wangle)
target_link_libraries(${LIB_NAME} PUBLIC folly)

if(ANDROID)
  
  target_link_libraries(${LIB_NAME} PUBLIC log)
  target_link_libraries(${LIB_NAME} PUBLIC atomic)
  target_link_libraries(${LIB_NAME} PUBLIC z)
  target_link_libraries(${LIB_NAME} PUBLIC m)
  target_link_libraries(${LIB_NAME} PUBLIC android)
  target_link_libraries(${LIB_NAME} PUBLIC BoostLibAtomic)
  target_link_libraries(${LIB_NAME} PUBLIC BoostLibSystem)
  target_link_libraries(${LIB_NAME} PUBLIC BoostLibFileSystem)
  target_link_libraries(${LIB_NAME} PUBLIC BoostLibRegex)
  target_link_libraries(${LIB_NAME} PUBLIC BoostLibThread)
  target_link_libraries(${LIB_NAME} PUBLIC BoostLibContext)
  target_link_libraries(${LIB_NAME} PUBLIC FFCallLib)
  include(AndroidNdkModules)
  android_ndk_import_module_cpufeatures()
  target_link_libraries(${LIB_NAME} PUBLIC cpufeatures)

elseif(IOS)
  target_link_libraries(${LIB_NAME} PUBLIC BoostLib)
elseif(LINUX_PC)
  include(${PROJECT_SOURCE_DIR}/cmake/find_deps.cmake)
  target_link_libraries(${LIB_NAME} PUBLIC m Boost::system Boost::thread)
elseif(MSVC)
  target_link_libraries(${LIB_NAME} PUBLIC pthread)
  target_link_libraries(${LIB_NAME} PUBLIC shlwapi)
endif()




if(NOT LINUX_PC)

if(IOS)
target_link_libraries(${LIB_NAME} PUBLIC OpenSSL:SSL)
target_link_libraries(${LIB_NAME} PUBLIC OpenSSL::Crypto)
else()
target_link_libraries(${LIB_NAME} PUBLIC OpenSSL)
target_link_libraries(${LIB_NAME} PUBLIC CryptoLib)
endif()

endif()

if(IOS)
  include(${PROJECT_SOURCE_DIR}/cmake/xcode.cmake)
  xcode_lib_standard_setting(${LIB_NAME})
  combine_static_lib(XLNCoreFat Core)
  set_target_properties(XLNCoreFat PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY_DEBUG "/$(BUILD_DIR)/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)"
                      ARCHIVE_OUTPUT_DIRECTORY_RELEASE "/$(BUILD_DIR)/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)")
  
endif()






