#include <iostream>
#include <stdint.h>

// The four orientations correspond to a rotation of 0°, 90°, 180° and 270° clockwise
enum Orientation {
    ONE, TWO, THREE, FOUR
};

// The possible sides a piece can have
enum Side : int_fast8_t{
    BOOBS_IN = -1, BOOBS_OUT = 1, RECT_IN = -2, RECT_OUT = 2, TRIANGLE_IN = -3, TRIANGLE_OUT = 3, TRAPEZIUM_IN = -4, TRAPEZIUM_OUT = 4,
};

struct Piece {
    uint_fast8_t pieceNum; // needed for later matching the program output to real puzzle pieces
    bool isUsed;
    const Side sides[4];
    Orientation orientation;
    Piece(uint_fast8_t pieceNum, Side sideOne, Side sideTwo, Side sideThree, Side sideFour) : isUsed(false), pieceNum(pieceNum),sides{sideOne, sideTwo, sideThree, sideFour}, orientation(ONE) {}
};

Piece pieces[9] = {
    {1, RECT_IN, TRIANGLE_IN, BOOBS_OUT, BOOBS_OUT},
    {2, RECT_OUT, BOOBS_IN, RECT_IN, TRIANGLE_OUT},
    {3, TRAPEZIUM_OUT, BOOBS_IN, RECT_IN, BOOBS_OUT},
    {4, BOOBS_OUT, TRAPEZIUM_OUT, RECT_IN, TRAPEZIUM_IN},
    {5, TRAPEZIUM_OUT, TRIANGLE_OUT, TRIANGLE_IN, TRAPEZIUM_IN},
    {6, RECT_OUT, TRAPEZIUM_IN, TRIANGLE_IN, TRIANGLE_OUT},
    {7, TRIANGLE_IN, TRIANGLE_IN, RECT_OUT, TRAPEZIUM_OUT},
    {8, TRAPEZIUM_IN, RECT_IN, RECT_OUT, TRAPEZIUM_OUT},
    {9, BOOBS_IN, TRIANGLE_IN, RECT_OUT, BOOBS_OUT}
};
Piece* puzzle[9] = { nullptr };

// Retuns the side index updated on the basis of the orientation
unsigned int convSideAndOrient(int sideIdx, Orientation orientation) {
    return (int) orientation > sideIdx ? sideIdx - (int) orientation + 4 : sideIdx - (int) orientation;
}

// Prints the found puzzle solution
void printPuzzle() {
    for (uint_fast8_t puzzleIdx = 0; puzzleIdx < 9; ++puzzleIdx) {
        std::cout << "Part " << (int)puzzle[puzzleIdx]->pieceNum << " at position " << (int)puzzleIdx+1 << " with orientation " << (int) puzzle[puzzleIdx]->orientation << std::endl;
    }
}

// Returns true if the puzzle is still valid, otherwise false
bool checkPuzzle(uint_fast8_t puzzleIdx) {
    if (puzzleIdx == 1 || puzzleIdx == 2 || puzzleIdx == 4 || puzzleIdx == 5 || puzzleIdx == 7 || puzzleIdx == 8) {
        // check if this piece and the neighbouring piece on the left fit together
        Piece* leftNeighbor = puzzle[puzzleIdx - 1];
        const Side& mySide = puzzle[puzzleIdx]->sides[convSideAndOrient(3, puzzle[puzzleIdx]->orientation)];
        const Side& otherSide = leftNeighbor->sides[convSideAndOrient(1, leftNeighbor->orientation)];
        if (mySide + otherSide != 0) {
            return false;
        }
    }
    if (puzzleIdx > 2) {
        // check if this piece and the upper neighbouring piece fit together
        Piece* upperNeighbor = puzzle[puzzleIdx - 3];
        const Side& mySide = puzzle[puzzleIdx]->sides[convSideAndOrient(0, puzzle[puzzleIdx]->orientation)];
        const Side& otherSide = upperNeighbor->sides[convSideAndOrient(2, upperNeighbor->orientation)];
        if (mySide + otherSide != 0) {
            return false;
        }
    }
    return true;
}

void solve(uint_fast8_t startPuzzleIdx = 0) {
    for (uint_fast8_t pieceCnt = 0; pieceCnt < 9; ++pieceCnt) { // try every piece ...
        if (!pieces[pieceCnt].isUsed) { // ... that has not yet been used 
            puzzle[startPuzzleIdx] = &pieces[pieceCnt];
            pieces[pieceCnt].isUsed = true;
            for (uint_fast8_t sideCnt = 0; sideCnt < 4; ++sideCnt) { // try every orientation
                pieces[pieceCnt].orientation = (Orientation)sideCnt;
                if (checkPuzzle(startPuzzleIdx)) {
                    // puzzle is valid
                    if (startPuzzleIdx == 8) {
                        // all 9 parts used 
                        printPuzzle();
                    } else {
                        // not yet all parts used so do a recursive call for next position in the puzzle
                        solve(startPuzzleIdx + 1);
                    }
                }
                if (startPuzzleIdx == 4) {
                    break; // don't rotate middle piece (otherwise results in four puzzle solutions, same solution but rotated three time by 90 degrees)
                }
            }
            pieces[pieceCnt].isUsed = false;
            puzzle[startPuzzleIdx] = nullptr;
        }
    }
} 

int main() {
    std::cout << "c't 15/2023 puzzle solver" << std::endl;
    solve();
    return 0;
}