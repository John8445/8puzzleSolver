#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;

struct puzzleNode
{
    vector<int> puzzle;
    vector<int> goalPuzzle;
    /* std::vector<puzzleNode> adjacentVertexList; "if needed"*/
    int totalMoves;
    int totalCost;
    puzzleNode *upChild;
    puzzleNode *downChild;
    puzzleNode *rightChild;
    puzzleNode *leftChild;
    puzzleNode *parentNode;

};

list<vector<int>> dfsPath;
list<vector<int>> bfsPath;
list<vector<int>> dijkstraPath;
list<vector<int>> puzzles;
puzzleNode* root;

void deleteAll(puzzleNode* root)
{
    if (root != nullptr)
    {
        deleteAll(root->leftChild);
        deleteAll(root->rightChild);
        deleteAll(root->upChild);
        deleteAll(root->downChild);
        delete root;
    }
}

vector<int> moveRight(vector<int> puzzle, int zeroIndex){

    if(zeroIndex % 3 < 2) {
        int temp = puzzle.at(zeroIndex);
        puzzle.at(zeroIndex) = puzzle.at(zeroIndex + 1);
        puzzle.at(zeroIndex + 1) = temp;
        return puzzle;
    }
    return {};
}

vector<int> moveLeft(vector<int> puzzle, int zeroIndex){

    if(zeroIndex % 3 > 0) {
        int temp = puzzle.at(zeroIndex);
        puzzle.at(zeroIndex) = puzzle.at(zeroIndex - 1);
        puzzle.at(zeroIndex - 1) = temp;
        return puzzle;
    }
    return {};
}

vector<int> moveUp(vector<int> puzzle, int zeroIndex){

    if(zeroIndex > 2) {
        int temp = puzzle.at(zeroIndex);
        puzzle.at(zeroIndex) = puzzle.at(zeroIndex - 3);
        puzzle.at(zeroIndex - 3) = temp;
        return puzzle;
    }
    return {};
}

vector<int> moveDown(vector<int> puzzle, int zeroIndex){

    if(zeroIndex < 6) {
        int temp = puzzle.at(zeroIndex);
        puzzle.at(zeroIndex) = puzzle.at(zeroIndex + 3);
        puzzle.at(zeroIndex + 3) = temp;
        return puzzle;
    }
    return {};
}

int findZero(vector<int> puzzleToSearch){
    int zeroIndex = 0;

    for(int i = 0; i < puzzleToSearch.size(); i++) {
        if(puzzleToSearch.at(i) == 0) {
            zeroIndex = i;
        }
    }
    return zeroIndex;
}

void printPuzzle(vector<int> puzzleToPrint) {

    for(int i = 0; i < puzzleToPrint.size(); i++) {

         if( (i % 3 == 0) && (i != 0) ) {
            cout << endl;
        }

        cout << puzzleToPrint.at(i) << " ";

    }
    cout << endl << endl;
}

bool contains(vector<int> puz)
{
    list<vector<int>>::iterator it;
    for(it = puzzles.begin(); it != puzzles.end(); it++)
    {
        if (*it == puz)
            return true;
    }
    return false;
}

int calculateCost(vector<int> start, vector<int> finish, int zero)
{
    return finish[zero];
}

void createTree(puzzleNode* startState, vector<int> endState)
{
    root = startState;
    list<puzzleNode*> puzzleList;
    puzzleList.push_back(root);
    puzzleNode* curr;
    while (curr->puzzle != endState)
    {
        curr = puzzleList.front();
        puzzleList.pop_front();
        int i = findZero(curr->puzzle);
        vector<int> down = moveDown(curr->puzzle, i);
        vector<int> up = moveUp(curr->puzzle, i);
        vector<int> right = moveRight(curr->puzzle, i);
        vector<int> left = moveLeft(curr->puzzle, i);
        if (!down.empty() && !contains(down))
        {
            curr->downChild = new puzzleNode();
            curr->downChild->puzzle = down;
            curr->downChild->parentNode = curr;
            puzzles.push_back(down);
            curr->downChild->totalMoves = curr->totalMoves + 1;
            curr->downChild->totalCost = curr->totalCost + calculateCost(curr->puzzle, curr->downChild->puzzle, i);
            puzzleList.push_back(curr->downChild);
        }
        if (!up.empty() && !contains(up))
        {
            curr->upChild = new puzzleNode();
            curr->upChild->puzzle = up;
            curr->upChild->parentNode = curr;
            puzzles.push_back(up);
            curr->upChild->totalMoves = curr->totalMoves + 1;
            curr->upChild->totalCost = curr->totalCost + calculateCost(curr->puzzle, curr->upChild->puzzle, i);
            puzzleList.push_back(curr->upChild);
        }
        if (!right.empty() && !contains(right))
        {
            curr->rightChild = new puzzleNode();
            curr->rightChild->puzzle = right;
            curr->rightChild->parentNode = curr;
            puzzles.push_back(right);
            curr->rightChild->totalMoves = curr->totalMoves + 1;
            curr->rightChild->totalCost = curr->totalCost + calculateCost(curr->puzzle, curr->rightChild->puzzle, i);
            puzzleList.push_back(curr->rightChild);
        }
        if (!left.empty() && !contains(left))
        {
            curr->leftChild = new puzzleNode();
            curr->leftChild->puzzle = left;
            curr->leftChild->parentNode = curr;
            puzzles.push_back(left);
            curr->leftChild->totalMoves = curr->totalMoves + 1;
            curr->leftChild->totalCost = curr->totalCost + calculateCost(curr->puzzle, curr->leftChild->puzzle, i);
            puzzleList.push_back(curr->leftChild);
        }
    }
}

bool great(puzzleNode* n1, puzzleNode* n2)
{
    return !(n1->totalCost < n2->totalCost);   
}

