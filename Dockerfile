FROM alpine:latest AS Build
ENTRYPOINT ["tail", "-f", "/dev/null"]
RUN apk update && apk add doxygen g++ make bash
COPY . /app
WORKDIR /app
#CMD /bin/bash -c ' ./build.sh; /bin/bash'