#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int BOARDSIZE = 3;
//allows for functions to be passed as parameters
typedef void (* functionCall) (int args);/*functionCall func*/
//Function Prototypes
vector<vector<int> > generalSearch(vector<vector<int> > problem);
vector<vector<int> > misplacedTile(vector<vector<int> > problem);
vector<vector<int> > manhattanDistance(vector<vector<int> > problem);
void expandNode(queue<vector<vector<int> > > &nodes);
void queueingFunction(queue<vector<vector<int> > > &problem);
bool goalTest(vector<vector<int> > problem);
void printPuzzle(vector<vector<int> > problem);

int main (int argc, char* argv[] )
{
    int menu1Input =0, menu2Input = 0;
    vector<vector<int> > problem (BOARDSIZE, vector<int>(BOARDSIZE));
    vector<vector<int> > solution (BOARDSIZE, vector<int>(BOARDSIZE));

    cout << "Welcome to Carlos Santillana's 8-puzzle solver.\n";
    cout << "Type \"1\" to use the default puzzle or \"2\" to enter your own puzzle\n";
    while (menu1Input <1 || menu1Input > 2){
        cin >> menu1Input;
        if (menu1Input == 1 ){// initialize default puzzle
            problem[0][0] = 1; problem[0][1] = 2; problem[0][2] = 3;
            problem[1][0] = 4; problem[1][1] = -1; problem[1][2] = 6;
            problem[2][0] = 7; problem[2][1] = 5; problem[2][2] = 8;
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
        solution = generalSearch(problem);
    }
    else if (menu2Input == 2){
        solution = misplacedTile(problem);
    }
    else if (menu2Input == 3){
        solution = manhattanDistance(problem);
    }
    if (solution[0][0] != 0 ){
    cout << "found solution \n";
    printPuzzle(solution);
    }
    else{
        cout << "no solution\n";
    }
    return 0;
}
//Does breadth first search
vector<vector<int> > generalSearch(vector<vector<int> > problem){
    queue<vector<vector<int> > > nodes;
    vector<vector<int> > failure (1, vector<int>(1));
    failure[0][0] = 0;
    nodes.push(problem);
    while (nodes.size() != 0){
        if (goalTest(nodes.front()) == 1){
            return nodes.front();
        }
        cout << "Expanding State" << endl;
        printPuzzle(nodes.front());
        expandNode(nodes);
    }
    return failure;
}
//Uses A* search with the Misplaced Tile heuristic
vector<vector<int> > misplacedTile(vector<vector<int> > problem){

}
//Uses A* search with the Manhattan Distance heuristic
vector<vector<int> > manhattanDistance(vector<vector<int> > problem){

}
//Expands node by branching  out its nodes
//Does this by doing the up right, down left operations
void expandNode(queue<vector<vector<int> > > &nodes){
    vector<vector<int> > current = nodes.front();
    nodes.pop();
    bool found = false;
    for (int h=0; h < 4; h++){//does all operations
        for (int i=0; i < BOARDSIZE && found == false; i++){
            for (int j=0; j < BOARDSIZE && found == false; j++){
                if (current[i][j] == -1){
                    if (h == 0 && i > 0){//up
                        current[i][j] = current[i-1][j];
                        current[i-1][j] = -1;
                        nodes.push(current);
                        current[i-1][j] = current[i][j];
                        current[i][j] = -1;
                        found = true;
                    }
                    if (h == 1 && j < BOARDSIZE-1){//right
                        current[i][j] = current[i][j+1];
                        current[i][j+1] = -1;
                        nodes.push(current);
                        current[i][j+1] = current[i][j];
                        current[i][j] = -1;
                        found = true;
                    }
                    if (h == 2 && i < BOARDSIZE-1){//down
                        current[i][j] = current[i+1][j];
                        current[i+1][j] = -1;
                        nodes.push(current);
                        current[i+1][j] = current[i][j];
                        current[i][j] = -1;
                        found = true;
                    }
                    if (h == 3 &&  j > 0){//left
                        current[i][j] = current[i][j-1];
                        current[i][j-1] = -1;
                        nodes.push(current);
                        current[i][j-1] = current[i][j];
                        current[i][j] = -1;
                        found = true;
                    }
                }
            }
        }
        found = false;

    }

}
// void queueingFunction(vector<vector<int> > problem ){
//
// }

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
