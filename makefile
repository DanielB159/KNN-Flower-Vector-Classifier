all: client.out server.out

client.out: clientMain.o stringToVec.o networkFunctions.o
	g++ -pthread -std=c++11 -o client.out clientMain.o stringToVec.o networkFunctions.o

clientMain.o: clientMain.cpp VectorRelated/stringToVec.h Network/networkFunctions.h
	g++ -std=c++11 -c -o clientMain.o clientMain.cpp

stringToVec.o: VectorRelated/stringToVec.cpp VectorRelated/stringToVec.h
	g++ -std=c++11 -c -o stringToVec.o VectorRelated/stringToVec.cpp

networkFunctions.o: Network/networkFunctions.cpp Network/networkFunctions.h
	g++ -std=c++11 -c -o networkFunctions.o Network/networkFunctions.cpp

server.out: serverMain.o stringToVec.o slct.o typeDistances.o vectorType.o Command.o CLI.o UploadFile.o AlgorithmSettings.o ClassifyData.o DisplayResults.o DownloadResults.o SocketIO.o
	g++ -pthread -std=c++11 -o server.out serverMain.o stringToVec.o slct.o typeDistances.o vectorType.o Command.o CLI.o UploadFile.o AlgorithmSettings.o ClassifyData.o DownloadResults.o DisplayResults.o SocketIO.o
	rm -f *.o

serverMain.o: serverMain.cpp VectorRelated/stringToVec.h VectorRelated/typeDistances.h VectorRelated/vectorFeatures.h VectorRelated/vectorType.h VectorRelated/slct.h Network/DefaultIO.h Network/SocketIO.h Commands/Command.h Network/CLI.h
	g++ -std=c++11 -c -o serverMain.o serverMain.cpp

slct.o: VectorRelated/slct.cpp VectorRelated/slct.h
	g++ -std=c++11 -c -o slct.o VectorRelated/slct.cpp

typeDistances.o: VectorRelated/typeDistances.cpp VectorRelated/typeDistances.h
	g++ -std=c++11 -c -o typeDistances.o VectorRelated/typeDistances.cpp

vectorType.o: VectorRelated/vectorType.cpp VectorRelated/vectorType.h VectorRelated/vectorFeatures.h
	g++ -std=c++11 -c -o vectorType.o VectorRelated/vectorType.cpp

SocketIO.o: Network/SocketIO.cpp Network/SocketIO.h Network/DefaultIO.h
	g++ -std=c++11 -c -o SocketIO.o Network/SocketIO.cpp

Command.o: Commands/Command.cpp Commands/Command.h Network/DefaultIO.h Network/SocketIO.h VectorRelated/stringToVec.h VectorRelated/typeDistances.h VectorRelated/slct.h VectorRelated/vectorType.h
	g++ -std=c++11 -c -o Command.o Commands/Command.cpp

CLI.o: Network/CLI.cpp Network/CLI.h VectorRelated/typeDistances.h VectorRelated/vectorFeatures.h Network/DefaultIO.h Commands/Command.h Commands/UploadFile.h Commands/AlgorithmSettings.h Commands/ClassifyData.h Commands/DisplayResults.h Commands/DownloadResults.h
	g++ -std=c++11 -c -o CLI.o Network/CLI.cpp

UploadFile.o: Commands/UploadFile.cpp Commands/UploadFile.h Commands/Command.h
	g++ -std=c++11 -c -o UploadFile.o Commands/UploadFile.cpp

AlgorithmSettings.o: Commands/AlgorithmSettings.cpp Commands/AlgorithmSettings.h Commands/Command.h
	g++ -std=c++11 -c -o AlgorithmSettings.o Commands/AlgorithmSettings.cpp

ClassifyData.o: Commands/ClassifyData.cpp  Commands/ClassifyData.h Commands/Command.h
	g++ -std=c++11 -c -o  ClassifyData.o  Commands/ClassifyData.cpp

DisplayResults.o: Commands/DisplayResults.cpp Commands/DisplayResults.h Commands/Command.h
	g++ -std=c++11 -c -o DisplayResults.o Commands/DisplayResults.cpp

DownloadResults.o: Commands/DownloadResults.cpp Commands/DownloadResults.h Commands/Command.h
	g++ -std=c++11 -c -o DownloadResults.o Commands/DownloadResults.cpp

clean:
	rm -f *.o *.out *.gch