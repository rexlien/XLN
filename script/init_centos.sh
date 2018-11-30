#!/bin/bash
which docker
if [ $? -eq 0 ]
then
    docker --version | grep "1.0"
    if [ $? -eq 0 ]
    then
        echo "docker existing"
    else
        echo "wrong version"
    fi
else
    sudo yum -y install -y yum-utils device-mapper-persistent-data lvm2
    sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
    sudo yum makecache fast
    yum install -y --setopt=obsoletes=0 docker-ce-17.03.1.ce-1.el7.centos docker-ce-selinux-17.03.1.ce-1.el7.centos
    sudo mkdir -p /etc/docker/
    sudo touch /etc/docker/daemon.json
    echo "{\"storage-driver\": \"devicemapper\"}" | sudo tee /etc/docker/daemon.json
    sudo systemctl start docker
    sudo /etc/init.d/ds_agent stop || true
fi
