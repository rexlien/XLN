set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(cross_triple "x86_64-linux-gnu")

#set(CMAKE_C_COMPILER /usr/bin/${cross_triple}-gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/${cross_triple}-g++)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

SET (CMAKE_C_COMPILER             "/usr/bin/clang")
SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")

SET (CMAKE_AR      "/usr/bin/llvm-ar-6.0" CACHE FILEPATH "Archiver")
SET (CMAKE_LINKER  "/usr/bin/llvm-link-6.0" CACHE FILEPATH "Linker")
SET (CMAKE_NM      "/usr/bin/llvm-nm-6.0")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump-6.0")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib-6.0")

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

if(UNIX AND NOT ANDROID)
set (LINUX_PC true)
endif()
##set(CMAKE_CROSSCOMPILING_EMULATOR /usr/bin/${cross_triple}-noop)
