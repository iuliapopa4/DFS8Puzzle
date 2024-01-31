#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

const int N = 3;
const int goal[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

// Define a Puzzle structure to represent the state of the puzzle
struct Puzzle {
    int board[N][N];
    int steps;
    Puzzle* parent;

    // Default constructor
    Puzzle() : steps(0), parent(nullptr) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                board[i][j] = 0;
            }
        }
    }

    // Constructor with parameters to initialize the puzzle state
    Puzzle(int puzzle[N][N], int moves = 0, Puzzle* par = nullptr) : steps(moves), parent(par) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                board[i][j] = puzzle[i][j];
            }
        }
    }

    // Check if the puzzle is in the goal state
    bool isGoal() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] != ::goal[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    // Find the position of the empty tile (0)
    void findEmpty(int& emptyX, int& emptyY) {
        emptyX = -1;
        emptyY = -1;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == 0) {
                    emptyX = i;
                    emptyY = j;
                    return;
                }
            }
        }
    }

    // Get possible moves from the current state
    vector<Puzzle> getPossibleMoves() {
        vector<Puzzle> possibleMoves;
        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};

        int emptyX, emptyY;
        findEmpty(emptyX, emptyY);

        for (int k = 0; k < 4; ++k) {
            int newX = emptyX + dx[k];
            int newY = emptyY + dy[k];
            if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
                int newPuzzle[N][N];
                copyPuzzle(newPuzzle);
                swap(newPuzzle[emptyX][emptyY], newPuzzle[newX][newY]);
                possibleMoves.push_back(Puzzle(newPuzzle, steps + 1));
            }
        }
        return possibleMoves;
    }

    // Copy the puzzle state
    void copyPuzzle(int newPuzzle[N][N]) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                newPuzzle[i][j] = board[i][j];
            }
        }
    }
};

// Convert the puzzle state to a string for hashing
string PuzzleToString(const Puzzle& puzzle) {
    string strPuzzle;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            strPuzzle += to_string(puzzle.board[i][j]);
        }
    }
    return strPuzzle;
}

// Print the path from initial state to goal state
void printPath(Puzzle* current) {
    if (current == nullptr) {
        return;
    }
    printPath(current->parent);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << current->board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Solve the puzzle using depth-first search
void SolvePuzzle(Puzzle initial) {
    stack<Puzzle> nextPuzzles;
    unordered_set<string> exploredPuzzles;
    Puzzle solution;
    int maxDepth = 0;
    nextPuzzles.push(initial);

    while (!nextPuzzles.empty()) {
        Puzzle currentPuzzle = nextPuzzles.top();
        nextPuzzles.pop();
        exploredPuzzles.insert(PuzzleToString(currentPuzzle));

        if (currentPuzzle.isGoal()) {
            solution = currentPuzzle;
            break;
        }

        vector<Puzzle> possibleMoves = currentPuzzle.getPossibleMoves();
        for (Puzzle& currentMove : possibleMoves) {
            string currentMoveString = PuzzleToString(currentMove);
            if (exploredPuzzles.find(currentMoveString) == exploredPuzzles.end()) {
                currentMove.parent = new Puzzle(currentPuzzle.board, currentPuzzle.steps, currentPuzzle.parent);
                nextPuzzles.push(currentMove);
                exploredPuzzles.insert(currentMoveString);
                maxDepth = max(maxDepth, currentMove.steps);
            }
        }
    }

    if (solution.isGoal()) {
        cout << "Solution found!" << endl;
        cout << "Path from initial to goal: " << endl;
        printPath(&solution);
    } else {
        cout << "No solution found." << endl;
    }
}

int main() {
    int puzzle[N][N] = {{0, 1, 2}, {5, 4, 3}, {8, 7, 6}};
    Puzzle initial(puzzle);
    SolvePuzzle(initial);

    return 0;
}
