# GalDanielAdvProg1

Compilation can be done either using shell command (while at the project path ./GalDanielAdvProg1/):
linux> make

This will create two run files: client.out, server.out.

To compile only one of the files, use instead:
linux> make client.out
or
linux> make server.out

Run server using command:
linux> ./server.out [file] [port]
And client using:
linux > ./client.out [ip] [port]

Optionally, could also clean up compiled files using command:
linux> make clean

This program implements algorithm KNN (K Nearest Neighbours) on a database of classified vegetation, between a local client and a different server.
The database contains examples for flower data, and the program calculates which flower classification most resembles a given input flower vector based on it.
'dataInput' contains implementation for reading from the database. During the course of the process's runtime, only the given database is read and loaded once.
It then parses the data in the set into a list of vectors using implementation in 'stringToVec'.
For each new flower vector, the program generates a list of distances using 5 distance classes inheriting from a single interface.
It then uses an median of medians select algorithm to find the kth closest vector and partition based on it, and calculates the most common classification within the kth closest vectors to the input vector using a map data structure.

The server implements a multi-threaded TCP connection, allowing for multiple users to connect to it. And use its functionality separately at once.
The server also implements a Command design pattern, to help with modularity within the server.

Usage:
On the client side, upon connecting to server, the user will be presented with a menu for commands the server is capable of. They should input a number 1 through 5 to select one of them, or 8 to close connection with the server.

The user should first use option 1 to upload classified and unclassified vector information (.csv).
The user may then specify the distance function and k value they would like to classify by, using option 2 on the menu. If no input was chosen, the KNN algorithm will use the default arguments of k=5 and Euclidian distance.
Then, option 3 will ask the server to classify your unclassified file, using the classified information received.
Option 4 will then print those ordered results to the user, and those classifications can be downloaded to a file on a separate result using option 5.
Note that option 5 can be executed even if option 4 was not used yet. 

Optimization:
Only the necessary database is loaded to the server program, and only loaded once at initialization.
for n = the number of vectors in the given database, each with dimension m. For each new input, the program runs at time complexity of  O(n*m).
O(m) for calculating each vector's distance from the input, for a total of n times, and then O(n) for the select algorithm, and anther
O(n) for finding the most common classification.

Notes:
Since the Euclidian and the Manhattan distance are subsets of the Minkowski distance, they also use its implementation.
The default input for the Minkowski distance being simply the Euclidian distance (when P = 2. It's defined as a const).

*distances functions:
AUC: Euclidian
MAN: Manhatten
CHB: Chebyshev
CAN: Canberra
MIN: Minkowski
