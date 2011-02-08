#include "Sudoku.h"

#include <iostream>

#include <windows.h> //DEBUG

using namespace std;

int main()
{
    cout << "Sudoku-Solver 1.0 BETA\n" << endl;
    cout << "coord form: (block_x / block_y / field_x / field_y)\n" << endl; //=HELP
//    Sudoku s1;
//    s1.load_from_file("sudoku2.txt");
//    s1.print();
//    s1.solve();
//    s1.print();
    Sudoku s1;

    Sudoku s2("sudoku.txt");
    s2.solve();
    s2.print();
    //system("PAUSE"); //DEBUG
//    char tmp;
//    cout << "Type anything to continue...";
//    cin >> tmp;f
    return 0;
}
