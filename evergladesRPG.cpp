#include<iostream>
#include<iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>
#include<windows.h>
#include <conio.h>
#include <cmath>
using namespace std;

//Global Constant
const int ROWS = 8; // Map Col
const int COLS = 8; // Map Col
const int MAX_DANGER = 28;

// Structure to hold the game state
struct GameState
{
    char everGladesMap[ROWS][COLS];
    bool dangerCells[ROWS][COLS];
    int timeLeft = 20;
    int rangerRow, rangerCol, touristRow, touristCol;
    string dangerName[5] = {"Hungry Alligator", "Swarm of Giant Mosquitos", "Venomous Spider", "Python", "Jaguar"};
    char dangerFill[5] = {'A', 'M', 'S', 'P', 'J'};
};

// Function Prototypes
void displayMenuInterface(int &choice);
void processUserChoice(GameState &gameState, int &choice);
void displayRules();
void createMap(GameState &gameState);
void displayMap(GameState &gameState);
void placeDangers(GameState &gameState, int numDangers);
void handleDanger(GameState &gameState, int newRow, int newCol);
bool moveRanger(GameState &gameState);
void print_with_delay(const string &text, int delay = 50);

int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator

    // Variables
    int choice;
    GameState gameState;
    
    cout << endl;
    print_with_delay("Lost in the Everglades ... ");

    // Menu Interface
    do
    {
        displayMenuInterface(choice);
        processUserChoice(gameState, choice);
    } while (choice != 3);

    return 0;
}

void displayMenuInterface(int& choice)
{
    print_with_delay("\nChoose one of the following options");
    print_with_delay("\t1. See rules.");
    print_with_delay("\t2. Play game.");
    print_with_delay("\t3. Quit", 100);

    cout << "Option: ";
    cin >> choice;

    while (choice < 1 || choice > 3) {
    cout << "\nError ... Invalid Option. Try Again."
        << "\nOption: ";
    cin >> choice;
    };
}

void processUserChoice(GameState &gameState, int &choice){

    switch (choice)
        {
        case 1:
            // Function to see rules
            displayRules();
            break;
        case 2:
            cout << endl;
            print_with_delay("Lost in the Everglades ... ");
            cout << endl;

            createMap(gameState);
            displayMap(gameState);

            while (gameState.timeLeft > 0) // Loop for user input on moves
            {
                moveRanger(gameState);
                displayMap(gameState);

                // Check if the game ended due to rescuing tourists or time expiration
                if (gameState.rangerRow == gameState.touristRow && gameState.rangerCol == gameState.touristCol)
                {
                    cout << endl;
                    cout << "Congratulations! You rescued the tourists. The Everglades adventure ends successfully!\n";
                    gameState.timeLeft = 20;
                    break; // Exit the loop when the game ends
                }
                else if (gameState.timeLeft <= 0)
                {
                    print_with_delay("Time has run out! The fate of the tourists is forever unknown.");
                    gameState.timeLeft = 20;
                    break; // Exit the loop when time runs out
                }
            }
            break;
        case 3:
            // Quit
            print_with_delay("Good Bye ... ");
            break;
        }
}

void displayRules()
{
cout << "\nA group of tourists have been lost in the Everglades and a Ranger must rescue them before time runs out." << endl
    << "\nThe Everglades map is represented by a 8x8 matrix." <<"\nThe Ranger (R) starts the rescue at the upper-left"
    << "corner of the park, the group of tourists (T) are lost at the lower-right corner." << "\nThe ranger has 20 gongs of "
    << "time to find and rescue the tourists before they perish." << endl 
    << "\nDuring the journey, the ranger might find dangers." << endl
    << "When the ranger finds a danger, they can choose to fight them, or hide and wait for them to leave." << endl
    << "\nIf the Ranger waits" << endl << "\tMoves to the desired cell\n" <<"\tLoses 3 gongs of time" << endl  
    << "\nIf the Ranger fights and wins" << endl << "\tMoves to the desired cell\n" << "\tGains 2 gong of time" << endl
    << "\nIf the Ranger fights and loses" << endl << "\tGoes back to the initial cell\n" << "\tLoses 4 gongs of time\n" 
    << "\tThe danger remains in the cell" << endl 
    << "\nIf the Ranger moves to a cell with no dangers\n" << "\tLoses 1 gong of time" << endl
    << "\nThe game ends when either:" << endl <<"\tThe ranger rescues the group of tourists\n" 
    << "\tThe time expires and the fate of the tourists is forever unknown\n";
}

void displayMap(GameState &gameState){
// Loop to create column numbers on top of array
cout << "   ";
    for (int i = 0; i <COLS; ++i) {
        cout << setw(2) << left << i << "  ";
    }
    cout << endl;
    /* In order to hide the dangers on the map we need to check if the danger cell is true or false
    If the danger cell is false then we display our normal place holder '*'  if it is true then we display the contents of the cell
    Thus revealing the cell to the user We determine it is true by running the handleDanger function and setting the danger cell to true 
    if our Ranger loses their fight */
    for (int x = 0; x < ROWS; x++) {
        cout << x;
        for (int y = 0; y < COLS; y++) {
            if (!gameState.dangerCells[x][y] && gameState.everGladesMap[x][y] != 'R' && gameState.everGladesMap[x][y] != 'T' && gameState.everGladesMap[x][y] != ' ' ) {
                cout << "| " << setw(1) << left << '*' << " ";  // Display '*' for hidden dangers
            } else {
                cout << "| " << setw(1) << left << gameState.everGladesMap[x][y] << " "; 
            }
        }
        cout << "|" << endl;
    }
    cout << endl;
    cout << "\nTime left: " << gameState.timeLeft << " gongs\n";
};

