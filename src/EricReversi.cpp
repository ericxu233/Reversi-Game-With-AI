#include "EricReversi.h"

bool EricReversi::OnUserCreate()
{
    // Called once at the start, so create things here
    height = ScreenHeight();
    width = ScreenWidth();
    board_dim_init();
    cout << height << " " << width << endl;
	return true;
}

bool EricReversi::OnUserUpdate(float fElapsedTime)
{
    // called once per frame
	Clear(olc::Pixel(101, 229, 227));
    draw_board();
    move();
    //usleep(10000);
    for (int i = 0; i < pieceX.size(); i++) {
        draw_piece(pieceX[i], pieceY[i], 2);
    }
	return true;
}

void EricReversi::board_dim_init() {
    padding = 24;
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

        add_piece(px, py);
        //if ()
    }
}

void EricReversi::draw_piece(int x, int y, int player) {
    if(player == 1) FillCircle(midX[x], midY[y], slen*0.8, olc::WHITE);
    else if(player == 2) FillCircle(midX[x], midY[y], slen*0.4, olc::BLACK);
}

void EricReversi::add_piece(int x, int y) {
    pieceX.push_back(x);
    pieceY.push_back(y);
}