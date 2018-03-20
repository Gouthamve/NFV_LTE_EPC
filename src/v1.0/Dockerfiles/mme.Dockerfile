FROM gouthamve/vepc-dist-base:v0.1 

ADD mme-dist.out /mme

ENTRYPOINT ["/mme", "10"]
