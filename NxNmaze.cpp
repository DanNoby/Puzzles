
#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string>

using namespace std;

const vector<pair<int, int>> directions = {
    {0, 2}, {2, 0}, {0, -2}, {-2, 0}
};

void generateMaze(int n, vector<vector<int>>& maze) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            maze[i][j] = 1;
        }
    }

    maze[0][0] = 0;

    stack<pair<int, int>> cellStack;
    cellStack.push({0, 0});

    while (!cellStack.empty()) {
        auto [x, y] = cellStack.top();
        vector<pair<int, int>> neighbors;

        for (const auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;

            if (nx >= 0 && nx < n && ny >= 0 && ny < n && maze[nx][ny] == 1) {
                int wallX = x + dir.first / 2;
                int wallY = y + dir.second / 2;
                if (wallX >= 0 && wallX < n && wallY >= 0 && wallY < n && maze[wallX][wallY] == 1) {
                    neighbors.push_back({nx, ny});
                }
            }
        }

        if (!neighbors.empty()) {
            int randIndex = rand() % neighbors.size();
            auto [nx, ny] = neighbors[randIndex];

            maze[(x + nx) / 2][(y + ny) / 2] = 0;
            maze[nx][ny] = 0;

            cellStack.push({nx, ny});
        } else {
            cellStack.pop();
        }
    }
}

void printMaze(vector<vector<int>>& maze) {
    cout << endl; cout << endl; cout << endl; cout << endl;
    maze[0][0] = -1;
    for (const auto& row : maze) {
        for (int cell : row) {
            if (cell == 1)
                cout << setw(4) << '#'; // Wall
            else if(cell == 0)
                cout << setw(4) << ' ';
            else
                cout << setw(4) << cell; // Path with incremented values
        }
        cout << endl;
    }
}

void printBackTrackMaze(vector<vector<int>>& maze, int n) {
    cout << endl; cout << endl;
    maze[0][0] = 100000;
    for (const auto& row : maze) {
        for (int cell : row) {
            if (cell == 1)
                cout << setw(4) << '#';
            else if(cell == 0)
                cout << setw(4) << ' ';
            else if(cell == 100000)
                cout << setw(4) << 0;
            else
                cout << setw(4) << ' ';
        }
        cout << endl;
    }
}

void backtrack(vector<vector<int>>& maze, int n) {
    int x = n - 1;
    int y = n - 1;

    // Check if we reached the destination during traversal
    if (maze[x][y] <= 1) {
        cout << "No path to trace back." << endl;
        return;
    }
    const vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int ptr = maze[x][y];

    // Backtrack from the end cell, marking the path with '*'
    while (ptr != -1) {
        maze[x][y] = 100000;
        int minX = x, minY = y;
        // Check all four directions to find the next cell in the path
        for (const auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            
            if (newX >= 0 && newX < n && newY >= 0 && newY < n &&
                maze[newX][newY] <= ptr && maze[newX][newY]!=1 && maze[newX][newY]!=0 && maze[newX][newY] < maze[minX][minY]) {
                minX = newX;
                minY = newY;
            }
        }
        x = minX; y = minY;
        ptr = maze[x][y];
    }
    printBackTrackMaze(maze,n);
    maze[0][0] = 100000; // Mark the start position as part of the path
}


void Traversal(vector<vector<int>>& maze, int trav, int n) {
    vector<pair<int, int>> directions = {
        {0, 1},  // Move right
        {1, 0},  // Move down
        {0, -1}, // Move left
        {-1, 0}  // Move up
    };

    vector<vector<bool>> visited(n, vector<bool>(n, false)); // Track visited cells
    int increment = 1;

    maze[0][0] = increment; // Start cell gets the value 1

    if (trav==0 || trav == 1) { // DFS / A*
        stack<pair<int, int>> S;
        S.push({0, 0});

        while (!S.empty()) {
            auto [x, y] = S.top();
            S.pop();
            
            if(trav==0) {
                int endx = n-1;
                int endy = n-1;
                
                int MH = abs(endx-x) + abs(endy-y);
                if(!S.empty()) {
                    auto [otherx, othery] = S.top();
                    int otherMH = abs(endx-otherx) + abs(endy-othery);
                    if(otherMH < MH) {
                        S.pop();
                        S.push({x,y});
                        S.push({otherx,othery});
                        continue;
                    }
                }
            }

            if (visited[x][y]) continue;

            visited[x][y] = true;
            maze[x][y] = increment;
            increment++;
            printMaze(maze);
            this_thread::sleep_for(chrono::milliseconds(150));

            if (x == n - 1 && y == n - 1) {
                cout << "Reached the destination at (" << x + 1 << ", " << y + 1 << ")\n";
                backtrack(maze,n);
                return;
            }

            for (const auto& dir : directions) {
                int newX = x + dir.first;
                int newY = y + dir.second;

                if (newX >= 0 && newX < n && newY >= 0 && newY < n &&
                    maze[newX][newY] == 0 && !visited[newX][newY]) {
                    S.push({newX, newY});
                }
            }
        }
    }
    else if(trav==2) { // BFS
        queue<pair<int, int>> Q;
        Q.push({0, 0});

        while (!Q.empty()) {
            auto [x, y] = Q.front();
            Q.pop();

            if (visited[x][y]) continue;

            visited[x][y] = true;
            maze[x][y] = increment;
            increment++;
            printMaze(maze);
            this_thread::sleep_for(chrono::milliseconds(150));

            if (x == n - 1 && y == n - 1) {
                cout << "Reached the destination at (" << x + 1 << ", " << y + 1 << ")\n";
                backtrack(maze,n);
                return;
            }

            for (const auto& dir : directions) {
                int newX = x + dir.first;
                int newY = y + dir.second;

                if (newX >= 0 && newX < n && newY >= 0 && newY < n &&
                    maze[newX][newY] == 0 && !visited[newX][newY]) {
                    Q.push({newX, newY});
                }
            }
        }
    }
    cout << "No path to the destination found.\n";
}



int main(void) {
    srand(static_cast<unsigned int>(time(0)));

    int n;
    cout << "Enter the size of the maze (n x n): ";
    cin >> n;

    if (n % 2 == 0) {
        n++;
    }

    vector<vector<int>> maze(n, vector<int>(n));
    generateMaze(n, maze);
    
    int trav;
    cout << "GBFS(0) / DFS(1) / BFS(2):" << endl;
    cin >> trav;
    
    Traversal(maze,trav,n);
    return 0;
}
