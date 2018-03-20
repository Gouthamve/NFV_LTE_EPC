FROM gouthamve/vepc-dist-base:v0.1 

ADD run_server.sh /run_server.sh 

ENTRYPOINT ["bash", "/run_server.sh", "0.0.0.0:8090"]
