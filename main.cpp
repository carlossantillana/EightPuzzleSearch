#include <iostream>
#include <queue>
#include <utility>
#include <string>
#include <unordered_set>
using namespace std;
const int BOARDSIZE = 3;

struct state{
    vector<vector<int> > puzzle = vector<vector<int> >(BOARDSIZE, vector<int> (BOARDSIZE));
    int gCost = 0, hCost = 0;
};

struct compareWeight {
    bool operator()(state const & s1, state const & s2) {
        return (s1.gCost + s1.hCost) > (s2.gCost + s2.hCost);
    }
};
struct Hash {
   size_t operator() (const state &problem) const {
       string tmp = "";
       for (int i=0; i < BOARDSIZE; i++){
           for (int j=0; j < BOARDSIZE; j++){
               tmp += to_string (problem.puzzle[i][j]);
           }
       }
         return (tmp.length());
       }
};
inline bool operator == (state const& lhs, state const& rhs)
{
    bool ret = true;
    for (int i=0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            if (lhs.puzzle[i][j] != rhs.puzzle[i][j]){
                ret = false;
            }
        }
    }
    return ret;
}
//allows for functions to be passed as parameters
typedef void (* functionCall) (state &args);/*functionCall func*/
//Function Prototypes
state AStar(state problem, functionCall func, float &expandedNodes, float &maxQueue, unordered_set<state, Hash> &hashTable);
void uniformCost(state &problem);
void misplacedTile(state &problem);
void manhattanDistance(state &problem);
pair<int,int> findLocation(int wrongLoc);
void expandNode(priority_queue<state, vector<state>, compareWeight> &nodes, functionCall func, float &expandedNodes, unordered_set<state, Hash> &hashTable);
bool goalTest(vector<vector<int> >  problem);
void printPuzzle(vector<vector<int> > problem);

