#include "hypercube.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// Reading the file
void HyperCube::read(istream& input) {
    string numPermitted;
    getline(input, numPermitted);

    string line;
    while (getline(input, line)) {
        permitted.insert(line);
    }
}

SearchResult HyperCube::search(const string& start) const {
    // Initialize ending node of all 1's.
    string end(start.size(), '1');

    // Instantiate priority queue to use for A* algorithm.
    priority_queue<Node, vector<Node>, NodePriority> unvisited;

    // Initialize the number of expansions used by the algorithm and path.
    size_t expansions = 0;
    vector<string> path;

    // Add any variables needed for A* algorithm
    map<string, Node> visited;

    // Implement A* algorithm. Remember to add node label strings on the path
    // to the results vector. Order of the strings in the vector matters. Make
    // sure to return a SearchResult object.

    // Construct the start node
    Node startNode(start, start, 0);
    // Push the start node on the open list
    unvisited.push(startNode);

    // While the opne list is not empty
    while (unvisited.empty() == false) {

        // Get the min node
        Node s = unvisited.top();
        unvisited.pop();

        // If we saw this node skip
        if (visited.find(s.word) != visited.end()) {
            continue;
        }

        // If we didn't see it then increment expansions and push to path
        if (s.h != 0) {
            expansions++;
        }

        // Insert into visited
        visited.insert(make_pair(s.word, s));

        if (s.h == 0) {

            // Pushing back optimal route
            while (s.word != start) {
                path.push_back(s.word);
                s = visited.at(s.pred);
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return SearchResult(path, expansions);
        } else {
            for (int i=0; i<s.word.size(); i++) {
                string temp(s.word);
                if (temp[i] == '0') {
                    temp[i] = '1';
                } else {
                    temp[i] = '0';
                }
                // If v on visited, skip
                if (visited.find(temp) != visited.end()) {
                    continue;
                }
                // If v not in PQ, push it in PQ
                if (permitted.find(temp) != permitted.end()) {
                    Node neighbor(temp, s.word, s.g+1);
                    unvisited.push(neighbor);
                }
            }
        }
    }

    return SearchResult(false, expansions);
}
