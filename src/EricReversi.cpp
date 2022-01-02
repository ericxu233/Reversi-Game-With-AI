#include "EricReversi.h"

bool EricReversi::OnUserCreate()
{
    // Called once at the start, so create things here
    height = ScreenHeight();
    width = ScreenWidth();
    cout << height << " " << width << endl;
	return true;
}

bool EricReversi::OnUserUpdate(float fElapsedTime)
{
    // called once per frame
	Clear(olc::Pixel(101, 229, 227));
    //usleep(10000);
	return true;
}

void EricReversi::draw_board() {
    olc::Pixel col(3, 82, 81);
    
}