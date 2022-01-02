#include "common.h"
#include "EricReversi.h"

int main() {
    EricReversi Game;

    if (Game.Construct(512, 512, 2, 2))
		Game.Start();


    return 0;
}