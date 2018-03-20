FROM gouthamve/vepc-ran:v0.3 

ADD pgw.out /pgw

ENTRYPOINT ["/pgw", "5", "5"]
