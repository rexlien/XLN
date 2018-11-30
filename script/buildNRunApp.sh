#!/bin/bash
TARGET="${@: -1}"
export CONTAINER_NAME=$TARGET
CLEAN=0 
sudo docker stop $(sudo docker ps -q --filter ancestor=xln_vs-linux )
shopt -s nocasematch
for i in "$@"
do
case $i in
    -m=*|--mode=*)
    MODE="${i#*=}"
    shift # past argument=value
    ;;
    --clean|-c)
    CLEAN=1
    shift
    ;;
    *)
          # unknown option
    ;;
esac
done

if [[ $CLEAN = "1" ]]
then
    rm -rf ./build
fi

MODE=$(echo $MODE | awk '{ print tolower($0) }')
if [[ $MODE = "gdb" ]]
then
    ./XLN/docker-linux/xln_docker-linux cmake -Bbuild -H. -GNinja -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_TOOLCHAIN_FILE=/usr/lib/XLN/Toolchain.cmake -DLINUX_PC=true -DXLN_BUILD_SERVER=true && \
    ./XLN/docker-linux/xln_docker-linux ninja -Cbuild $TARGET && \
    ./XLN/docker-linux/xln_docker-linux -a '-p 10000-10010:10000-10010' bash -c 'gdbserver --wrapper env ASAN_OPTIONS=detect_leaks=0:detect_container_overflow=0:abort_on_error=1:log_path=./build/sanitizer.log -- localhost:10010 ./build/'"$TARGET"
elif [[ $MODE = "lldb" ]]
then
    ./XLN/docker-linux/xln_docker-linux cmake -Bbuild -H. -GNinja -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_TOOLCHAIN_FILE=/usr/lib/XLN/Toolchain.cmake -DLINUX_PC=true -DXLN_BUILD_SERVER=true && \
    ./XLN/docker-linux/xln_docker-linux ninja -Cbuild $TARGET && \
    ./XLN/docker-linux/xln_docker-linux -a '-p 10000-10020:10000-10020' bash -c "(lldb-server platform --listen \"*:10010\" --min-gdbserver-port 10011 --max-gdbserver-port 10020 --server &) && (ASAN_OPTIONS=detect_leaks=1:detect_container_overflow=0:log_path=./build/sanitizer.log:quarantine_size_mb=16 ./build/$TARGET)"
elif [[ $MODE = "debug" ]]
then
    ./XLN/docker-linux/xln_docker-linux cmake -Bbuild -H. -GNinja -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_TOOLCHAIN_FILE=/usr/lib/XLN/Toolchain.cmake -DLINUX_PC=true -DXLN_BUILD_SERVER=true && \
    ./XLN/docker-linux/xln_docker-linux ninja -Cbuild $TARGET && \
    ./XLN/docker-linux/xln_docker-linux -a '-p 10000-10010:10000-10010' bash -c 'ASAN_OPTIONS=detect_leaks=1:detect_container_overflow=0:log_path=./build/sanitizer.log:quarantine_size_mb=16 ./build/'"$TARGET"
else
    ./XLN/docker-linux/xln_docker-linux cmake -Bbuild -H. -GNinja -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_TOOLCHAIN_FILE=/usr/lib/XLN/Toolchain.cmake -DLINUX_PC=true -DXLN_BUILD_SERVER=true && \
    ./XLN/docker-linux/xln_docker-linux ninja -Cbuild $TARGET && \
    ./XLN/docker-linux/xln_docker-linux -a '-p 10000-10010:10000-10010' bash -c './build/'"$TARGET"
fi