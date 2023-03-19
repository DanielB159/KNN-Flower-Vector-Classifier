all: client.out server.out

client.out: clientMain.o stringToVec.o networkFunctions.o
	g++ -pthread -std=c++11 -o client.out clientMain.o stringToVec.o networkFunctions.o

clientMain.o: clientMain.cpp stringToVec.h networkFunctions.h
	g++ -std=c++11 -c -o clientMain.o clientMain.cpp

stringToVec.o: stringToVec.cpp stringToVec.h
	g++ -std=c++11 -c -o stringToVec.o stringToVec.cpp

networkFunctions.o: networkFunctions.cpp networkFunctions.h
	g++ -std=c++11 -c -o networkFunctions.o networkFunctions.cpp

server.out: serverMain.o stringToVec.o slct.o dataInput.o typeDistances.o vectorType.o Command.o CLI.o UploadFile.o AlgorithmSettings.o ClassifyData.o DisplayResults.o DownloadResults.o SocketIO.o
	g++ -pthread -std=c++11 -o server.out serverMain.o stringToVec.o slct.o dataInput.o typeDistances.o vectorType.o Command.o CLI.o UploadFile.o AlgorithmSettings.o ClassifyData.o DownloadResults.o DisplayResults.o SocketIO.o

serverMain.o: serverMain.cpp stringToVec.h typeDistances.h dataInput.h vectorType.h slct.h DefaultIO.h SocketIO.h Command.h CLI.h
	g++ -std=c++11 -c -o serverMain.o serverMain.cpp

slct.o: slct.cpp slct.h
	g++ -std=c++11 -c -o slct.o slct.cpp

dataInput.o: dataInput.cpp dataInput.h
	g++ -std=c++11 -c -o dataInput.o dataInput.cpp

typeDistances.o: typeDistances.cpp typeDistances.h
	g++ -std=c++11 -c -o typeDistances.o typeDistances.cpp

vectorType.o: vectorType.cpp vectorType.h dataInput.h
	g++ -std=c++11 -c -o vectorType.o vectorType.cpp

SocketIO.o: SocketIO.cpp SocketIO.h DefaultIO.h
	g++ -std=c++11 -c -o SocketIO.o SocketIO.cpp

Command.o: Command.cpp Command.h DefaultIO.h SocketIO.h stringToVec.h typeDistances.h slct.h vectorType.h
	g++ -std=c++11 -c -o Command.o Command.cpp

CLI.o: CLI.cpp CLI.h typeDistances.h dataInput.h DefaultIO.h Command.h UploadFile.h AlgorithmSettings.h ClassifyData.h DisplayResults.h DownloadResults.h
	g++ -std=c++11 -c -o CLI.o CLI.cpp

UploadFile.o: UploadFile.cpp UploadFile.h Command.h
	g++ -std=c++11 -c -o UploadFile.o UploadFile.cpp

AlgorithmSettings.o: AlgorithmSettings.cpp AlgorithmSettings.h Command.h
	g++ -std=c++11 -c -o AlgorithmSettings.o AlgorithmSettings.cpp

ClassifyData.o: ClassifyData.cpp  ClassifyData.h Command.h
	g++ -std=c++11 -c -o  ClassifyData.o  ClassifyData.cpp

DisplayResults.o: DisplayResults.cpp DisplayResults.h Command.h
	g++ -std=c++11 -c -o DisplayResults.o DisplayResults.cpp

DownloadResults.o: DownloadResults.cpp DownloadResults.h Command.h
	g++ -std=c++11 -c -o DownloadResults.o DownloadResults.cpp

clean:
	rm -f *.o *.out *.gch