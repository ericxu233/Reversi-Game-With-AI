#include "common.h"
#include "AI.h"


ReversiAi::ReversiAi() {

}

ReversiAi::ReversiAi(int** target, Piece move, int AIplayer) {
    boardref = target;
    player = AIplayer;
    boardref[move.x][move.y] = (!(player - 1)) + 1;
    con = BoardController(true, boardref);
    con.update_score();
    con.get_playable_pos();
}

ReversiAi::ReversiAi(int** target, int AIplayer, BoardController* refcpy) {
    boardref = target;
    player = AIplayer;
    con = BoardController(refcpy, target);
    //con.update_score();
    con.get_playable_pos();
}

//int debug = 0;

int ReversiAi::determine_move(Piece move) {
    vector<Piece> revert;
    int ret1 = con.player_update(move.x, move.y, &revert);
    
    /*
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << boardref[i][j];
        }
        cout << endl;
    }
    cout << endl;
    */
    

    //artist<< "update1 " << debug++ << " return code is " << ret1 << endl; 
    int a = INT_MIN;
    int b = INT_MAX;
    //sleep(1);
    int ret = minmax(64, a, b, false);
    //artist<< "Reach here..." << endl;
    con.revert_move(&revert);
    //artist<< "Reach here..." << endl;

    /*
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << boardref[i][j];
        }
        cout << endl;
    }
    cout << endl;
    */
    


    return ret;
}

int ReversiAi::minmax(int depth, int& alpha, int& beta, bool maximize) {
    if (depth == 0 || con.get_winner() != 0) {
        cout << "depth is " << depth << endl;
        cout << "winner is " << con.get_winner();
        return eval();
    }

    if (con.skipped()) {
        cout << "skip" << endl;
        maximize = !maximize;
    }

    if (maximize) {
        int maxEval = INT_MIN;
        vector<Piece>* cpyPlayable = con.get_playable();
        for (int i = 0; i < cpyPlayable->size(); i++) {
            vector<Piece> revert;
            revert.reserve(16);
            int ret = con.player_update((*cpyPlayable)[i].x, (*cpyPlayable)[i].y, &revert);
            //artistitic2<< "update2 " << debug++  << " depth is " << depth << " return code is " << ret << " loop size is " << cpyPlayable->size() << endl; 
            int eval = minmax(depth - 1, alpha, beta, false);
            con.revert_move(&revert);
            //artistitic2<< "depth is " << depth << endl;
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break; 
        }
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        vector<Piece>* cpyPlayable = con.get_playable();
        for (int i = 0; i < cpyPlayable->size(); i++) {
            vector<Piece> revert;
            revert.reserve(16);
            int ret = con.player_update((*cpyPlayable)[i].x, (*cpyPlayable)[i].y, &revert);
            //artistitic2<< "update3 " << debug++  << " depth is " << depth << " return code is " << ret << " loop size is " << cpyPlayable->size() << endl; 
            int eval = minmax(depth - 1, alpha, beta, true);
            con.revert_move(&revert);
            //artistitic2<< "depth is " << depth << endl;
            beta = min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

int ReversiAi::eval() {
    int player_score = con.scores.back().get_score(player);
    int oppon_score = con.scores.back().get_score((!(player - 1)) + 1);

    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << boardref[i][j];
        }
        cout << endl;
    }
    cout << endl;
    
    

    if (player_score - oppon_score < -64 || player_score - oppon_score > 64) cout << "Alarming..." << endl; 

    return player_score - oppon_score;
}

