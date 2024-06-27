FROM ubuntu:20.04

WORKDIR /usr/app

RUN apt-get update && apt-get install -y openssl openjdk-11-jdk

RUN mkdir build-certs certs

COPY certificate.sh entrypoint.sh ./

RUN chmod +x certificate.sh entrypoint.sh

RUN ./certificate.sh

ENTRYPOINT [ "./entrypoint.sh" ]