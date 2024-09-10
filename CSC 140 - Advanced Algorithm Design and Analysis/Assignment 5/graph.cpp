// © 2024 IllyaGY
// Licensed under the MIT License. See the LICENSE file for details.

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include "priorityQueue.cpp"


void printSolution(int *answers, int size, int maxDistance){
    std::ostringstream vertices;
    std::ostringstream distances;

    vertices << "Vertex     ";
    distances << "\nDistance   ";
    for (int i = 0; i < size; i++){

        if (answers[i] < maxDistance) {
            vertices << std::left << std::setw(10) << i;
            distances << std::left << std::setw(10) << answers[i];
        } else {
            vertices << std::left << std::setw(12) << i;
            distances << std::left << std::setw(12) << "infinity";
        }
    }
    std::ofstream outFile("output.txt");
    if (outFile.is_open()) {
        // Write data to the file
        outFile << vertices.str() + distances.str();

        // Close the file
        outFile.close();
        std::cout << "Data has been written to the file successfully." << std::endl;
    } else {
        // Error message if the file could not be opened
        std::cout << "Error opening file!" << std::endl;
    }
}


/*Dijksttra*/
void DijkstraAlg(int size, int edgeNum, int start, std::vector<int*> edges, int maxDistance) {
    int *answers = nullptr;
    answers = new int[size];
    for (int i = 0; i < size; i++)
        answers[i] = maxDistance;
    answers[0] = 0;
    PriorityQueue queue = PriorityQueue(size, maxDistance);
    queue.update(0, 0);
    queue.printN();
    while (queue.getRoot().nodeValue < maxDistance) {
        Node u = queue.extractMin();
        for (int i = 0; i < edgeNum;) {
            if (u.nodeNum == edges.at(i)[0]) {
                if (answers[edges.at(i)[1]] > u.nodeValue + edges.at(i)[2]) {
                    answers[edges.at(i)[1]] = u.nodeValue + edges.at(i)[2];
                    queue.update(edges.at(i)[1], u.nodeValue + edges.at(i)[2]);
                    queue.printN();
                }
                delete[] edges.at(i);
                edges.erase(edges.begin() + i);
                edgeNum--;
            } else i++;

        }

    }
    printSolution(answers, size, maxDistance);
    delete [] answers;

}



int main() {

    std::ifstream file("input4.txt");               //Change the file name to the one you are using
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }
    int maxDistance = 0;
    std::string line;
    std::vector<int*> edges;
    int *dim = nullptr;
    dim = new int[2];
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> numbers;
        int num;
        while (iss >> num) {
            numbers.push_back(num);
        }
        if (numbers.size() == 3) {
            maxDistance+=numbers[2];
            edges.push_back(new int[]{numbers[0], numbers[1], numbers[2]});
        } else if (numbers.size() == 2) {
            dim[0] = numbers[0];
            dim[1] = numbers[1];
        }
    }

       DijkstraAlg(dim[0], dim[1], 0,edges,maxDistance+1);
       delete [] dim;









       return 0;
   }
