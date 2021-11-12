all: docs test_client server

# test-bin: test_client.cc easylogger.h easylogger-impl.h Makefile
# 	$(CXX) -g -o test-bin test_client.cc
# 	# ./test-bin

server: server.cc easylogger.h easylogger-impl.h  Makefile 
	$(CXX) server.cc -g -o server.exe
	chmod +x server.exe
	mv server.exe ..

test_client: test_client.cc easylogger.h easylogger-impl.h Makefile
	$(CXX) test_client.cc -g -o test_client.exe 	
	chmod +x test_client.exe
	mv test_client.exe ..

docs:
	doxygen

clean:
	rm -f *-bin
	rm -f *.exe	
	rm -rf input
	rm -rf output