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
    num_pieces = 4;
}

BoardController::BoardController(bool ai1, int** target) {
    boardref = target;

    ai = ai1;
    player = 1;
    end = false;
    winner = 0;
    num_pieces = 4;
}

BoardController::BoardController(BoardController* refcpy, int** target) {
    boardref = target;

    ai = refcpy->ai;
    ai = true;
    player = refcpy->player;
    end = refcpy->end;
    winner = refcpy->winner;
    num_pieces = refcpy->num_pieces;

    scores.push_back(refcpy->scores.back());
}

int BoardController::player_update(int x, int y) {
    bool match = false;
    vector<Piece>* playabback = playab.back();
    for (int i = 0; i < playabback->size(); i++) {
        if (x == (*playabback)[i].x && y == (*playabback)[i].y) match = true;
    }
    if (!match) return -1;
    if (end) return 0;
    boardref[x][y] = player;
    lastmove.push_back({x, y});
    if (ai) flip(x, y, true);
    else flip(x, y, false);
    player = (!(player - 1)) + 1;
    num_pieces++;

    if (num_pieces == SIZE*SIZE) end = true;
    if (end) {
        get_playable_pos();
        skip.push_back(false);
        return 0;
    }

    //artist<< "Reaches HEre" << endl;

    int ret = get_playable_pos();

    if (ret == 0) {
        player = (!(player - 1)) + 1;
        int ret = get_playable_pos();
        skip.push_back(true);
        if (ret == 0) {
            //artist<< "Reaches HEre Uh Oh" << endl;
            end = true;
            return 0;
        }
        //artistitic2<< "lastmove: " << lastmove.size() << " skip: " << skip.size() << " scores: " << scores.size() << endl;
        return 2;
    }
    skip.push_back(false);
    //artistitic2<< "lastmove: " << lastmove.size() << " skip: " << skip.size() << " scores: " << scores.size() << endl;
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

int BoardController::get_playable_pos() {
    vector<Piece>* tts = new vector<Piece>();
    tts->reserve(16);
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
                tts->push_back({i, j});
            }
        }
    }
    playab.push_back(tts);
    if (tts->empty()) return 0;
    return 1;
}

void BoardController::revert_move() {
    int col = player;
    if (skip.back()) {
        col = (!(player - 1)) + 1;
        playab.pop_back();
        playab.pop_back();
    }
    else {
        col = player;
        player = (!(player - 1)) + 1;
        playab.pop_back();
    }

    vector<Piece>* ref = flipped_content.back();

    //cout << ref->size() << endl;
    //cout << end << endl;
    //cout << endl;

    for (int i = 0; i < ref->size(); i++) {
        boardref[(*ref)[i].x][(*ref)[i].y] = col;
    }
    //artistitic2<< " yolo hhhhhhhh lastmove: " << lastmove.size() << " skip: " << skip.size() << " scores: " << scores.size() << endl;
    boardref[lastmove.back().x][lastmove.back().y] = 0;
    num_pieces--;

    lastmove.pop_back();
    skip.pop_back();
    scores.pop_back();
    flipped_content.pop_back();
    if (end) end = false;
    if (winner != 0) winner = 0;
}

bool BoardController::skipped() {
    if (skip.empty()) return false;
    return skip.back();
}

vector<Piece>* BoardController::get_playable() {
    return playab.back();
}

void BoardController::update_score() {
    cout << "note: deprecated do not use..." << endl;
    int score[3];
    score[0] = 0;
    score[1] = 0;
    score[2] = 0;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            score[boardref[i][j]] += 1;
        }
    }
    scores.push_back(Score(score[1], score[2]));
}

bool BoardController::progress() {
    return flipped_content.size() > 0;
}

void BoardController::flip(int x, int y, bool record) {
    //flipped_content = flip_ref;

    vector<Piece>* flipped = new vector<Piece>();
    flipped->reserve(20);

    if (scores.empty()) scores.push_back(Score());
    else scores.push_back(Score(scores.back()));

    scores.back().add_score(player);

    flipDIR(x, y, 1, 1, record, 1, flipped);
    flipDIR(x, y, 1, 0, record, 1, flipped);
    flipDIR(x, y, 1, -1, record, 1, flipped);
    flipDIR(x, y, 0, 1, record, 1, flipped);
    flipDIR(x, y, 0, -1, record, 1, flipped);
    flipDIR(x, y, -1, 1, record, 1, flipped);
    flipDIR(x, y, -1, 0, record, 1, flipped);
    flipDIR(x, y, -1, -1, record, 1, flipped);

    flipped_content.push_back(flipped);
    //flipped_content = NULL;
}

bool BoardController::flipDIR(int x, int y, int deltaX, int deltaY, bool record, int iter, vector<Piece>* flip_ref) {
    if (iter == 1) {
        x += deltaX;
        y += deltaY;
    }
    if (out_of_bounds(x, y) || boardref[x][y] == 0) return false;
    if (iter == 1 && boardref[x][y] == player) return false;
    if (boardref[x][y] == player) return true;
    bool res = flipDIR(x + deltaX, y + deltaY, deltaX, deltaY, record, iter + 1, flip_ref);

    if (res) {
        boardref[x][y] = player;
        scores.back().add_score(player);
        if (record) flip_ref->push_back({x, y});
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