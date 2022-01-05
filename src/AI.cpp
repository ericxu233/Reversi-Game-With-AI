#include "common.h"
#include "AI.h"


ReversiAi::ReversiAi() {

}

ReversiAi::ReversiAi(int** target, Piece move, int AIplayer) {
    boardref = target;
    player = AIplayer;
    boardref[move.x][move.y] = (!(player - 1)) + 1;
    con = BoardController(true, boardref);
    con.get_playable_pos();
}

ReversiAi::ReversiAi(int** target, int AIplayer) {
    boardref = target;
    player = AIplayer;
    con = BoardController(true, boardref);
    con.get_playable_pos();
}

int ReversiAi::determine_move(Piece move) {
    vector<Piece> revert;
    con.player_update(move.x, move.y, &revert);
    int a = INT_MIN;
    int b = INT_MAX;
    int ret = minmax(4, a, b, false);
    con.revert_move(&revert);
    return ret;
}

int ReversiAi::minmax(int depth, int& alpha, int& beta, bool maximize) {
    if (depth == 0 || con.get_winner() != 0) {
        return eval();
    }

    if (con.skipped()) return minmax(depth, alpha, beta, !maximize);

    if (maximize) {
        int maxEval = INT_MIN;
        vector<Piece> cpyPlayable = con.playable;
        for (int i = 0; i < cpyPlayable.size(); i++) {
            vector<Piece> revert;
            revert.reserve(16);
            con.player_update(cpyPlayable[i].x, cpyPlayable[i].y, &revert);
            int eval = minmax(depth - 1, alpha, beta, false);
            con.revert_move(&revert);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break; 
        }
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        vector<Piece> cpyPlayable = con.playable;
        for (int i = 0; i < cpyPlayable.size(); i++) {
            vector<Piece> revert;
            revert.reserve(16);
            con.player_update(cpyPlayable[i].x, cpyPlayable[i].y, &revert);
            int eval = minmax(depth - 1, alpha, beta, false);
            con.revert_move(&revert);
            beta = min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

int ReversiAi::eval() {
    return con.scores.back().get_score(player) - con.scores.back().get_score((!(player - 1)) + 1);
}

