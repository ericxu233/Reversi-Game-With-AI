#pragma once
#include "common.h"

class BoardController {
private:
    bool ai;
    int player;
    bool end;
    int winner;
    int** boardref;
    vector<Piece>* flipped_content;
    int num_pieces;
    list<bool> skip;

public:
    list<vector<Piece>*> playab;
    list<Piece> lastmove;
    list<Score> scores;

public:
    BoardController();
    BoardController(bool ai, int** target);
    BoardController(BoardController* refcpy, int** target);

    int player_update(int x, int y, vector<Piece>* ref);
    int get_playable_pos();
    int get_winner();
    int get_player();
    void revert_move(vector<Piece>* ref);
    bool skipped();
    vector<Piece>* get_playable();
    void update_score();


private:
    void flip(int x, int y, bool record, vector<Piece>* flip_ref);
    bool flipDIR(int x, int y, int deltaX, int deltaY, bool record, int iter);
    bool gppDIR(int x, int y, int deltaX, int deltaY);

};