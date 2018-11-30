# XLN (Crossline)

A general C++ frameowrk that facilitate C++ applicaiton development 

### Pre-requirement
* docker
* python

## Build/Run docker image for building XLN
* sh ./script/init_centos.sh (Linux only)
* sh build_vs-linux.sh
* sh run_vs-linux.sh

## Run application depends on XLN
* sh ./XLN/script/buildNRunApp.sh -m debug $APP_EXE_Project_Name
* sh ./XLN/script/buildNRunApp.sh -m gdb $APP_EXE_Project_Name (GDB attachable)


