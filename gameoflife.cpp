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
            if(dr == 0 && dc == 0) continue;

            int nr = row + dr;
            int nc = col + dc;
            if(wrap){
                nr = (nr + rows) % rows;
                nc = (nc + cols) % cols;
            }else{
                if(nr < 0 || nr >= rows || nc < 0 || nc >= cols){
                    continue;
                }
            }
            count += cells[nr][nc];
        }
    }
    return count;
}

bool step(){
    std::vector<std::vector<int>> next(rows, std::vector<int>(cols, 0));

    for(int r = 0; r < rows; ++r){
        for(int c = 0; r < cols; ++c){
            int n = countNeighbors(r, c);
            
            if(cells[r][c] == 1){
                next[r][c] = (n == 2 || n == 3) ? 1 : 0;
            }else{
                next[r][c] = (n == 3) ? 1 : 0;
            }
        }
    }
    bool changed = (next != cells);
    cells = std::move(next);
    ++generation;
    return changed;
}
int population() const{
    int total = 0;
    for(const auto& row : cells){
        for(int cell : row){
            total += cell;
        }
    }
    return total;
}

void print() const{
    std::string frame = "\33[H";
    frame += "Generation: " + std::to_string(generation) + "   Population: " + std::to_string(population()) + "     \n";
    for(int r = 0; r < rows; ++r){
        for(int c = 0; c < cols; ++c){
            frame += (cells[r][c] == 1) ? '#' : '.';
        }
        frame += '\n';
    }
    std::cout << frame << std::flush;
}
};

int readIntInRange(const std::string& prompt, int low, int high){
    while(true){
        std::cout << prompt;
        int value;
        if(std::cin >> value){
            if(value >= low && value <= high){
                return value;
            }
            std::cout << "Please enter a number from " << low << "to " << high << "." << std::endl;
        }else{
            if(std::cin.eof()){
                std::cout << std::endl << "Input closed." << std::endl;
                std::exit(0);
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Numbers only please!" << std::endl;
        }
    }
}
int main(){
const int ROWS = 24;
const int COLS = 60;


std::cout << "=== Conway's game of life ===" << std::endl;
int pattern = readIntInRange("Do you wish to play the game?, enter 1 if yes, close the program if no: ", 1, 1);
int generations = readIntInRange("How many generations: (1-2000)", 1, 2000);
int delayMS = readIntInRange("Delay between frames (1-1000): ", 1, 1000);
int wrapChoice = readIntInRange("Edges: 1 = wrap around, 0 = walls: ", 0, 1);

life game(ROWS, COLS, wrapChoice == 1);

switch(pattern){
    case 1: 
    game.randomize(0.30);
    break;
    default:
    std::cout << "Enter a valid number";
    break;
}

std::cout << "\033[2J";

for(int g = 0; g <= generations; ++g){
    game.print();
    if(g == generations) break;

    std::this_thread::sleep_for(std::chrono::milliseconds(delayMS));

    bool changed = game.step();
    if(!changed){
        game.print();
        std::cout << "Everything died";
        break;
    }
}
std::cout << "done!";
return 0;
}