void placeDangers(GameState &gameState, int MAX_DANGERS) {
    
    // Here we are randomly placing dangers on the map
    for (int i = 0; i < MAX_DANGERS; ++i) {
        int dangerRow, dangerCol;
        do {
            dangerRow = rand() % ROWS;
            dangerCol = rand() % COLS;
        } while (gameState.everGladesMap[dangerRow][dangerCol] != '*');

        char dangerSymbol = gameState.dangerFill[rand() %5];
        // Mark the cell with a danger symbol
        gameState.everGladesMap[dangerRow][dangerCol] = dangerSymbol; 
    }
}

void createMap(GameState &gameState) {
    // Initialize the map, ranger, and tourist positions
    // Set all cells to '*'
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            gameState.everGladesMap[i][j] = '*';
        }
    }

    // Initialize the danger cells to false
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            gameState.dangerCells[i][j] = false;
        }
    }

    // Set ranger position to the upper-left corner
    gameState.rangerRow = 0;
    gameState.rangerCol = 0;
    gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = 'R';

    // Set tourist position to the lower-right corner
    gameState.touristRow = 7;
    gameState.touristCol = 7;
    gameState.everGladesMap[gameState.touristRow][gameState.touristCol] = 'T';

    // Place dangers randomly on the map
    placeDangers(gameState, MAX_DANGER);
}

bool moveRanger(GameState &gameState) {
    int newRow, newCol;

    // Get user input for the new position
    cout << "Enter next cell (row & col): ";
    cin >> newRow >> newCol;

    // Validate the user input
    if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS) {
        cout << "Invalid cell. Please enter valid row and col values." << endl;
        return false;
    }else if(newRow > gameState.rangerRow + 1 || newCol > gameState.rangerCol + 1 ){
        cout << "Invalid cell. Please enter valid row and col Values." << endl;
        return false;
    }

    // Check if the new cell is free or a danger
    char cellContent = gameState.everGladesMap[newRow][newCol];
    if (cellContent == '*') {
        // Free cell, the ranger advances and loses 1 gong of time
        gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = ' ';  // Leave the current spot blank
        gameState.rangerRow = newRow;
        gameState.rangerCol = newCol;
        gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = 'R';  // Move the ranger to the new spot
        gameState.timeLeft -= 1;
        return true;
    } else if (cellContent == 'T') {                      
        gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = ' ';  // Leave the current spot blank
        gameState.rangerRow = newRow;
        gameState.rangerCol = newCol;
        gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = 'R';  // Move the ranger to the new spot
        return true;  // Updated the ranger position, thus program will end as game has been won
    } else if (cellContent == 'A' || cellContent == 'M' || cellContent == 'S' || cellContent == 'P' || cellContent == 'J') {
        // Danger cell, prompt the user for action
        handleDanger(gameState,newRow, newCol);
        return true;
    } else {
        // Other cases (e.g., 'R'), consider adding additional checks if needed
        cout << "Invalid move. Please choose a different cell." << endl;
        return false;
    }
return true;
}

void handleDanger(GameState &gameState, int newRow, int newCol)
{
    // Check if the destination cell contains a danger
    for (int i = 0; i < 4; ++i)
    {
        if (gameState.everGladesMap[newRow][newCol] == gameState.dangerFill[i])
        {
            // Danger found, prompt user to choose an action
            cout << "---> Watch out! There is a " << gameState.dangerName[i] << " ahead." << endl;
            cout << endl << "Choose your next move:" << endl;
            cout << "\t1. Wait until it leaves." << endl;
            cout << "\t2. Fight it." << endl;

            int action;
            cout << "Move: ";
            cin >> action;

            switch (action)
            {
            case 1:
                // Wait option, lose 5 gongs of time
                cout << "\n---> " << gameState.dangerName[i] << " is gone ... You Advance!" << endl;
                gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = ' '; // Clear the previous cell
                gameState.rangerRow = newRow;
                gameState.rangerCol = newCol;
                gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = 'R'; // Move to the new cell
                gameState.timeLeft -= 5;
                break;
            case 2:
                // Fight option
                if (rand() % 2 == 0)
                {
                    // Win the fight, move to the desired cell, lose 2 gongs of time
                    cout << "\nYou fight the " << gameState.dangerName[i] << " and win ... You Advance!" << endl;
                    gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = ' '; // Clear the previous cell
                    gameState.rangerRow = newRow;
                    gameState.rangerCol = newCol;
                    gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = 'R'; // Move to the new cell
                    gameState.timeLeft += 2;
                }
                else
                {
                    // Lose the fight, go back to the initial cell, lose 3 gongs of time
                    cout << "\nYou fight the " << gameState.dangerName[i] << " and lose ... Retreat." << endl;
                    gameState.everGladesMap[gameState.rangerRow][gameState.rangerCol] = 'R'; // Return to the previous cell
                    gameState.dangerCells[newRow][newCol] = true;  // Mark the danger in the cell ranger attempted to move to as revealed 
                    gameState.timeLeft -= 4;
                }
                break;
            default:
                cout << "Invalid action. Try again." << endl;
                handleDanger(gameState, newRow, newCol);
                break;
            }
        }
    }
}

void print_with_delay(const string& text, int delay) {
    for (char c : text) {
        cout << c;
        Sleep(delay);
    }
    cout << endl;
}

