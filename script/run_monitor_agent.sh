
which osqueryi
if [ $? -ne 0 ]
then
sudo rpm -ivh https://osquery-packages.s3.amazonaws.com/centos7/noarch/osquery-s3-centos7-repo-1-0.0.noarch.rpm
sudo yum install -y osquery
fi

sudo mkdir -p /var/osquery
sudo mkdir -p ../../osquery
sudo pkill osqueryd
sudo osqueryd --config_path osquery.conf --logger_path='../../osquery/' --allow_unsafe=true > /dev/null 2>&1 &
sudo docker stop xln_logstash
sudo docker rm xln_logstash
sudo docker build -t xln_logstash ../docker-linux/logstash
sudo docker run --name xln_logstash -e XPACK_MONITORING_ENABLED=false -v $PWD/../../osquery:/var/log/osquery/ --rm -it -d --user root xln_logstash