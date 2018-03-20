FROM gouthamve/vepc-dist-base:v0.1 

ADD loc.out /loc

ENTRYPOINT ["/loc", "10"]
