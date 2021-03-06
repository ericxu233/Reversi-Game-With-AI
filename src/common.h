#pragma once
#include <iostream>
#include <unistd.h>
#include <climits>
#include "olcPGE/olcPixelGameEngine.h"
#include <thread>
#include <mutex>


#define SIZE 8

using namespace std;

extern int** board;

struct Piece {
    int x;
    int y;
};

class Score {
public:
    int score[2];

    Score() {
        score[0] = 0;
        score[1] = 0;
    }

    Score(const Score& cpy) {
        score[0] = cpy.score[0];
        score[1] = cpy.score[1];
    } 

    Score(int x, int y) {
        score[0] = x;
        score[1] = y;
    }

    void add_score(int player) {
        int index = player - 1;
        score[index] += 1;
    }

    int get_score(int player) {
        return score[player - 1];
    }
};

bool out_of_bounds(int x, int y);