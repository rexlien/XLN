set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(cross_triple "x86_64-linux-gnu")

##set(CMAKE_C_COMPILER /usr/bin/${cross_triple}-gcc)
##set(CMAKE_CXX_COMPILER /usr/bin/${cross_triple}-g++)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "compile flags" FORCE)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}" CACHE STRING "compile flags" FORCE)

#set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

SET (CMAKE_C_COMPILER             "/usr/bin/clang")
SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set( CMAKE_C_COMPILER_ID Clang)
set( CMAKE_CXX_COMPILER_ID Clang)

SET (CMAKE_C_FLAGS "-Wall")
SET (CMAKE_CXX_FLAGS "-Wall")

SET (CMAKE_AR      "/usr/bin/llvm-ar-6.0" CACHE FILEPATH "Archiver")
SET (CMAKE_LINKER  "/usr/bin/llvm-link-6.0" CACHE FILEPATH "Linker")
SET (CMAKE_NM      "/usr/bin/llvm-nm-6.0")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump-6.0")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib-6.0")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -stdlib=libc++ -ggdb -g -O0 -fno-limit-debug-info -fno-omit-frame-pointer -fno-optimize-sibling-calls" CACHE STRING "compile flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -stdlib=libc++ -ggdb -g -O2" CACHE STRING "compile flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} -flto=thin -stdlib=libc++ -lc++abi -lsupc++ -ggdb -fno-omit-frame-pointer -fno-limit-debug-info -ldl" CACHE STRING "compile flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi -lsupc++ -ggdb -ldl -latomic" CACHE STRING "compile flags" FORCE)


include_directories("/usr/lib/clang/6.0/include")
include_directories("/usr/include/libcxxabi")

set(THREADS_PTHREAD_ARG "2" CACHE STRING "Forcibly set by CMakeLists.txt." FORCE)

#include_directories("/usr/lib/clang/4.0/include")
if(UNIX AND NOT ANDROID)
    set (LINUX_PC true)
endif()
