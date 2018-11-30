FROM ubuntu:18.04

RUN apt-get update && \
	apt-get install -y openssh-server gdb gdbserver sudo build-essential git && \
	mkdir /var/run/sshd && \
	echo 'root:toor' | chpasswd && \
	sed -i -E 's/#\s*PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config && \
	sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd && \
	apt-get clean

VOLUME /usr/src
WORKDIR /usr/src

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]

RUN sudo apt-get update && sudo apt-get upgrade -y && \
sudo apt-get install -y build-essential && \
sudo apt-get install -y aptitude

RUN sudo apt-get update && \
sudo apt-get install -y git cmake && \
sudo git clone https://github.com/Kitware/CMake.git && \
cd CMake && \
sudo git checkout tags/v3.11.4 && \
mkdir out && \
cd out && \
cmake ../ && \
make && \
sudo make install

#for folly
RUN sudo apt-get update && \
    apt-get install -y \
    g++ \
    automake \
    autoconf \
    autoconf-archive \
    libtool \
    liblz4-dev \
    liblzma-dev \
    libsnappy-dev \
    make \
    zlib1g-dev \
    binutils-dev \
    libjemalloc-dev \
    libssl-dev \
    pkg-config

RUN sudo apt-get update && \
  apt-get install ninja-build

RUN sudo apt-get install -y software-properties-common
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add -
RUN sudo add-apt-repository -y "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-6.0 main" 
#Fingerprint: 6084 F3CF 814B 57C1 CF12 EFD5 15CF 4D18 AF4F 7421
RUN sudo apt-get update && apt-get install -y clang-6.0 lldb-6.0
#RUN sudo apt-get update && apt-get install -y libc++-dev
#RUN sudo apt-get update && apt-get install -y libc++abi-dev
RUN sudo apt-get install -y libstdc++6
RUN sudo apt-get update && apt-get install -y gdb


ENV CROSS_TRIPLE x86_64-linux-gnu
ENV CROSS_ROOT /usr/bin

ENV AS=/usr/bin/${CROSS_TRIPLE}-as \
    AR=/usr/bin/llvm-ar-6.0 \
    CC=/usr/bin/clang \
    CPP=/usr/bin/${CROSS_TRIPLE}-cpp \
    CXX=/usr/bin/clang++ \
    LD=/usr/bin/${CROSS_TRIPLE}-ld

RUN sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-6.0 100
RUN sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-6.0 100
RUN sudo update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-6.0 100
RUN sudo update-alternatives --install /usr/bin/lldb-server lldb-server /usr/bin/lldb-server-6.0 100
RUN sudo update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-6.0 100

RUN sudo apt-get update && \
  apt-get install -y subversion

#ENV CPP_INCLUDE_PATHS= echo | c++ -Wp,-v -x c++ - -fsyntax-only 2>&1 \
#  |grep ' /usr'|tr '\n' ' '|tr -s ' ' |tr ' ' ';' \
#CPP_INCLUDE_PATHS="/usr/include/c++/v1/;$CPP_INCLUDE_PATHS"

RUN svn co http://llvm.org/svn/llvm-project/libcxx/branches/release_50/ ~/libcxx && \
svn co http://llvm.org/svn/llvm-project/libcxxabi/branches/release_50/ ~/libcxxabi && \
mkdir -p ~/libcxx/build ~/libcxxabi/build
RUN cd ~/libcxx/build && \
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_CONFIG_PATH=/usr/bin/llvm-config-6.0 -DCMAKE_INSTALL_PREFIX=/usr .. && \
make install
RUN cd ~/libcxxabi/build && \
cmake -G "Unix Makefiles" -DLIBCXX_CXX_ABI=libstdc++ \
      -DLIBCXX_LIBSUPCXX_INCLUDE_PATHS="$CPP_INCLUDE_PATHS" \
      -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr \
      -DLLVM_CONFIG_PATH=/usr/bin/llvm-config-6.0 \
      -DLIBCXXABI_LIBCXX_INCLUDES=../../libcxx/include  .. && \
make install && \
cd ~/libcxx/build && \
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr \
      -DLIBCXX_CXX_ABI=libcxxabi -DLLVM_CONFIG_PATH=/usr/bin/llvm-config-6.0\
      -DLIBCXX_CXX_ABI_INCLUDE_PATHS=../../libcxxabi/include .. \
&& make install

RUN sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100
RUN sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang-6.0 100

RUN wget --no-check-certificate https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz && \
tar xzf boost_1_67_0.tar.gz && \
cd boost_1_67_0 && \
./bootstrap.sh --with-toolset=clang --prefix=/usr && \
./b2 clean && \
./b2 install toolset=clang cxxflags="-std=c++11 -stdlib=libc++" linkflags="-stdlib=libc++" --prefix=/usr -j 4 --without-python
## stage release
##./b2 install toolset=clang cxxflags="-std=c++11 -stdlib=libc++" linkflags="-stdlib=libc++" --prefix=/usr

#libevent-dev \
#libgflags-dev \

#for fbthrift
RUN sudo apt-get update && apt-get install -y flex bison libkrb5-dev libsasl2-dev

COPY Toolchain.cmake /usr/lib/XLN/

RUN git clone https://github.com/no1msd/mstch.git && \
cd mstch/ && \
mkdir build && \
cd build && \
cmake -DCMAKE_TOOLCHAIN_FILE=/usr/lib/XLN/Toolchain.cmake .. && \
make && \
sudo make install
#RUN ln -s /usr/lib/x86_64-linux-gnu/* /usr/lib/

RUN wget --no-check-certificate https://github.com/facebook/zstd/archive/v1.3.2.tar.gz && \
mkdir zstd && \
tar xzf v1.3.2.tar.gz && \
cd zstd-1.3.2 && \
sudo make install
#mkdir build && \
#cd build && \
#cmake -DCMAKE_TOOLCHAIN_FILE=/usr/lib/XLN/Toolchain.cmake .. && \
#make && \
#sudo make install


##for mysql server/client
RUN sudo apt-get install -y libncurses5-dev libncursesw5-dev \
  libreadline7

WORKDIR /work