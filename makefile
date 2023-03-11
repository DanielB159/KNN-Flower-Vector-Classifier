all: client.out server.out

client.out: clientMain.cpp stringToVec.h stringToVec.cpp networkFunctions.cpp networkFunctions.h
	g++ -pthread -std=c++11 -o client.out clientMain.cpp stringToVec.h stringToVec.cpp networkFunctions.cpp networkFunctions.h

server.out: serverMain.cpp stringToVec.cpp stringToVec.h slct.cpp slct.h dataInput.cpp dataInput.h typeDistances.cpp typeDistances.h vectorType.cpp vectorType.h DefaultIO.h Command.h CLI.h CLI.cpp
	g++ -pthread -std=c++11 -o server.out serverMain.cpp stringToVec.cpp stringToVec.h slct.cpp slct.h dataInput.cpp dataInput.h typeDistances.cpp typeDistances.h vectorType.cpp vectorType.h DefaultIO.h Command.h CLI.h CLI.cpp

clean:
	rm -f *.o *.out *.gch
