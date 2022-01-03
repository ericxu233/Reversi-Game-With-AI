#include "EricReversi.h"

bool EricReversi::OnUserCreate()
{
    // Called once at the start, so create things here
    height = ScreenHeight();
    width = ScreenWidth();
    board_dim_init();
    cout << height << " " << width << endl;

    board = new int*[SIZE];
    for (int i = 0; i < SIZE; i++) {
        board[i] = new int[SIZE];
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    
    board[3][3] = 1;
    board[4][3] = 2;
    board[4][4] = 1;
    board[3][4] = 2;

    b_con = new BoardController(false, board);

    b_con->get_playable_pos();

    cout << "Reach Here?" << endl;

	return true;
}

bool EricReversi::OnUserUpdate(float fElapsedTime)
{
    // called once per frame
	Clear(olc::Pixel(101, 229, 227));
    draw_board();
    move();
    if (b_con->get_winner() != 0) draw_winner();
    //usleep(10000);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != 0) draw_piece(i, j, b_con->get_player());
        }
    }
    
    for (int i = 0; i < b_con->playable.size(); i++) {
        FillCircle(midX[b_con->playable[i].x], midY[b_con->playable[i].y], slen*0.1, olc::RED);
    }

	return true;
}

void EricReversi::board_dim_init() {
    padding = 36;
    bstartX = padding - 2;
    bstartY = padding - 2;

    bendX = width - padding*2 + bstartX;
    bendY = width - padding*2 + bstartY;

    stride = (width - padding*2)/8;
    slen = stride - 1;

    for (int i = 0; i < 8; i++) {
        midX[i] = bstartX + i*stride + slen/2 + 1;
        midY[i] = bstartY + i*stride + slen/2 + 1;
    }
}

void EricReversi::draw_board() {
    olc::Pixel col(3, 82, 81);

    for (int i = 0; i < 9; i++) {
        DrawLine(bstartX, bstartY + i*stride, bendX, bstartY + i*stride, col);
        DrawLine(bstartX + i*stride, bstartY, bstartX + i*stride, bendY, col);
    }
}

void EricReversi::move() {
    auto click = GetMouse(0);
    if (IsFocused() && click.bPressed) {
        int mx = GetMouseX();
        int my = GetMouseY();

        if (mx < bstartX || mx > bendX || my < bstartY || my > bendY) return;

        int px = (mx - bstartX)/stride;
        int py = (my - bstartY)/stride;

        b_con->player_update(px, py);
    }
}

void EricReversi::draw_piece(int x, int y, int player) {
    if(player == 1) FillCircle(midX[x], midY[y], slen*0.4, olc::BLACK);
    else if(player == 2) FillCircle(midX[x], midY[y], slen*0.4, olc::WHITE);
}

void EricReversi::draw_winner() {
    int scale = 3;
    int winner = b_con->get_winner();
    string message;
    if (winner == 3) message = "It is a draw!";
    else if (winner == 1) message = "Player 1 has won!";
    else if (winner == 2) message = "Player 2 has won!";

    if (winner == 1 || winner == 3) DrawString(width/2 - message.length()*scale*8/2, 12, message, olc::BLACK, scale);
    else if (winner == 2) DrawString(width/2 - message.length()*scale*8/2, 12, message, olc::WHITE, scale);
}


void EricReversi::add_piece(int x, int y) {
    pieceX.push_back(x);
    pieceY.push_back(y);
}