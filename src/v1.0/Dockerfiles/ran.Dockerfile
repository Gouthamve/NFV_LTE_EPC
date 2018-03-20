FROM gouthamve/vepc-ran:v0.3 

ADD ransim.out /ransim 

ENTRYPOINT ["/ransim", "1", "10000"]
