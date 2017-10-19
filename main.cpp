#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int BOARDSIZE = 3;
//Function Prototypes
typedef void (* functionCall) (int args);
vector<vector<int> > generalSearch(vector<vector<int> > problem, functionCall func);
void expandNode(queue<vector<vector<int> > > &nodes);
void queueingFunction(queue<vector<vector<int> > > &problem);
void uniformCostSearch(vector<vector<int> > problem);
bool goalTest(vector<vector<int> > problem);
void printPuzzle(vector<vector<int> > problem);

int main (int argc, char* argv[] )
{
    int menu1Input =0, menu2Input = 0;
    vector<vector<int> > problem (BOARDSIZE, vector<int>(BOARDSIZE));

    cout << "Welcome to Carlos Santillana's 8-puzzle solver.\n";
    cout << "Type \"1\" to use the default puzzle or \"2\" to enter your own puzzle\n";
    while (menu1Input <1 || menu1Input > 2){
        cin >> menu1Input;
        if (menu1Input == 1 ){
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
        generalSearch(problem, (functionCall) uniformCostSearch);
    }
    return 0;
}
//Does breadth first search
vector<vector<int> > generalSearch(vector<vector<int> > problem, functionCall func){
    queue<vector<vector<int> > > nodes;
    nodes.push(problem);
    while (nodes.size() != 0){
        if (goalTest(problem) == 1){
            return problem;
        }
        expandNode(nodes);

    }
    cout << "breath first search\n";
    printPuzzle(problem);

    return problem;
}
void expandNode(queue<vector<vector<int> > > &nodes){
    vector<vector<int> > current = nodes.front();
    nodes.pop();
    bool found = false;
    for (int h=0; h < 4; h++){//does all branches
        for (int i=0; i < BOARDSIZE && found == false; i++){
            for (int j=0; j < BOARDSIZE && found == false; j++){
                if (current[i][j] == -1){
                    if (h == 0 && i > 0){//up
                        current[i][j] = current[i-1][j];
                        current[i+1][j] = -1;
                        printPuzzle(current);
                        nodes.push(current);
                        current[i+1][j] = current[i-1][j];
                        current[i][j] = -1;
                        found = true;
                    }
                    if (h == 1 && j < BOARDSIZE-1){//right
                        current[i][j] = current[i][j-1];
                        current[i][j+1] = -1;
                        nodes.push(current);
                        current[i][j+1] = current[i][j-1];
                        current[i][j] = -1;
                        found = true;
                    }
                    if (h == 2 && i < BOARDSIZE-1){//down
                        current[i][j] = current[i+1][j];
                        current[i-1][j] = -1;
                        nodes.push(current);
                        current[i-1][j] = current[i+1][j];
                        current[i][j] = -1;
                        found = true;
                    }
                    if (h == 3 &&  j > 0){//left
                        current[i][j] = current[i-1][j];
                        current[i+1][j] = -1;
                        nodes.push(current);
                        current[i+1][j] = current[i-1][j];
                        current[i][j] = -1;
                        found = true;
                    }

                }
            }
        }
        found = false;

    }

}
void queueingFunction(vector<vector<int> > problem ){

}
void uniformCostSearch(vector<vector<int> > problem){

}
void printPuzzle(vector<vector<int> > problem){
    for (int i=0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            cout << problem[i][j] << " ";
        }
    }
}
//Tests if problem state is goal state
bool goalTest(vector<vector<int> > problem){
    bool done = 1;
    int count =1;
    for (int i=0; i < BOARDSIZE; i++){
        for (int j=0; j < BOARDSIZE; j++){
            cout << "count: " << count;
            cout << "; problem: " << problem [i][j] << endl;
            if ( problem[i][j] != count && count < BOARDSIZE*BOARDSIZE){
                done =0;
                cout << "broken here!\n";
            }
            if (count== BOARDSIZE*BOARDSIZE && problem[BOARDSIZE-1][BOARDSIZE-1] != -1 ){
                done=0;
                cout << "broken here!\n";
            }
            count++;
        }
    }
    return done;
}
