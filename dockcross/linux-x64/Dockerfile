FROM dockcross/base:latest
MAINTAINER Matt McCormick "matt.mccormick@kitware.com"


RUN sudo apt-get update && \
    apt-get install -y \
    g++ \
    automake \
    autoconf \
    autoconf-archive \
    libtool \
    libboost-all-dev \
    libevent-dev \
    libdouble-conversion-dev \
    libgflags-dev \
    liblz4-dev \
    liblzma-dev \
    libsnappy-dev \
    make \
    zlib1g-dev \
    binutils-dev \
    libjemalloc-dev \
    libssl-dev \
    pkg-config

RUN sudo apt-get install -y software-properties-common
RUN sudo add-apt-repository -y "deb http://apt.llvm.org/jessie/ llvm-toolchain-jessie main" && wget --quiet -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -
RUN sudo apt-get update && apt-get install -y libc++-dev
RUN sudo apt-get update && apt-get install -y clang


#RUN ln -s /usr/lib/x86_64-linux-gnu/* /usr/lib/

ENV CROSS_TRIPLE x86_64-linux-gnu
ENV CROSS_ROOT /usr/bin

ENV AS=/usr/bin/${CROSS_TRIPLE}-as \
    AR=/usr/bin/llvm-ar-6.0 \
    CC=/usr/bin/clang++ \
    CPP=/usr/bin/${CROSS_TRIPLE}-cpp \
    CXX=/usr/bin/clang++ \
    LD=/usr/bin/${CROSS_TRIPLE}-ld

COPY ${CROSS_TRIPLE}-noop.sh /usr/bin/${CROSS_TRIPLE}-noop

COPY Toolchain.cmake /usr/lib/${CROSS_TRIPLE}/
ENV CMAKE_TOOLCHAIN_FILE /usr/lib/${CROSS_TRIPLE}/Toolchain.cmake
ENV DEFAULT_DOCKCROSS_IMAGE xln_dockcross
# Build-time metadata as defined at http://label-schema.org
ARG BUILD_DATE
ARG IMAGE
ARG VCS_REF
ARG VCS_URL
LABEL org.label-schema.build-date=$BUILD_DATE \
      org.label-schema.name=$IMAGE \
      org.label-schema.vcs-ref=$VCS_REF \
      org.label-schema.vcs-url=$VCS_URL \
      org.label-schema.schema-version="1.0"
