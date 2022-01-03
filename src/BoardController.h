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

public:
    vector<Piece> playable;

public:
    BoardController();
    BoardController(bool ai, int** target);

    bool player_update(int x, int y);
    void get_playable_pos();
    int get_winner();
    int get_player();


private:
    void flip(int x, int y, int player, bool record, vector<Piece>* flip_ref);
    bool flipDIR(int x, int y, int deltaX, int deltaY, int player, bool record, int iter);
    bool gppDIR(int x, int y, int deltaX, int deltaY);
};