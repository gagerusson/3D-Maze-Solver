#include "Maze.h"

using namespace std;

int main(int argc, char *argv[]) {
    int rows, cols, levels = 0;
    ifstream file(argv[1]);
    if (file.is_open()) {
        string line;
        getline(file, line);
        stringstream ss(line);
        ss >> rows;
        ss >> cols;
        ss >> levels; //pull out first 3 numbers for dimensions of maze
    }

    Maze maze(rows, cols, levels);
    maze.importMaze(argv[1]);
    cout << maze.toString() << endl;
    maze.solutionFile(argv[2]);
}
