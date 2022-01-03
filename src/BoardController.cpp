#include "BoardController.h"

int** board;


//common.h function, maybe will consider the conten to be moved to a common.cpp file
bool out_of_bounds(int x, int y) {
    if (x < 0 || x > SIZE || y < 0 || y > SIZE) return true;
    else return false;
}


BoardController::BoardController() {
    boardref = board;

    ai = false;
    player = 1;
    end = false;
    winner = 0;
    playable = vector<Piece>();
    playable.reserve(16);
    flipped_content = NULL;
    num_pieces = 4;
}

BoardController::BoardController(bool ai1, int** target) {
    boardref = target;

    ai = ai1;
    player = 1;
    end = false;
    winner = 0;
    playable = vector<Piece>();
    playable.reserve(16);
    flipped_content = NULL;
    num_pieces = 4;
}

bool BoardController::player_update(int x, int y) {
    if (end) return true;
    playable.clear();
    boardref[x][y] = player;
    player = (!(player - 1)) + 1;
    num_pieces++;

    if (num_pieces == SIZE*SIZE) end = true;
    if (end) return true;

    get_playable_pos();

    if (playable.empty()) {
        player = (!(player - 1)) + 1;
        get_playable_pos();
        if (playable.empty()) {
            end = true;
            return true;
        }
    }
    return false;
}

int BoardController::get_winner() {
    if (winner != 0) return winner;
    if (!end) return 0;

    int score[3];
    score[0] = 0;
    score[1] = 0;
    score[2] = 0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            score[boardref[i][j]] += 1;
        }
    }
    winner = (score[1] < score[2]) + 1;
    if (score[1] == score[2]) winner = 3;

    return winner;
}

int BoardController::get_player() {
    return player;
}

void BoardController::get_playable_pos() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != 0) continue;
            bool playable_ind = gppDIR(i, j, 1, 1) ||
                            gppDIR(i, j, 1, 0) ||
                            gppDIR(i, j, 1, -1) ||
                            gppDIR(i, j, 0, 1) ||
                            gppDIR(i, j, 0, -1) ||
                            gppDIR(i, j, -1, 1) ||
                            gppDIR(i, j, -1, 0) ||
                            gppDIR(i, j, -1, -1);
            if (playable_ind) {
                playable.push_back({i, j});
            }
        }
    }
}

void BoardController::flip(int x, int y, int player, bool record, vector<Piece>* flip_ref = NULL) {
    flipped_content = flip_ref;

    flipDIR(x, y, 1, 1, player, record, 1);
    flipDIR(x, y, 1, 0, player, record, 1);
    flipDIR(x, y, 1, -1, player, record, 1);
    flipDIR(x, y, 0, 1, player, record, 1);
    flipDIR(x, y, 0, -1, player, record, 1);
    flipDIR(x, y, -1, 1, player, record, 1);
    flipDIR(x, y, -1, 0, player, record, 1);
    flipDIR(x, y, -1, -1, player, record, 1);

    flipped_content = NULL;
}

bool BoardController::flipDIR(int x, int y, int deltaX, int deltaY, int player, bool record, int iter) {
    if (out_of_bounds(x, y) || boardref[x][y] == 0) return false;
    if (iter == 1 && boardref[x][y] == player) return false;
    if (boardref[x][y] == player) return true;
    bool res = flipDIR(x + deltaX, y + deltaY, deltaX, deltaY, player, record, iter + 1);

    if (res) {
        boardref[x][y] = player;
        if (record && flipped_content != NULL) flipped_content->push_back({x, y});
    }

    return res;
}

bool BoardController::gppDIR(int x, int y, int deltaX, int deltaY) {
    int iter = 1;
    int tempx = x;
    int tempy = y;
    while(!out_of_bounds(tempx + iter*deltaX, tempy + iter*deltaY)) {
        tempx = tempx + iter*deltaX;
        tempy = tempx + iter* deltaY;
        if (iter == 1 && boardref[tempx][tempy] == player) return false;
        else if(boardref[tempx][tempy] == 0) return false;
        else if(boardref[tempx][tempy] == player) return true;
        iter++;
    }
    return false;
}