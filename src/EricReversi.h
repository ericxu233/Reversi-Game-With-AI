#pragma once
#include "common.h"

class EricReversi : public olc::PixelGameEngine {
public:
    EricReversi() 
    {
        sAppName = "Eric Reversi";
    }

private:
    int height;
    int width;

    //testing only
    vector<int> pieceX;
    vector<int> pieceY; 

    //board specs
    int padding;
    int bstartX;
    int bendX;
    int bstartY;
    int bendY;
    int stride;
    int slen;
    int midX[8];
    int midY[8];

public:
    //core engine functions
    bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

private:
    //helper functions
    void board_dim_init();
    void move();

    //game drawing routine
    void draw_board();
    void draw_piece(int x, int y, int player);

    //testing only
    void add_piece(int x, int y);
};

