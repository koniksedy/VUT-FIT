# Dockerfile
# UPA project
# Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
#          Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
#          Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 28.09.2022


# Ubuntu 22.04 is not yet supported. 
FROM ubuntu:20.04

# Install MongoDB
RUN \
    apt-get update && \
    apt-get install -y gnupg && \
    apt-get install -y wget && \
    wget -qO - https://www.mongodb.org/static/pgp/server-6.0.asc | apt-key add - && \
    echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu focal/mongodb-org/6.0 multiverse" | tee /etc/apt/sources.list.d/mongodb-org-6.0.list && \
    apt-get update && \
    apt-get install -y mongodb-org


VOLUME [ "/data/db" ]

WORKDIR /data

CMD ["mongod", "--bind_ip_all"]

# Expose ports
EXPOSE 27017
EXPOSE 27018
