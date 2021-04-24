#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::vector;
using std::string;
using std::abs;
using std::ifstream;
using std::istringstream;
using std::sort;


enum class State {kStart, kFinish, kEmpty, kObstacle, kClosed, kPath};

// directional deltas (up,left,down,right)
const int delta[4][2] {{-1, 0}, {0, -1}, {1, 0}, {0,1}};


vector<State> ParseLine(string line){
    istringstream sline(line);
    char c;
    int i;
    vector<State> row;

    while (sline >> i >> c && c == ','){
        if (i == 0){
            row.push_back(State::kEmpty);
        } else {
            row.push_back(State::kObstacle);
        }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path){
    ifstream myfile(path);
    vector<vector<State>> board {};

    if (myfile){
        string line;
        while (getline(myfile, line)){
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}


// comparing the F values of two cells.
bool Compare(const vector<int> i, const vector<int> j){
    int func_1 = i[2] + i[3]; // f = g + h
    int func_2 = j[2] + j[3];

    return func_1 > func_2;
}


// sorting the two-dimensional vector of ints in descending order.
void CellSort(vector<vector<int>> *v){
    sort(v->begin(), v->end(), Compare);
}


//calculating manhattan distance
int Heuristic(int x1, int y1, int x2, int y2){
    return abs(x2 - x1) + abs(y2 - y1);
} 


// checking if the cell is valid
bool CheckValidCell(int x, int y, vector<vector<State>> grid){
    bool on_grid_x = (x >= 0 && x < grid.size());
    bool on_grid_y = (y >= 0 && y < grid[0].size());

    if (on_grid_x && on_grid_y){
        return grid[x][y] == State::kEmpty;
    }
    
    return false;
}


// adding a node to the open list and marking it as open
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid){
    openlist.push_back(vector<int> {x, y, g, h});
    grid[x][y] = State::kClosed;
}


// expanding current node's neighbors and adding them to the open list
void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid){
    int x = current[0];
    int y = current[1];
    int g = current[2];

    for (int i = 0; i < 4; i++){
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];

        if (CheckValidCell(x2, y2, grid)){
            int g2 = g + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, openlist, grid);
        }
    }
}


// A* Search Algorithm
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]){
    vector<vector<int>> open;

    // starting node
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0], goal[1]);
    AddToOpen(x, y, g, h, open, grid);

    while (open.size() > 0){
        // getting the next node
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        grid[x][y] = State::kPath;

        if (x == goal[0] && y == goal[1]){
            grid[init[0]][init[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kFinish;

            return grid;
        }

        ExpandNeighbors(current, goal, open, grid);
    }

    cout << "No Path was Found !" << '\n';
    return vector<vector<State>> {};
}


string CellString(State cell) {
  switch(cell) {
      case State::kObstacle: return "⛰️   ";
      case State::kPath: return "🚗   ";
      case State::kEmpty: return "E   ";
      case State::kClosed: return "C   ";
      case State::kStart: return "🚦   ";
      case State::kFinish: return "🏁   ";
      default: return "?   ";

  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (vector<State> row : board) {
    for (State each : row) {
      cout << CellString(each);
    }
    cout << "\n";
  }
}


int main() {
  auto board = ReadBoardFile("boards/board.csv");
  int init[2] {0, 0};
  int goal[2] {int(board.size() - 1), int(board[0].size() - 1)};
  auto solution = Search(board, init, goal);
  PrintBoard(solution);
}
