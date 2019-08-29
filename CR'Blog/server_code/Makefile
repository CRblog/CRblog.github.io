FLAG=-std=c++11 -L/usr/lib64/mysql -lmysqlclient -ljsoncpp -lpthread 

all:blog_server db_test

blog_server:blog_server.cc
	g++ blog_server.cc -o blog_server $(FLAG)

db_test:db_test.cc 
	g++ db_test.cc -o db_test $(FLAG)

.PHONY:clean
clean:
	rm blog_server db_test
