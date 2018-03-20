FROM gouthamve/vepc-hss:v0.2 

ADD hss.out /hss
ADD hss/run_hss.sh /run_hss.sh

ENTRYPOINT ["bash", "/run_hss.sh"]
