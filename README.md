# KNN-Flower-Vector-Classifier
## Introduction
This is a console application for classifying input vectors using the **KNN classification algorithm**.

#### What is the KNN classification algorithm?
KNN (K - nearest neighbor) is an algorithm for classifying an **unclassified** input vector of x elements based on a given database of at least K other **classified** vectors of the same length. The pseudocode for the algorithm:  

```
KNN(Vector v, int k):
- find K closest vectors in database by some distance metric.
- define type to be the most frequent type of all k closest vectors.
- return type.
```

## Design
The program is split to two processes. **the server proccess** and **the client proccess**. The client handles the requeses from the user.
With all server-client communication implemented using *TCP*.
The possible requests are:
1. uploading the database of classified and of unclassified vectors to the server.
2. choosing the K number and the distance metric.
3. Classifying the vectors.
4. Showing the classifications to the user in the console.
5. Downloading the classified vectors to the user's computer.

- To handle multiple clients, the server uses *multithreading* in order to handle all of the clients at once, with at most 40 clients in the queue.
- The client process also uses *multithreading* to let the user continue the communication with the server while the classified vectors are downloaded in the background in another thread.
- The 5 possible user requests are implemented using a Command Line Interface (CLI) Design Pattern.
- The possible Distances are:  
1. "AUC" - Euclidian (more info at https://en.wikipedia.org/wiki/Euclidean_distance)  
2. "MAN" - Manhatten  (more info at https://en.wikipedia.org/wiki/Taxicab_geometry)  
3. "CHB" - Chebyshev  (more info at https://en.wikipedia.org/wiki/Chebyshev_distance)  
4. "CAN" - Canberra  (more info at https://en.wikipedia.org/wiki/Canberra_distance)  
5. "MIN" - Minkowski  (more info at https://en.wikipedia.org/wiki/Minkowski_distance)  
**implementation Note** - Since the Euclidian and the Manhattan distance are subsets of the Minkowski distance, they also use its implementation.
The default input for the Minkowski distance being simply the Euclidian distance (when P = 2. It's defined as a const).

### Time Complexity
for n = the number of vectors in the given database, each with dimension m. For each new input, the program runs at time complexity of ***O(n*m)***.
*O(m)* for calculating each vector's distance from the input, for a total of n times, and then *O(n)* for the select algorithm, and anther
*O(k)* for finding the most common classification.

## Key Learnings
By developing this project, i have gained experience in the following areas:  
- *KNN classification algorithm*.
- *TCP* server and client implementation in C++.
- *multithreading* programming.
- Implementing a *Command Line Interface* (CLI) design pattern.

## Compilation and Example run
Compilation can be done either using terminal command:  `linux> make`  
This will create two run files: client.out, server.out.  
To compile only one of the files instead, use:  `linux> make client.out`  or  `linux> make server.out`  

Run server using command:  `linux> ./server.out [file-path] [port-number]`  
And client using:  `linux > ./client.out [server-ip] [server-port-number]`

Optionally, could also clean up compiled files using command: `linux> make clean`

Example run:
server:
```
./server.out 60000

```
client:
```
./client.out 127.0.0.1 60000
Welcome to the KNN Classifier Server. Please choose an option:
1. upload an unclassified csv data file
2. algorithm settings
3. classify data
4. display results
5. download results
8. exit

1

Please upload your local train csv file.
datasets/iris/iris_classified.csv
Please upload your local test csv file.
datasets/iris/iris_Unclassified.csv

Welcome to the KNN Classifier Server. Please choose an option:
1. upload an unclassified csv data file
2. algorithm settings
3. classify data
4. display results
5. download results
8. exit

2

The current KNN parameters are: K = 5, distance metric = AUC
19 CHB

Welcome to the KNN Classifier Server. Please choose an option:
1. upload an unclassified csv data file
2. algorithm settings
3. classify data
4. display results
5. download results
8. exit

3

classifying data complete

Welcome to the KNN Classifier Server. Please choose an option:
1. upload an unclassified csv data file
2. algorithm settings
3. classify data
4. display results
5. download results
8. exit

4

1 Iris-setosa
2 Iris-virginica
3 Iris-virginica
4 Iris-versicolor
5 Iris-versicolor
6 Iris-setosa
7 Iris-virginica
8 Iris-setosa
9 Iris-virginica
10 Iris-setosa
11 Iris-versicolor
12 Iris-virginica
13 Iris-virginica
14 Iris-versicolor
15 Iris-setosa
Done.

Welcome to the KNN Classifier Server. Please choose an option:
1. upload an unclassified csv data file
2. algorithm settings
3. classify data
4. display results
5. download results
8. exit

8

```
