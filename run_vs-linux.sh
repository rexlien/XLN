sudo docker stop xln_vs-linux
sudo docker rm xln_vs-linux
sudo docker run -d -p 12345:22 -v $(pwd):/home/XLN --name xln_vs-linux --security-opt seccomp:unconfined xln_vs-linux