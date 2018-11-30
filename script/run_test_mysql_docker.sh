mkdir ~/test_mysql
sudo docker stop xln_test_mysql
sudo docker rm xln_test_mysql
sudo docker build -t xln_test_mysql ../docker-linux/mysql
sudo docker run --name xln_test_mysql -v ~/test_mysql:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=1234 -e MYSQL_DATABASE=db -e MYSQL_USER=user -e MYSQL_PASSWORD=1234 -p 3306:3306 -d xln_test_mysql