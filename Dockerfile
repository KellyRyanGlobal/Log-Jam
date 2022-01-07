FROM ubuntu:14.04
RUN apt-get update 
RUN apt-get install python3 -y
RUN apt-get install python-pip -y
RUN pip install python-pypi-mirror
WORKDIR /app
COPY pythonbuild.sh /app
COPY requirements.txt /app
