#include "common.h"
#include "BoardController.h"

class ReversiAi {
public:
    ReversiAi();
    ReversiAi(int** target, Piece move, int AIplayer);
    ReversiAi(int** target, int AIplayer, BoardController* refcpy);

private:
    int** boardref;
    int player;
    BoardController con;

public:
    int determine_move(Piece move);
    //int minmax(int depth, int& alpha, int& beta, bool maximize);

private:
    int minmax(int depth, int& alpha, int& beta, bool maximize);
    int eval();

    //eval helper functions
    
};