puzzleNode* BFS(puzzleNode* startState, vector<int> endState)
{
    list<puzzleNode*> bfslist;
    puzzleNode* curr = startState;
    bfslist.push_back(curr);
    while (curr->puzzle != endState)
    {
        bfslist.pop_front();
        if (curr->downChild != nullptr)
        {
            bfslist.push_back(curr->downChild);
        }
        if (curr->upChild != nullptr)
        {
            bfslist.push_back(curr->upChild);
        }
        if (curr->rightChild != nullptr)
        {
            bfslist.push_back(curr->rightChild);
        }
        if (curr->leftChild != nullptr)
        {
            bfslist.push_back(curr->leftChild);
        }
        curr = bfslist.front();
    }
    return curr;
}

puzzleNode* DFS(puzzleNode* startState, vector<int> endState)
{
    list<puzzleNode*> dfslist;
    
    dfslist.push_back(startState);
    puzzleNode* curr = dfslist.front();
    vector<puzzleNode*> foundNodes;
    while (!dfslist.empty())
    {
        dfslist.pop_front();
        if (curr->downChild != nullptr)
        {
            if (curr->downChild->puzzle == endState)
            {
                foundNodes.push_back(curr->downChild);
            }
            dfslist.push_front(curr->downChild);
        }
        if (curr->upChild != nullptr)
        {
            if (curr->upChild->puzzle == endState)
            {
                foundNodes.push_back(curr->upChild);
            }
            dfslist.push_front(curr->upChild);
        }
        if (curr->rightChild != nullptr)
        {
            if (curr->rightChild->puzzle == endState)
            {
                foundNodes.push_back(curr->rightChild);
            }
            dfslist.push_front(curr->rightChild);
        }
        if (curr->leftChild != nullptr)
        {
            if (curr->leftChild->puzzle == endState)
            {
                foundNodes.push_back(curr->leftChild);
            }
            dfslist.push_front(curr->leftChild);
        }
        curr = dfslist.front();
    }
    int i;
    vector<puzzleNode*>::iterator it;
    puzzleNode* min = foundNodes.at(0);
    
    
    for (i = 0; i < foundNodes.size(); i++)
    {
        puzzleNode* temp = foundNodes.at(i);
        if (temp->totalMoves < min->totalMoves)
        {
            min = temp;
        }
    }
    return min;
}

puzzleNode* dijkstra(puzzleNode* startState, vector<int> endState)
{
    list<puzzleNode*> dijkstralist;
    puzzleNode* curr = startState;
    dijkstralist.push_back(startState);
    while (curr->puzzle != endState)
    {
        dijkstralist.pop_back();
        if (curr->downChild != nullptr)
        {
            dijkstralist.push_back(curr->downChild);
        }
        if (curr->upChild != nullptr)
        {
            dijkstralist.push_back(curr->upChild);
        }
        if (curr->rightChild != nullptr)
        {
            dijkstralist.push_back(curr->rightChild);
        }
        if (curr->leftChild != nullptr)
        {
            dijkstralist.push_back(curr->leftChild);
        }
        dijkstralist.sort(great);    
        curr = dijkstralist.back();
    } 
    return curr;
}

int main() 
{
    vector<int> puzzle1;
    puzzle1 = {1, 3, 4, 8, 0, 6, 7, 5, 2};
    puzzleNode* puz = new puzzleNode;
    puz->puzzle = puzzle1;
    puz->totalMoves = 0;
    puz->totalCost = 0;


    vector<int> endPuzzle;
    endPuzzle = {1, 2, 3, 8, 0, 4, 7, 6, 5};
    createTree(puz, endPuzzle);
    int algo;
    bool valid = false;
    do
    {   
        cout << endl << "Please select an algorithm" << endl <<
                        "by entering 1, 2, or 3:" << endl << 
                        "1) Breadth First Search" << endl <<
                        "2) Depth First Search" << endl <<
                        "3) Dijkstra's Algorirthm" << endl << endl;
        cin >> algo;
        if (algo == 1)
        {
            puzzleNode* x = BFS(puz, endPuzzle);
            bfsPath.push_front(x->puzzle);
            cout << "Cost: " << x->totalMoves << endl << endl;
            while (x->parentNode != nullptr)
            {
                bfsPath.push_front(x->parentNode->puzzle);
                x = x->parentNode;
            }
            list<vector<int>>::iterator it;
            for (it = bfsPath.begin(); it != bfsPath.end(); it++)
            {
                printPuzzle(*it);
            }
            valid = true;
        }
        else if (algo == 2)
        {
            puzzleNode* x = DFS(puz, endPuzzle);
            dfsPath.push_front(x->puzzle);
            cout << "Cost: " << x->totalMoves << endl << endl;
            while (x->parentNode != nullptr)
            {
                dfsPath.push_front(x->parentNode->puzzle);
                x = x->parentNode;
            }
            list<vector<int>>::iterator it;
            for (it = dfsPath.begin(); it != dfsPath.end(); it++)
            {
                printPuzzle(*it);
            }
            valid = true;
        }
        else if (algo == 3)
        {
            puzzleNode* x = dijkstra(puz, endPuzzle);
            dijkstraPath.push_front(x->puzzle);
            cout << "Cost: " << x->totalCost << endl << endl;
            while (x->parentNode != nullptr)
            {
                dijkstraPath.push_front(x->parentNode->puzzle);
                x = x->parentNode;
            }
            list<vector<int>>::iterator it;
            for (it = dijkstraPath.begin(); it != dijkstraPath.end(); it++)
            {
                printPuzzle(*it);
            }
            valid = true;
        }
        else 
            cout << "Invalid option. Please select a valid option." << endl;
    } while (!valid);
    
    deleteAll(root);
}