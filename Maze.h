#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

const int WALL = 0;
const int POSSIBLE_PATH = 1;
const int VISITED = 2;
const int PATH = 3;

class Maze {
private:
    vector<string> solved;
    int ROWS;
    int COLS;
    int LEVELS;
    int *** maze;
public:
    Maze(int rows, int cols, int levels) {
        ROWS = rows;
        COLS = cols;
        LEVELS = levels;
        allocate(ROWS, COLS, LEVELS);
    }

    ~Maze() {
        clearMemory();
    }

    string toString() const {
        stringstream ss;
        for(int lev = 0; lev < LEVELS; lev++) {
            for(int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    ss << maze[row][col][lev];
                    if (col < COLS-1) {
                        ss << " ";
                    }
                }
                ss << endl;
            }
            if (lev < LEVELS-1) {
                ss << endl;
            }
        }
        return ss.str();
    }

    void randomMaze() {
        for (int lev = 0; lev < LEVELS; lev++) {
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    maze[row][col][lev] = rand() % 2;
                }
            }
        }
        maze[0][0][0] = 1;
        maze[ROWS-1][COLS-1][LEVELS-1] = 1;
    }

    bool importMaze(string file_name) {
        ifstream file(file_name.c_str());
        if (file.is_open()) {
            //remove first line
            string first_line;
            getline(file, first_line);

            string line;
            for (int lev = 0; lev < LEVELS; lev++) {
                getline(file, line);
                for (int row = 0; row < ROWS; row++) {
                    getline(file, line);
                    stringstream ss(line);
                    for (int col = 0; col < COLS; col++) {
                        int value;
                        ss >> value;
                        if (value != 0 && value != 1) {
                            return false;
                        }
                        maze[row][col][lev] = value;
                    }
                }
            }
        }
        return(true);
    }

    void solutionFile(string filename) {
        int *** temp_maze = maze;
        ofstream out;
        out.open(filename);
        if (mazeSolver(temp_maze, 0, 0, 0) == false) {
            out << "NO SOLUTION";
        }
        else {
            vector<string> solution(reverseSolution());
            out << "SOLUTION";
            for (int i = 0; i < solution.size(); i++) {
                out << solution[i] << endl;
            }
            out << to_string(ROWS-1) + " " + to_string(COLS-1) + " " + to_string(LEVELS-1);
        }

    }

    bool mazeSolver(int *** temp_maze, int x, int y, int z) {
        if (x < 0 || y < 0 || z < 0 || x >= ROWS || y >= COLS || z >= LEVELS) {
            return false;
        }
        else if (temp_maze[x][y][z] == VISITED || temp_maze[x][y][z] == WALL) {
            return false;
        }
        else if (temp_maze[x][y][z] != POSSIBLE_PATH) {
            return false;
        }
        else if (x == ROWS-1 && y == COLS-1 && z == LEVELS-1) {
            temp_maze[x][y][z] = PATH;
            solved.push_back(to_string(x) + " " + to_string(y) + " " + to_string(z));
            return true;
        }
        else {
            temp_maze[x][y][z] = VISITED;
            if (mazeSolver(temp_maze, x+1, y, z) || mazeSolver(temp_maze, x-1, y, z) || mazeSolver(temp_maze, x, y+1, z) || mazeSolver(temp_maze, x, y-1, z) ||
                    mazeSolver(temp_maze, x, y, z+1) || mazeSolver(temp_maze, x, y, z-1)) {
                stringstream ss;
                ss << x << " " << y << " " << z;
                solved.push_back(ss.str());
                return true;
            }
            else {
                return false;
            }
        }
    }

    vector<string> reverseSolution() {
        vector<string> temp_solved;
        for (int i = solved.size(); i > 0; i--) {
            temp_solved.push_back(solved[i]);
        }
        return temp_solved;
    }

    void allocate(int ROWS, int COLS, int LEVELS) {
        //Allocate memory
        maze = new int**[ROWS];
        for (int row = 0; row < ROWS; row++) {
            maze[row] = new int*[COLS];
        }
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                maze[row][col] = new int[LEVELS];
            }
        }
        //Fill with 1's
        for (int lev = 0; lev < LEVELS; lev++) {
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++)
                    maze[row][col][lev] = 1;
                }
        }
    }

    void clearMemory() {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                delete [] maze[row][col];
            }
            delete [] maze[row];
        }
        delete [] maze;
    }

};
