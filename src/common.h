#pragma once
#include <iostream>
#include <unistd.h>
#include "olcPGE/olcPixelGameEngine.h"


#define SIZE 8

using namespace std;

extern int** board;


struct Piece {
    int x;
    int y;
};

bool out_of_bounds(int x, int y);