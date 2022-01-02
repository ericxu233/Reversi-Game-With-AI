#pragma once
#include "common.h"

class EricReversi : public olc::PixelGameEngine {
public:
    EricReversi() 
    {
        sAppName = "Eric Reversi";
    }

public:
    int height;
    int width;

public:
    //core engine functions
    bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

private:
    //helper functions
    void draw_board();
};

