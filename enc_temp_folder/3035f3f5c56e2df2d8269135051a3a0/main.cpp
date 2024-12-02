#include <cctype>
#include <iostream>
#include <cstdlib>
#include <array> //this is for C++-style arrays
#include <cmath> //sqrt
#include <algorithm> //std::max
#include <string>
#include <vector>

struct BoardPosition {
    int row = -1;
    int col = -1;
};


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

    std::string toEmoji() { //this doesn't work in the console display, but it's here for possible future versions
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

};

class GameBoard {
    int boardSize = -1;
    int minWinTurns = 0; //the minimum turns to win is the square root of the size
    int turn = 0;
    std::array<std::array<Space, 26>, 26> spaces; //maximum size always reserved

public:
    void show() {
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
    }

    std::string getHorizontalLine(int length) {
        std::string line = "--";
        for (int i = 0; i < length; i++) {
            line.append("+---");
        }
        line.append("+--\n");
        return line;
    }

    GameBoard(int size) { //constructor for GameBoard
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

    Space& getSpace(BoardPosition pos) {
        return spaces[pos.row][pos.col];
    }
    
    Space& getSpace(int row, int col) {
        return spaces[row][col];
    }

    Space& getSpace(std::string columnRow) {
        BoardPosition pos = this->columnRowToBoardPos(columnRow);
        return getSpace(pos);
    }

    char checkWinner() {
        if (turn < minWinTurns) {
            return ' ';
        }
        else
        {
            char ret = ' ';
            bool stop = false;
            int consecutiveSpaces = 0;
            char players[] = { 'R', 'H' };
            for (char p : players) { //we want to try for each plater
                //horizontal (across row) win condition
                for (int col = 0; col < boardSize; col++) {
                    for (int row = 0; row < boardSize; row++) {
                        Space s = getSpace(col, row);
                        if (s.getFill() == p) {
                            consecutiveSpaces++;
                            std::cout << "h: consecutiveSpaces: " << consecutiveSpaces << " / boardSize: " << boardSize << " " << std::endl;
                            if (consecutiveSpaces == boardSize) {
                                ret = p; //set the winner
                                stop = true;
                            }
                            continue;
                        }
                        else {
                            consecutiveSpaces = 0;
                            stop = true;
                        }

                        if (stop) {
                            stop = false; //stop is also used for col loop
                            break;
                        }
                        
                    }


                    if (stop || ret != ' ') { //if ret has been set, then we already know someone won
                        stop = false;
                        break;
                    }
                }
                consecutiveSpaces = 0;
                //vertical (up/down column) win condition
                if (ret == ' ') { //only check vertical win condition if horizontal one didn't return anything
                    for (int row = 0; row < boardSize; row++) {
                        for (int col = 0; col < boardSize; col++) {
                            Space s = getSpace(col, row);
                            if (s.getFill() == p) {
                                consecutiveSpaces++;
                                std::cout << "v: consecutiveSpaces: " << consecutiveSpaces << " / boardSize: " << boardSize << " " << std::endl;
                                if (consecutiveSpaces == boardSize) {
                                    ret = p; //set the winner
                                    stop = true;
                                }
                                continue;
                            }
                            else {
                                consecutiveSpaces = 0;
                                stop = true;
                            }

                            if (stop) {
                                stop = false; //stop is also used for col loop
                                break;
                            }
                        }
                    }

                    if (stop || ret != ' ') { //if ret has been set, then we already know someone won
                        stop = false;
                        break;
                    }
                }


                //diagonal (top left->bottom right) win condition
                if (ret == ' ') { //only check diagonal win condition if previous two didn't return anything

                }


                //diagonal (top right->bottom left)
                if (ret == ' ') { //only check diagonal win condition if previous two didn't return anything

                }
                
            }

            







            return ret;
        }

    }

    int nextTurn() {
        turn++;
        return turn;
    }

    static char IntToChar(int i) {
        return (char)i + 65; //65 is where the capital alphabet starts in ASCII
    }

    BoardPosition columnRowToBoardPos(std::string columnRow) { //converts a string into a (valid) board position
        //std::cout << "columnRow: " << columnRow << " " << std::endl;
        BoardPosition ret;

        if (columnRow.length() == 0) { //don't bother with converting if they didn't input anything
            ret.col = -1;
            ret.row = -1;
            return ret;
        }

        char colChar = ' ';
        if (std::isalpha(columnRow[0])) {
            colChar = std::toupper(columnRow[0]);
        }
        else
        {
            ret.col = -1; //return a new space that's not part of the board
            //this will be checked for in the main game loop
        }
        int col = colChar - 65; //convert back to int from ascii char
        if (col < boardSize) {
            ret.col = col;
        }

        std::string rowString = columnRow.substr(1, columnRow.length()); //from substring
        int row = -1;
        try {
            row = std::stoi(rowString); //convert the substring into an integer
            row = row - 1; //the displayed board is 1-indexed, but the board internally is 0-indexed
        }
        catch (const std::exception& e) {
            // if stoi fails
            std::cout << "ERR: Invalid Row: " << rowString << std::endl;
        }

        if (row < boardSize) {
            ret.row = row;
        }

        return ret;
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
        std::string boardSizeIn;
        std::getline(std::cin, boardSizeIn);

        try {
            boardSize = stoi(boardSizeIn);
            if (boardSize > MAX_BOARD_SIZE || boardSize < MIN_BOARD_SIZE) {
                std::cout << "Board size must be in between " << MIN_BOARD_SIZE << " and " << MAX_BOARD_SIZE << ". Please try again. ";
            }
        }
        catch (const std::exception& e) {
            // if stoi fails
            std::cout << "ERR: There was an error with your input, please try again. ";
        }
    }

    GameBoard g = GameBoard(boardSize);

    std::cout << "Will Player 1 be Cheese (H) or Crackers (R)? " << std::endl;
    char player1choice = ' ';
    char winner = ' ';
    while (player1choice != 'H' && player1choice != 'R') {
        //std::cout << "player1choice: " << player1choice << " " << std::endl;
        try {
            std::cin >> player1choice;
            player1choice = toupper(player1choice);
            if (player1choice == 'H') {
                //we already set player1 to H
                std::cout << "Player 2 will be Crackers (R)." << std::endl;
            }
            else if (player1choice == 'R') {
                player1 = 'R';
                player2 = 'H';
                std::cout << "Player 2 will be Cheese (H)." << std::endl;
            }
            else {
                std::cout << "That was not an option, try again.";
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "ERR: could not parse input, please try again.";
        }
    }
    

    std::cout << "Begin!" << std::endl;
    std::cin.ignore();

    //main game loop
    while (winner == ' ') {
        g.show();
        std::string move;
        int playerNum = player1Goes ? 1 : 2;
        std::cout << "Player " << playerNum << ", where will you place your " << player1 << "?" << std::endl;
        std::cout << "Use column-row notation, e.g. B2, or input anything else to pass your turn" << std::endl;
        std::getline(std::cin, move);
        //std::cout << "move: " << move << std::endl;
        BoardPosition pos = g.columnRowToBoardPos(move);
        //std::cout << "pos: " << pos.col << " " << pos.row << std::endl;
        if (pos.col == -1 || pos.row == -1) {
            std::cout << "Turn passed." << std::endl;
        }
        else {
            Space& s = g.getSpace(move);

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
        }

        player1Goes = !player1Goes; //invert turn
        winner = g.checkWinner();
        if (winner != ' ') {
            g.show();
        }
        else {
            g.nextTurn();
        }
        
    }

    //if the loop is done then there was a winner
    std::cout << winner << " wins!";


}