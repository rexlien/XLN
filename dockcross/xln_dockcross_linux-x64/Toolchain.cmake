set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(cross_triple "x86_64-linux-gnu")

#set(CMAKE_C_COMPILER /usr/bin/${cross_triple}-gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/${cross_triple}-g++)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

SET (CMAKE_C_COMPILER             "/usr/bin/clang")
SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set( CMAKE_C_COMPILER_ID Clang)
set( CMAKE_CXX_COMPILER_ID Clang)

SET (CMAKE_C_FLAGS"-Wall")
SET (CMAKE_CXX_FLAGS"-Wall")

SET (CMAKE_AR      "/usr/bin/llvm-ar-4.0" CACHE FILEPATH "Archiver")
SET (CMAKE_LINKER  "/usr/bin/llvm-link-4.0" CACHE FILEPATH "Linker")
SET (CMAKE_NM      "/usr/bin/llvm-nm-4.0")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump-4.0")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib-4.0")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -glldb" CACHE STRING "compile flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi -glldb" CACHE STRING "compile flags" FORCE)

include_directories("/usr/lib/clang/4.0/include")

if(UNIX AND NOT ANDROID)
set (LINUX_PC true)
endif()
##set(CMAKE_CROSSCOMPILING_EMULATOR /usr/bin/${cross_triple}-noop)
