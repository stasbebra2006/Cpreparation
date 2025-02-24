#ifndef __TRAINER__
#include <iostream>
#include <iomanip>

struct DominoTile {
    int pips[2];
};
#endif

bool readDominoTile(DominoTile & tile) {
	char leftBracket, line, rightBracket;
	std::cin >> leftBracket >> tile.pips[0] >> line >> tile.pips[1] >> rightBracket;
	if(std::cin.fail() || leftBracket != '[' || line != '|' || rightBracket != ']')
	{
		return false;
	}
	return true;
}

#ifndef __TRAINER__
int main () {
    DominoTile tile;
    if(!readDominoTile(tile)) {
        std::cout << "Nespravny vstup." << std::endl;
        return 1;
    }
    std::cout << '[' << tile.pips[0] << '|' << tile.pips[1] << ']' << std::endl;
    return 0;
}
#endif