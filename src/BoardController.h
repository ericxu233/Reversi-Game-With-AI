#pragma once
#include "common.h"

class BoardController {
private:
    bool ai;
    int player;
    bool end;
    int winner;
    int** boardref;
    //vector<Piece>* flipped_content;
    int num_pieces;
    list<bool> skip;

public:
    list<vector<Piece>*> playab;
    list<Piece> lastmove;
    list<Score> scores;
    list<vector<Piece>*> flipped_content;

public:
    BoardController();
    BoardController(bool ai, int** target);
    BoardController(BoardController* refcpy, int** target);

    int player_update(int x, int y);
    int get_playable_pos();
    int get_winner();
    int get_player();
    void revert_move();
    bool skipped();
    vector<Piece>* get_playable();
    void update_score(); //note deprecated do not use....
    bool progress();



private:
    void flip(int x, int y, bool record);
    bool flipDIR(int x, int y, int deltaX, int deltaY, bool record, int iter, vector<Piece>* flip_ref);
    bool gppDIR(int x, int y, int deltaX, int deltaY);

};