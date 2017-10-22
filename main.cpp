#include <iostream>
#include <queue>
#include <vector>
#include <limits>
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
//allows for functions to be passed as parameters
typedef void (* functionCall) (state &args);/*functionCall func*/
//Function Prototypes
state AStar(state problem, functionCall func);
void uniformCost(state &problem);
void misplacedTile(state &problem);
void manhattanDistance(state &problem);
void expandNode(priority_queue<state, vector<state>, compareWeight> &nodes, functionCall func);
bool goalTest(vector<vector<int> >  problem);
void printPuzzle(vector<vector<int> > problem);

int main (int argc, char* argv[] )
{
    int menu1Input =0, menu2Input = 0;
    state problem;
    state solution;

    cout << "Welcome to Carlos Santillana's 8-puzzle solver.\n";
    cout << "Type \"1\" to use the default puzzle or \"2\" to enter your own puzzle\n";
    while (menu1Input <1 || menu1Input > 2){
        cin >> menu1Input;
        if (menu1Input == 1 ){// initialize default puzzle
            problem.puzzle[0][0] = 1; problem.puzzle[0][1] = 2; problem.puzzle[0][2] = 3;
            problem.puzzle[1][0] = 4; problem.puzzle[1][1] = -1; problem.puzzle[1][2] = 6;
            problem.puzzle[2][0] = 7; problem.puzzle[2][1] = 5; problem.puzzle[2][2] = 8;
        }
        else if (menu1Input == 2){
            cout << "Sorry that feature has not been implemented yet\n";
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
        solution = AStar(problem, uniformCost);
    }
    else if (menu2Input == 2){
        solution = AStar(problem, misplacedTile);
    }
    else if (menu2Input == 3){
        solution = AStar(problem, manhattanDistance);
    }
    if (solution.puzzle[0][0] != 0 ){
    cout << "found solution \n";
    printPuzzle(solution.puzzle);
    }
    else{
        cout << "no solution\n";
    }
    return 0;
}
state AStar(state problem, functionCall func){
    priority_queue<state, vector<state>,compareWeight> nodes;
    state failure;
    failure.puzzle[0][0] = 0;
    nodes.push(problem);
    while (nodes.size() != 0){
        if (goalTest(nodes.top().puzzle) == 1){
            return nodes.top();
        }
        cout << "Expanding State" << endl;
        printPuzzle(nodes.top().puzzle);
        expandNode(nodes, func);
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
            if ( problem.puzzle[i][j] != count && count < BOARDSIZE*BOARDSIZE){
                mTiles++;
            }
            count++;
        }
    }
    problem.hCost = mTiles;
}
//Uses A* search with the Manhattan Distance heuristic
void manhattanDistance(state &problem){
    int mDistance =0;

    problem.hCost = mDistance;
}
//Expands node by branching  out its nodes
//Does this by doing the up right, down left operations
void expandNode(priority_queue<state, vector<state>, compareWeight> &nodes, functionCall func){
    state current = nodes.top();
    nodes.pop();
    bool found = false;
    for (int h=0; h < 4; h++){//does all operations
        for (int i=0; i < BOARDSIZE && found == false; i++){
            for (int j=0; j < BOARDSIZE && found == false; j++){
                if (current.puzzle[i][j] == -1){
                    if (h == 0 && i > 0){//up
                        current.puzzle[i][j] = current.puzzle[i-1][j];
                        current.puzzle[i-1][j] = -1;
                        current.gCost++;
                        func(current);
                        nodes.push(current);
                        current.puzzle[i-1][j] = current.puzzle[i][j];
                        current.puzzle[i][j] = -1;
                        current.gCost--;
                        found = true;
                    }
                    if (h == 1 && j < BOARDSIZE-1){//right
                        current.puzzle[i][j] = current.puzzle[i][j+1];
                        current.puzzle[i][j+1] = -1;
                        current.gCost++;
                        func(current);
                        nodes.push(current);
                        current.puzzle[i][j+1] = current.puzzle[i][j];
                        current.puzzle[i][j] = -1;
                        current.gCost--;
                        found = true;
                    }
                    if (h == 2 && i < BOARDSIZE-1){//down
                        current.puzzle[i][j] = current.puzzle[i+1][j];
                        current.puzzle[i+1][j] = -1;
                        current.gCost++;
                        func(current);
                        nodes.push(current);
                        current.puzzle[i+1][j] = current.puzzle[i][j];
                        current.puzzle[i][j] = -1;
                        current.gCost--;
                        found = true;
                    }
                    if (h == 3 &&  j > 0){//left
                        current.puzzle[i][j] = current.puzzle[i][j-1];
                        current.puzzle[i][j-1] = -1;
                        current.gCost++;
                        func(current);
                        nodes.push(current);
                        current.puzzle[i][j-1] = current.puzzle[i][j];
                        current.puzzle[i][j] = -1;
                        current.gCost--;
                        found = true;
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
    cout << endl;
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
            if (count== BOARDSIZE*BOARDSIZE && problem[BOARDSIZE-1][BOARDSIZE-1] != -1 ){
                done=0;
            }
            count++;
        }
    }
    return done;
}
