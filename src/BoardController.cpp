#include "BoardController.h"

int** board;


//common.h function, maybe will consider the conten to be moved to a common.cpp file
bool out_of_bounds(int x, int y) {
    if (x < 0 || x > SIZE - 1 || y < 0 || y > SIZE - 1) return true;
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

    ai = false;
    player = 1;
    end = false;
    winner = 0;
    playable = vector<Piece>();
    playable.reserve(16);
    flipped_content = NULL;
    num_pieces = 4;
}

int BoardController::player_update(int x, int y, vector<Piece>* ref) {
    bool match = false;
    for (int i = 0; i < playable.size(); i++) {
        if (x == playable[i].x && y == playable[i].y) match = true;
    }
    if (!match) return -1;
    if (end) return 0;
    playable.clear();
    boardref[x][y] = player;
    lastmove.push_back({x, y});
    if (ai) flip(x, y, true, ref);
    else flip(x, y, false, ref);
    player = (!(player - 1)) + 1;
    num_pieces++;

    if (num_pieces == SIZE*SIZE) end = true;
    if (end) return 0;

    //cout << "Reaches HEre" << endl;

    get_playable_pos();

    if (playable.empty()) {
        player = (!(player - 1)) + 1;
        get_playable_pos();
        skip.push_back(true);
        if (playable.empty()) {
            //cout << "Reaches HEre Uh Oh" << endl;
            end = true;
            return 0;
        }
        return 2;
    }
    skip.push_back(false);
    return 1;
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
    playable.clear();
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

void BoardController::revert_move(vector<Piece>* ref) {
    int col = player;
    if (skip.back()) col = player = (!(player - 1)) + 1;

    for (int i = 0; i < ref->size(); i++) {
        boardref[(*ref)[i].x][(*ref)[i].y] = col;
    }
    boardref[lastmove.back().x][lastmove.back().y] = 0;

    lastmove.pop_back();
    skip.pop_back();
    scores.pop_back();
    if (end) end = false;
    if (winner != 0) winner = 0;
}

bool BoardController::skipped() {
    if (skip.empty()) return false;
    return skip.back();
}

void BoardController::flip(int x, int y, bool record, vector<Piece>* flip_ref = NULL) {
    flipped_content = flip_ref;

    if (scores.empty()) scores.push_back(Score());
    else scores.push_back(Score(scores.back()));

    scores.back().add_score(player);

    flipDIR(x, y, 1, 1, record, 1);
    flipDIR(x, y, 1, 0, record, 1);
    flipDIR(x, y, 1, -1, record, 1);
    flipDIR(x, y, 0, 1, record, 1);
    flipDIR(x, y, 0, -1, record, 1);
    flipDIR(x, y, -1, 1, record, 1);
    flipDIR(x, y, -1, 0, record, 1);
    flipDIR(x, y, -1, -1, record, 1);

    flipped_content = NULL;
}

bool BoardController::flipDIR(int x, int y, int deltaX, int deltaY, bool record, int iter) {
    if (iter == 1) {
        x += deltaX;
        y += deltaY;
    }
    if (out_of_bounds(x, y) || boardref[x][y] == 0) return false;
    if (iter == 1 && boardref[x][y] == player) return false;
    if (boardref[x][y] == player) return true;
    bool res = flipDIR(x + deltaX, y + deltaY, deltaX, deltaY, record, iter + 1);

    if (res) {
        boardref[x][y] = player;
        scores.back().add_score(player);
        if (record && flipped_content != NULL) flipped_content->push_back({x, y});
    }

    return res;
}

bool BoardController::gppDIR(int x, int y, int deltaX, int deltaY) {
    int iter = 1;
    while(!out_of_bounds(x + iter*deltaX, y + iter*deltaY)) {
        int tempx = x + iter*deltaX;
        int tempy = y + iter* deltaY;
        if (iter == 1 && boardref[tempx][tempy] == player) return false;
        else if(boardref[tempx][tempy] == 0) return false;
        else if(boardref[tempx][tempy] == player) return true;
        iter++;
    }
    return false;
}