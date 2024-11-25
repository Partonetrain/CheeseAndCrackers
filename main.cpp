#include <cctype>
#include <iostream>
#include <cstdlib>
#include <array> //this is for C++-style arrays
#include <cmath> //sqrt
#include <algorithm> //std::max
#include <string>

class Space {
private:
    char fill = ' '; //empty space by default

public:
    char getFill() {
        return fill;
    }

    bool isEmpty() {
        return fill == ' ';
    }

    bool isCheese() {
        return fill == 'H';
    }

    bool isCracker() {
        return fill == 'R';
    }

    std::string toEmoji() { //no idea if this will work
        if (this->isCheese()) {
            return "🧀";
        }
        else if (this->isCracker()) {
            return "🍘";
        }
        else {
            return " ";
        }
    }

    void setFill(char fill) {
        this->fill = fill;
    }

    void setCheese() {
        fill = 'H';
    }

    void setCracker() {
        fill = 'R';
    }
};

class GameBoard {
    int boardSize = -1;
    int minWinTurns = 0; //the minimum turns to win is the square root of the size
    int turn = 0;
    std::array<std::array<Space, 26>, 26> spaces; //maximum size always reserved

public:
    std::string show() {
        std::cout << "Here's the board on turn " << turn + 1 << ": " << std::endl;
        //top of board with addresses (letters across the top)
        std::cout << "  ";
        for (int colLabel = 0; colLabel < boardSize; colLabel++) {
            std::cout << "  " << IntToChar(colLabel) << " ";
        }
        std::cout << std::endl;

        std::cout << getHorizontalLine(boardSize);
        for (int row = 0; row < boardSize; row++) {
            //numbered row labels
            std::string rowLabel = std::to_string(row + 1);
            if (rowLabel.length() == 1) {
                rowLabel = " " + rowLabel;
            }
            //now print the actual row
            std::cout << rowLabel << "|";
            for (int col = 0; col < boardSize; col++) {
                //spaces[row][col].setCheese(); //used to test layout
                std::cout << " " << spaces[row][col].getFill() << " |";
            }
            std::cout << std::endl;
            //horizontal lines
            std::cout << getHorizontalLine(boardSize);
        }
        return "";
    }

    std::string getHorizontalLine(int length) {
        std::string line = "--";
        for (int i = 0; i <= length; i++) {
            line.append("+---");
        }
        line.append("\n");
        return line;
    }

    GameBoard(int size) {
        boardSize = size;
        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                Space s = Space(); //construct a new empty space
                spaces[col][row] = s;
                //std::cout << "row: " << row << " col: " << col << std::endl;
            }

        }
        minWinTurns = std::round(sqrt(boardSize));
        minWinTurns = std::max(3, minWinTurns); //hard minimum of 3 turns before checking who won
        //std::cout << "Min Win Turns: " << minWinTurns << std::endl;
    }

    Space getSpace(int row, int col) {
        return spaces[row][col];
    }

    Space getSpace(std::string columnRow) {
        //first parse the column row
        char colChar;
        if (std::isalpha(columnRow[0])) {
            colChar = std::toupper(columnRow[0]);
        }
        else
        {
            return Space(); //return a new space that's not part of the board
            //we are essentially throwing away the input
            std::cout << "Turn passed." << std::endl;
        }
        int col = colChar - 65; //convert back to int from ascii char
        if (col < boardSize) {
            return Space();
            std::cout << "Turn passed." << std::endl;
        }

        //TODO trycatch here?
        std::string rowNumbers = columnRow.substr(1, columnRow.length()); //from substring
        int row = std::stoi(rowNumbers); //string to int

        if (row < boardSize) {
            return Space();
            std::cout << "Turn passed." << std::endl;
        }

        return getSpace(col, row);
    }

    char checkWinner() { //TODO
        if (turn < minWinTurns) {
            return ' ';
        }
        else
        {
            return ' ';
        }

    }

    int nextTurn() {
        turn++;
        return turn;
    }

    static char IntToChar(int i) {
        return (char)i + 65; //65 is where the capital alphabet starts in ASCII
    }

};

int main() {
    int boardSize = -1;
    const int MAX_BOARD_SIZE = 26;
    const int MIN_BOARD_SIZE = 3;
    char player1 = 'H'; //player1 is cheese by default
    char player2 = 'R'; //player2 is crackers by default
    bool player1Goes = true;

    std::cout << "Welcome to Cheese and Crackers!" << std::endl;
    std::cout << "How big should the game board be? (Max: " << MAX_BOARD_SIZE << ") ";

    while (boardSize > MAX_BOARD_SIZE || boardSize < MIN_BOARD_SIZE) {
        //TODO what if the user input a string?
        std::cin >> boardSize;
        if (boardSize > MAX_BOARD_SIZE || boardSize < MIN_BOARD_SIZE) {
            std::cout << "Please try again. ";
        }
    }
    std::cin.ignore();

    GameBoard g = GameBoard(boardSize);

    std::cout << "Will Player 1 be Cheese (H) or Crackers (R)? " << std::endl;
    char player1choice;
    std::cin >> player1choice;
    player1choice = toupper(player1choice);
    if (player1choice == 'H') {
        //we already set player1 to H
        std::cout << "Player 2 will be Crackers (R)." << std::endl;
    }
    else if (player1choice == 'H') {
        player1 = 'R';
        player2 = 'H';
        std::cout << "Player 2 will be Cheese (H)." << std::endl;
    }
    else {
        std::cout << "Try again";
        //TODO handle not choosing player correctly
    }

    std::cout << "Begin!" << std::endl;
    std::cout << g.show();

    //main game loop
    while (g.checkWinner() == ' ') {
        std::string move;
        int playerNum = player1Goes ? 1 : 2;
        std::cout << "Player " << playerNum << ", where will you place your " << player1 << "?" << std::endl;
        std::cout << "Use column-row notation, e.g. B2, or input anything else to pass your turn" << std::endl;
        std::getline(std::cin, move);
        std::cin.ignore();
        Space s = g.getSpace(move);

        if (s.isEmpty()) {
            if (player1Goes) {
                s.setFill(player1);
            }
            else {
                s.setFill(player2);
            }
        }
        else {
            std::cout << "You input a valid space, but it was already filled. Passing turn." << std::endl;
        }
        std::cout << g.show();


        player1Goes = !player1Goes; //invert turn
        g.nextTurn();
    }


}