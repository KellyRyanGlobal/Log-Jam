# Log Jam
# Introduction

This is a fork version of https://github.com/elanthis/easylogger. Modifications are made for simple testing purpose of running basic testing functionality within a jenkins environment.

Some of the modifications will not have to do with the logger itself, but test out things such as:
- System calls
- User input
- Opening applications

The logging will help provide information based on these events occurring. 

Testing that will be performed include
- **Smoke testing**- verifying the application was built as intended and the artifact is present
- **Functional testing**- verifying the application is working as intended based predefined circumstance
   - Positive and negative- Testing based on assertions of input 
- **Acceptance testing**- Verifying the output of logic is what is expected based on requirements
  - Verifying logic is correct

# About Logger

All classes reside in the `easylogger` namespace.

There is only one primary class that the user must be aware of, which is
the `Logger` class.  Objects of `Logger` are used along with a set of
macros to perform all logging.

To create a new log, create a  `Logger` object with a log name.

	easylogger::Logger NETWORK("NETWORK");

You can then write message to the `NETWORK` log using one of `LOG_TRACE`,
`LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, `LOG_ERROR`, or `LOG_FATAL`.

	LOG_TRACE(NETWORK, "Trace message to NETWORK log");
	LOG_WARNING(NETWORK, "Warning message to NETWORK log");

You can inherit logs from each other, creating a hierarchy.  Pass the parent
log as the second argument to the `Logger` constructor.

	easylogger::Logger CONNECT("NETWORK.CONNECT", NETWORK);

	LOG_INFO(CONNECT, "Connection received");

Inherited logs will cause all messages to be sent to the parent log.  This
is useful for classification of logs, as well as allowing some logs to be
directed to alternative output streams.

A `Logger` instance with no parent by default will log all messages to
`std::cerr`.  `Logger` instances with a parent have no associated stream by
default.

You can set the associated stream on a `Logger` instance using the
`Logger::Stream(std::ostream&)` method.  Any `std::ostream` derivative is
acceptable.

	std::ofstream network_log("network.log");
	NETWORK.Stream(network_log);

	std::ofstream connect_log("connect.log");
	CONNECT.Stream(connect_log);

With the above example, all network log messages will be written to
`network.log`.  Additionally, any logs on the `CONNECT` log will also be
written to `connect.log`.

Each log has a minimum log level.  Only messages of that level or higher
will be processed.  The default log level is INFO.  This can be changed
by using the `Logger::Level(LogLevel)` method, giving it one of
`LEVEL_TRACE`, `LEVEL_DEBUG`, `LEVEL_INFO`, `LEVEL_WARNING`, `LEVEL_ERROR`,
or `LEVEL_FATAL`.

	NETWORK.Level(easylogger::LEVEL_DEBUG);

Logging any message with the level FATAL will cause the application to
abort immediately via `std::abort()`.

Finally, there is a set of assertion macros that can be used for checking
invariants.

	easylogger::Logger MAIN("MAIN");

	EASY_ASSERT(MAIN, param > 42, "param must be more than 42");
	ASSERT_EQ(MAIN, left, right, "left must be equal to right");
	ASSERT_NE(MAIN, left, right, "left must not equal right");
	ASSERT_TRUE(MAIN, param, "param must be true");
	ASSERT_FALSE(MAIN, param, "param must be false");


# Automated Testing Environment
The testing environment will use jenkins running in docker to deploy containers with logger running
The following instructions will guide the user/tester to build and deploy the application within a local environment or a container.

# Prerequisite

The prerequisites below require installation before deploying the application. For deployments Virtual box will run using CentOS that is deployed on a Windows 10 workstation.

##  Windows

The following needs to be pre installed on a Windows 10 workstation  before proceeding. How to install the services are outside of the scope of this README.

- VirtualBox (hypervisor)- See VirtualBox Prerequisite
- Linux (CentOS prefered) OS VM image
- Docker Desktop - Used for viewing containerization

### VirtualBox 
The following needs to be installed on the Linux OS VM that is installed on VirtualBox. The installation of how to install the OS is outside of the scope of this readme.
- doxygen - for building logger
- Install g++ and gcc
- Install Make
- Create a docker login at https://hub.docker.com
- **sshkeygen**- for ssh connection
- **docker**- for building containers
- **docker-compose**- for creating layered containers


## Jenkins Docker image setup
Reference https://www.jenkins.io/doc/book/installing/docker/ for more information

## Install Jenkins Docker container
The following will install a Jenkins docker container with persistent volume allowing for saved states based on the container being brought up and down.

It is recommended to deploy the jenkins docker container within a Linux VM. For this deployment we will deploy jenkins within a docker container within a Centos VM.


### Installation
1. Add docker credentials for dockerhub to the environment by running the following
   ```
   export DOCKER_USER="<enter docker hub username>"
   export DOCKER_PASS="<enter docker hub password>"
   ```

2. Run the following in the terminal
    1. **Localhost**: Open a terminal and SSH to the VM using the following command
        1. `ssh <username>@<IP of VM>`
    2. **VM TERMINAL**: Run the following command
        1. `git clone git@github.com:KellyRyanGlobal/testing-app.git`
    3. **VM TERMINAL**: Navigate to the cloned repo for `testing-app`
    4. **VM TERMINAL**: `cd jenkins`
    5. **VM TERMINAL**:`jenkins/scripts/jenkins.sh`
    6. Wait about 2 minutes and jenkins should be up
    7. **Local host**: Open a browser
    8. **Browser**: navigate to http://< VM IP >:8080
       1. If it is the first time, run the following command in a terminal
           1. **Terminal**:`docker exec -it jenkins-docker  cat /var/jenkins_home/secrets/initialAdminPassword`
       2. **Webpage**: Enter the `initialadminPassword` into the browser
    9.  **Browser**: Install Suggested plugins
    
### Jenkins Plugins
Install plugins within the jenkins dashboard after installation. These help with managment of testing results
- Install the recommended plugins
- Docker
- Ansi color

## Build logger in localhost

To build logger locally in your workstation, run the following command
1. `make -C src`
1. Verify the test-bin is created in the `src` folder
1. Verify the build does not fail
    

## Build logger in local docker container (ubuntu)

To build logger in ubuntu in a docker container run the following
1. **Terminal1**:`docker build -t logger_ubuntu:latest -f docker/ubuntu/Dockerfile`
1. **Terminal1**:`docker run --name logger logger_ubuntu:latest`
1. **Terminal2**: Open a new terminal and enter `docker exec -it logger sh`
1. **Container**: In the container run `/.build.sh`
1. **Container**: `cat input/results.txt`
1. Verify the contents of the txt file

## Build logger in local docker container (alpine)

To build logger in alpine in a docker container run the following
1. **Terminal1**: `docker build -t logger_alpine:latest -f docker/alpine/Dockerfile .`
1. **Terminal1**: `docker run --name logger_alpine logger_alpine:latest`
1. **Terminal2**: Open a new terminal and enter `docker exec -it logger_alpine sh`
1. **Container**: In the container run `/.build.sh`
1. **Container**: `cat input/results.txt`
1. Verify the contents of the txt file

## Build Logger using docker-compose

### Prerequisite
- Clean root folder of and .exe as well as input and output folders

### Deployment
The following instructions will build logger in 2 containers using docker-compose
1. **Local Host**: Open a windows terminal and naivigate to the root directory of the project
1. **Terminal1**: Run the following command `docker-compose -f docker/<linux os>/docker-compose.client.yml -f docker/<linux os>/docker-compose.server.yml --env-file .env up -d`
1. **Terminal1**: Verify output 
```
Creating network "<linux_OS>_logger_net" with the default driver
Creating server ... done
Creating client ... done
```
1. **Local Workstation**: Open Docker Desktop
1. **Docker Desktop**: Click the <linux OS> being run and then click server from the dropdown
1. **Docker Desktop**: Verify the output shows the file saved
1.  **Local Workstation**: Verify the following are now in the root directory
    1.  `test_client.exe`
    1.  `server.exe`
    1.  `input/results.txt`
    1.  `output/server.out`
1. **Terminal1**: `docker-compose -f docker/<linux os>/docker-compose.client.yml -f docker/<linux os>/docker-compose.server.yml down`

## Server-Client Deployment

Will build a server and client application

### Prerequisite
- Build logger in localhost must be run

### Build Client and Server
1. **Terminal1**: Run `./server.exe `
1. **Localhost**: Open a new Terminal
1. **Terminal2**: Run `./test_client.exe 127.0.0.1`
1. **Terminal2**: Verify output by running `cat output/server.out`



