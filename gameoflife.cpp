#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <chrono>
#include <limits>
#include <cstdlib>
#include <utility>

class life{
private:
int rows;
int cols;
bool wrap;
int generation = 0;

std::vector<std::vector<int>> cells;

public:
life(int rows, int cols, bool wrap) : rows(rows), cols(cols), wrap(wrap), cells(rows, std::vector<int>(cols, 0)) {}

void setAlive(int row, int col){
if(row >= 0 && row < rows && col >= 0 && col < cols){
    cells[row][col] = 1;
}
}
void placePattern(const std::vector<std::string>& shape, int top, int left){
    for(int r = 0; r <static_cast<int>(shape.size()); ++r){
        for(int c = 0; c <static_cast<int>(shape[r].size()); ++c){
            if(shape[r][c] == '0'){
                setAlive(top + r, left + c);
            }
        }
    }
}

void randomize(double chance){
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for(int r = 0; r < rows; ++r){
        for(int c = 0; c < cols; ++c){
            cells[r][c] = (dist(rng) < chance) ? 1 : 0;
        }
    }
}

int countNeighbors(int row, int col) const{
    int count = 0;
    for(int dr = -1; dr <= 1; ++dr){
        for(int dc = -1; dc <= 1; ++dc){
            if(dr == 0 && dc == 0) continue
        }
    }
}

};


int main(){



}