int main (int argc, char* argv[] )
{vector<vector<int> > puzzle;
    int menu1Input =0, menu2Input = 0;
    int p1=0, p2=0, p3=0;
    float expandedNodes = 0, maxQueue =0;
    state problem, solution;
    unordered_set<state, Hash > hashTable;

    cout << "Welcome to Carlos Santillana's 8-puzzle solver.\n";
    cout << "Type \"1\" to use the default puzzle or \"2\" to enter your own puzzle\n";
    while (menu1Input <1 || menu1Input > 2){
        cin >> menu1Input;
        if (menu1Input == 1 ){// initialize default puzzle
            problem.puzzle[0][0] = 1; problem.puzzle[0][1] = 2; problem.puzzle[0][2] = 3;
            problem.puzzle[1][0] = 4; problem.puzzle[1][1] = 0; problem.puzzle[1][2] = 6;
            problem.puzzle[2][0] = 7; problem.puzzle[2][1] = 5; problem.puzzle[2][2] = 8;
        }
        else if (menu1Input == 2){
            cout << "Enter your puzzle, use a zero to represent the blank\n";
            cout << "Enter the first row, use space or tabs between numbers: ";
            cin >> p1 >> p2 >> p3;
            problem.puzzle[0][0] = p1; problem.puzzle[0][1] = p2; problem.puzzle[0][2] = p3;
            cout << "Enter the second row, use space or tabs between numbers: ";
            cin >> p1 >> p2 >> p3;
            problem.puzzle[1][0] = p1; problem.puzzle[1][1] = p2; problem.puzzle[1][2] = p3;
            cout << "Enter the third row, use space or tabs between numbers: ";
            cin >> p1 >> p2 >> p3;
            problem.puzzle[2][0] = p1; problem.puzzle[2][1] = p2; problem.puzzle[2][2] = p3;
            printPuzzle(problem.puzzle);
        }
        if (menu1Input <1  || menu1Input > 2){
            cout << "Invalid input\n";
            cout << "Type \"1\" to use the default puzzle or \"2\" to enter your own puzzle\n";
        }
    }
    cout << "Enter your choice of algorithm\n";
    cout << "1. Uniform Cost Search\n";
    cout << "2. A* with the Misplaced tile heurisitic\n";
    cout << "3. A* with the Manhattan distance heuristic\n";
    while (menu2Input < 1 || menu2Input > 3){
        cin >> menu2Input;
        if (menu2Input <1  || menu2Input > 3){
            cout << "Invalid input\n";
            cout << "Enter your choice of algorithm\n";
            cout << "1. Uniform Cost Search\n";
            cout << "2. A* with the Misplaced tile heurisitic\n";
            cout << "3. A* with the Manhattan distance heuristic\n";
        }
    }
    if (menu2Input == 1){
        solution = AStar(problem, uniformCost, expandedNodes, maxQueue, hashTable);
    }
    else if (menu2Input == 2){
        solution = AStar(problem, misplacedTile, expandedNodes, maxQueue, hashTable);
    }
    else if (menu2Input == 3){
        solution = AStar(problem, manhattanDistance, expandedNodes, maxQueue, hashTable);
    }
    if (solution.puzzle[0][0] != -1 ){
    cout << "Goal! \n";
    printPuzzle(solution.puzzle);
    cout << "To solve this problem the search algorithm expanded a total of ";
    cout <<  expandedNodes << " nodes.\n";
    cout << "The maximum number of nodes in the queue at any one time was ";
    cout << maxQueue << ".\n";
    cout << "the depth of the goal node was " << solution.gCost << ".\n";
    }
    else{
        cout << "no solution\n";
    }
    return 0;
}
//Runs A* search
state AStar(state problem, functionCall func, float &expandedNodes, float &maxQueue, unordered_set<state, Hash> &hashTable){
    priority_queue<state, vector<state>,compareWeight> nodes;
    state failure;
    bool first = true;

    failure.puzzle[0][0] = -1;
    nodes.push(problem);
    hashTable.emplace(problem);
    //for(int i=0; i < 4; i++){
    while (nodes.size() != 0){
        if (goalTest(nodes.top().puzzle) == 1){
            return nodes.top();
        }
        if (first){
            cout << "Expanding state\n";
            printPuzzle(nodes.top().puzzle);
            first = false;
        }
        else{
            cout << "The best state to expand with a g(n) = "  << nodes.top().gCost;
            cout << " and h(n) = " << nodes.top().hCost  << " is ..."<< endl;
            printPuzzle(nodes.top().puzzle);
            cout << "Expanding this node...\n\n";
        }
        expandNode(nodes, func, expandedNodes, hashTable);
        if (nodes.size() > maxQueue)
            maxQueue = nodes.size();
    }
    return failure;
}
void uniformCost(state &problem){
    problem.hCost = 0;
}
//Uses A* search with the Misplaced Tile heuristic
void misplacedTile(state &problem){
    int count =1;
    int mTiles = 0;
    for (int i=0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            if (problem.puzzle[i][j] != count && problem.puzzle[i][j] != 0){
                mTiles++;
            }
            count++;
        }
    }
    problem.hCost = mTiles;
}
//Uses A* search with the Manhattan Distance heuristic
void manhattanDistance(state &problem){
    int count = 1;
    int mDist =0;
    pair<int,int> rightLoc;
    for (int i=0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            if (problem.puzzle[i][j] != count && problem.puzzle[i][j] != 0){
                rightLoc = findLocation(problem.puzzle[i][j]);
                mDist += abs(i - rightLoc.first) + abs(j - rightLoc.second);
            }
            count++;
        }
    }
    problem.hCost = mDist;
}
//Manhattan Distance helper function which finds coordinates of misplaced tile
pair<int,int> findLocation(int wrongLoc){
    int count = 1;
    pair<int,int> rightLoc;
    for (int i=0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            if (count == wrongLoc)
                return rightLoc = make_pair(i,j);
                count++;
        }
    }
}
//Expands node by branching  out its nodes
//Does this by doing the up right, down left operations
void expandNode(priority_queue<state, vector<state>, compareWeight> &nodes, functionCall func, float &expandedNodes, unordered_set<state, Hash> &hashTable){
    state current = nodes.top();
    nodes.pop();
    bool found = false;
    unordered_set<state, Hash>::const_iterator got;
    for (int h=0; h < 4; h++){//does all operations
        for (int i=0; i < BOARDSIZE && found == false; i++){
            for (int j=0; j < BOARDSIZE && found == false; j++){
                if (current.puzzle[i][j] == 0){
                    if (h == 0 && i > 0){//up
                        current.puzzle[i][j] = current.puzzle[i-1][j];
                        current.puzzle[i-1][j] = 0;
                        current.gCost++;
                        got = hashTable.find (current);
                        // cout << "looking for\n";
                        // printPuzzle(current.puzzle);
                        if (got == hashTable.end()){
                            func(current);
                            nodes.push(current);
                            // cout << "not found\n";
                        }
                        // else{
                        //     cout << "found\n";
                        // }
                        current.puzzle[i-1][j] = current.puzzle[i][j];
                        current.puzzle[i][j] = 0;
                        current.gCost--;
                        found = true;
                        expandedNodes++;
                    }
                    if (h == 1 && j < BOARDSIZE-1){//right
                        current.puzzle[i][j] = current.puzzle[i][j+1];
                        current.puzzle[i][j+1] = 0;
                        current.gCost++;
                        // cout << "looking for\n";
                        // printPuzzle(current.puzzle);
                        if (got == hashTable.end()){
                            func(current);
                            nodes.push(current);
                            // cout << "not found\n";
                        }
                        // else{
                        //     cout << "found\n";
                        // }
                        current.puzzle[i][j+1] = current.puzzle[i][j];
                        current.puzzle[i][j] = 0;
                        current.gCost--;
                        found = true;
                        expandedNodes++;
                    }
                    if (h == 2 && i < BOARDSIZE-1){//down
                        current.puzzle[i][j] = current.puzzle[i+1][j];
                        current.puzzle[i+1][j] = 0;
                        current.gCost++;
                        // cout << "looking for\n";
                        // printPuzzle(current.puzzle);
                        if (got == hashTable.end()){
                            func(current);
                            nodes.push(current);
                            // cout << "not found\n";
                        }
                        // else{
                        //     cout << "found\n";
                        // }
                        current.puzzle[i+1][j] = current.puzzle[i][j];
                        current.puzzle[i][j] = 0;
                        current.gCost--;
                        found = true;
                        expandedNodes++;
                    }
                    if (h == 3 &&  j > 0){//left
                        current.puzzle[i][j] = current.puzzle[i][j-1];
                        current.puzzle[i][j-1] = 0;
                        current.gCost++;
                        // cout << "looking for\n";
                        // printPuzzle(current.puzzle);
                        if (got == hashTable.end()){
                            func(current);
                            nodes.push(current);
                            // cout << "not found\n";
                        }
                        // else{
                        //     cout << "found\n";
                        // }
                        current.puzzle[i][j-1] = current.puzzle[i][j];
                        current.puzzle[i][j] = 0;
                        current.gCost--;
                        found = true;
                        expandedNodes++;
                    }
                }
            }
        }
        found = false;
    }
}
//prints state of puzzle in matrix notation
void printPuzzle(vector<vector<int> > problem){
    for (int i=0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            cout << problem[i][j] << " ";
            if (j == 2)
                cout << endl;
        }
    }
}
//Tests if problem state is goal state
bool goalTest(vector<vector<int> > problem){
    bool done = 1;
    int count =1;
    for (int i=0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            if ( problem[i][j] != count && count < BOARDSIZE*BOARDSIZE){
                done =0;
            }
            if (count== BOARDSIZE*BOARDSIZE && problem[BOARDSIZE-1][BOARDSIZE-1] != 0 ){
                done=0;
            }
            count++;
        }
    }
    return done;
}
