#include "easylogger.h"
#include "template_functions.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <bits/stdc++.h>

#define BIT_SIZE 8;
using namespace std;

static easylogger::Logger TEST("TEST");
static easylogger::Logger NETWORK("NETWORK");
static easylogger::Logger HOST("HOST");
static easylogger::Logger TRACER("TRACE");
static easylogger::Logger SUB("SUB", TEST);

static void test1() {
	LOG_TRACE(TRACER, test1);

	LOG_INFO(TEST, "Hi!" << 42);
}

static void test2() {

	LOG_DEBUG(TEST, "don't show me");
	LOG_DEBUG(NETWORK, "network");
}

static void add_test() {
	// add ints
	int a = 10, b = 20, result;
    result = sum <int> (a, b);
    LOG_INFO(TEST,  a << " + " << b << " = " << result);
}

/** function to calculate range of data type */
int printSignedRange(int count)
{
    int min = pow(2, count - 1);
    int max = pow(2, count - 1) - 1;
    // printf("\n %d to %d \n", min * (-1), max);
	return max;
}

/**
 * Function to trigger an overflow of a short int datatype.
 * */
static void overflow_test() {
	
	short int size = sizeof(short int)*BIT_SIZE; // sizeof returns # bytes
	short int max = printSignedRange(size);
	LOG_INFO(TEST,"max size of short int: " <<  max);
	
	// signed short int: -32768 to 32767
	// overflow 
	max++;
	LOG_INFO(TEST,"expected result: 32768, actual result due to overflow: " << max);
}


void host_test(int hostname) { //This function returns host name for

   if (hostname == -1) {
      LOG_ERROR(SUB, "gethostname");
      exit(1);
   }
}

void IP_formatter(char *IPbuffer) { //convert IP string to dotted decimal

   if (NULL == IPbuffer) {
      LOG_ERROR(SUB, "inet_ntoa");
      exit(1);
   }
}

int main(int argc , char *argv[]) {

    char host[256];
    char *IP = argv[1];
	ofstream file;
    int hostname;
	

	SUB.Level(easylogger::LEVEL_WARNING);
	NETWORK.Format("[%F:%C %P] %N: %S");
	NETWORK.Level(easylogger::LEVEL_TRACE);
	TRACER.Level(easylogger::LEVEL_TRACE);

	//create the file in case it does not exist
	file.open("input//results.txt");
	file.close();

	//logging output to file for server sending
	freopen ("input//results.txt", "w", stdout);

	test1();
	test2();
	add_test();
	overflow_test();
	LOG_INFO(TEST,"Program Name Is: " << argv[0]);
	LOG_INFO(TEST,"Server IP Is: " << IP);
	hostname = gethostname(host, sizeof(host));
	host[sizeof(host)-1] = '\0';
	host_test(hostname);
	LOG_INFO(HOST, "hostname: " << host);
	sleep(5);
	try
	{
		if(argv[1] != NULL)
		{
			Client_socket C(argv);
			LOG_INFO(HOST, "waiting");
			C.send_file();	
			fclose(stdout);	
		}
		else
		{
			LOG_ERROR(HOST, "need to provide a IP");
			return 1;
		}

	}
	catch(const std::exception& e)
	{
		LOG_ERROR(HOST, e.what() << '\n');
		fclose(stdout);	
	}
	 
	
	
	
	//clear output for sending
	//std output on console
	// test1();
	// test2();
	// add_test();
	// overflow_test();

	// hostname = gethostname(host, sizeof(host));
	// host_test(hostname);
	// strcat(output, host);
	// LOG_INFO(HOST, output);
    //C.send_file();
	//LOG_FATAL(TEST, "dead");
	//LOG_ERROR(TEST, "won't see me");
	return 0;
}
