FROM gouthamve/vepc-ran:v0.3 

ADD lb.out /lb

ENTRYPOINT ["/lb"]
