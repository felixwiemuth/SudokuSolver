#include "Sudoku.h"

#include <iostream>

using namespace std;

int main()
{
    cout << "Sudoku-Solver 1.0 BETA\n" << endl;
    cout << "coord form: (block_x / block_y / field_x / field_y)\n" << endl; //=HELP
    Sudoku s1;
    s1.load_from_file("sudoku.txt");
    s1.print();
    s1.solve();
    s1.print();
    return 0;
}
