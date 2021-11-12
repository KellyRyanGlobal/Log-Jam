# the following will build a jenkins image in docker using the jenkins jdk image from docker hub
FROM openjdk
USER root
RUN apt-get update && apt-get install -y apt-transport-https \
       ca-certificates curl gnupg2 \
       software-properties-common
RUN curl -fsSL https://download.docker.com/linux/debian/gpg | apt-key add -
RUN apt-key fingerprint 0EBFCD88
RUN add-apt-repository \
       "deb [arch=amd64] https://download.docker.com/linux/debian \
       $(lsb_release -cs) stable" \
RUN java -jar agent.jar -jnlpUrl http://192.168.1.166:8080/computer/VM_node/jenkins-agent.jnlp -secret a1fb2e8d80e89c7be06f5c78308c28f1d9ac95a68428de4961e97cf6a06e1d07 -workDir "/home/kryan/sample_cpp"
RUN apt-get update && apt-get install -y docker-ce-cli
RUN apt-get install maven
USER jenkins
RUN jenkins-plugin-cli --plugins "blueocean:1.24.7 docker-workflow:1.26"`