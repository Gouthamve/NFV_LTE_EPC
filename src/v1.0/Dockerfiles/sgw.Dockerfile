FROM gouthamve/vepc-ran:v0.3 

ADD sgw.out /sgw

ENTRYPOINT ["/sgw", "5", "5", "5"]
