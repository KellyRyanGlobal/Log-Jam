
// Server side C/C++ program to demonstrate Socket programming

#include "easylogger.h"
#include "template_functions.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>

#define PORT 8050

static easylogger::Logger SERVER("SERVER");
static easylogger::Logger SUB("SUB");

class Server_socket{

    ofstream myfile;
    int server_fd, valread, opt = 1, new_socket, address_length;
    struct sockaddr_in address;
    int addrlen;
    char buffer[1024] = {0};
    // char hello[] = "Hello from server";
    
    public:
        Server_socket () {
            create_socket();
                
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons( PORT );            
            address_length = sizeof(address);

            bind_socket();
            set_listen_set();
            accept_connection();

            
        }
        // Creating socket file descriptor
        void create_socket() {
            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
            {
                LOG_ERROR(SUB, "socket failed\n");
                exit(EXIT_FAILURE);
            }
            LOG_INFO(SERVER," Socket Created\n");
        }
        // Create the bind for server fd object
        void bind_socket() {
            if (bind(server_fd, (struct sockaddr *)&address, 
                                        sizeof(address))<0)
            {
                LOG_ERROR(SUB,  "Bind failed\n");
                exit(EXIT_FAILURE);
            }
            LOG_INFO(SERVER, " Bind Successful.\n");
        }
        // Listen for incoming request
        void set_listen_set() {
            if (listen(server_fd, 3) < 0)
            {
                LOG_ERROR(SUB,  "listen\n");
                exit(EXIT_FAILURE);
            }
            LOG_INFO(SERVER, " Socket Listening\n");
        }
        //accept the connection based on ....nothing
        void accept_connection() {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                            (socklen_t*)&addrlen))<0)
            {
                LOG_ERROR(SUB, "acception of Connection failed");
                exit(EXIT_FAILURE);
            }
            LOG_INFO(SERVER, "Connected to client\n");
        }

        void receive_file(){
            char buffer[1024] = {};
            int valread = read( new_socket , buffer, 1024);
            LOG_INFO(SERVER,"Data received "<< valread <<" bytes\n");
            LOG_INFO(SERVER,"Saving data to file.\n" << buffer << "\n" );
            myfile.open(".//output//server.out", ios::out | ios::trunc | ios::binary);
            if(myfile.is_open()) {
                LOG_INFO(SERVER, "File Creted.\n");
                myfile << buffer;
                LOG_INFO(SERVER," File Saved.\n");
                myfile.close();
            }
            else{
                LOG_ERROR(SUB, " File creation failed, Exititng.\n");
                exit(EXIT_FAILURE);
            }            
            
        }
};

int main(){
    Server_socket S;
    SUB.Level(easylogger::LEVEL_WARNING);
	SERVER.Level(easylogger::LEVEL_TRACE);
    S.receive_file();
    return 0;
}