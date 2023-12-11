#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

const int N = 3;
const vector<vector <int> > goalState = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

struct PuzzleState {
    vector<vector<int>> state;
    int heuristic;

    PuzzleState(vector<vector<int>> s) : state(s), heuristic(calculateHeuristic(s)) {}

    int calculateHeuristic(const vector<vector<int>>& s) const {
        int h = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (s[i][j] != 0) {
                    int goalRow, goalCol;
                    goalRow = (s[i][j] - 1) / N;
                    goalCol = (s[i][j] - 1) % N;
                    h += abs(i - goalRow) + abs(j - goalCol);
                }
            }
        }
        return h;
    }

    bool isGoalState() const {
        return state == goalState;
    }

    void printState() const {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << state[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

struct CompareHeuristic {
    bool operator()(const PuzzleState& lhs, const PuzzleState& rhs) const {
        return lhs.heuristic > rhs.heuristic;
    }
};

pair<int, int> findBlank(const vector<vector<int>>& state) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (state[i][j] == 0) {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // Blank not found (error condition)
}

void greedyBestFirstSearch(const PuzzleState& initialState) {
    priority_queue<PuzzleState, vector<PuzzleState>, CompareHeuristic> pq;
    map<vector<vector<int>>, bool> visited;

    pq.push(initialState);

    while (!pq.empty()) {
        PuzzleState current = pq.top();
        pq.pop();

        if (visited[current.state]) {
            continue;
        }

        visited[current.state] = true;

        current.printState();

        if (current.isGoalState()) {
            cout << "Goal state reached!" << endl;
            return;
        }

        pair<int, int> blankPos = findBlank(current.state);
        int blankRow = blankPos.first;
        int blankCol = blankPos.second;

        vector<pair<int, int>> moves = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& move : moves) {
            int newRow = blankRow + move.first;
            int newCol = blankCol + move.second;

            if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N) {
                vector<vector<int>> newState = current.state;
                swap(newState[blankRow][blankCol], newState[newRow][newCol]);

                PuzzleState nextState(newState);
                if (!visited[nextState.state]) {
                    pq.push(nextState);
                }
            }
        }
    }

    cout << "Goal state not reached!" << endl;
}

int main() {
    vector<vector<int>> initialState = {{8, 0, 6}, {5, 4, 7}, {2, 3, 1}};

    PuzzleState initialPuzzleState(initialState);

    greedyBestFirstSearch(initialPuzzleState);

    return 0;
}
