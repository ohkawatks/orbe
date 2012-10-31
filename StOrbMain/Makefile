
TARGET=pc
#TARGET=armadillo
#TARGET=xilinxEDK

default: server client


testAll: unitTest regressionTest clientServerTest

unitTest:
	cd test/unitTest; make test

regressionTest: idlc
	cd test/regressionTest; make test

clientServerTest: idlc
	cd test/clientServerTest; make test

server: idlc
	cd arch/$(TARGET); make server

client: idlc
	cd arch/$(TARGET); make client

idlc:
	cd idlc; make

clean:
	@rm -rf *~
	cd test/unitTest; make clean
	cd test/regressionTest; make clean

