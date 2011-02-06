#ifndef SUDOKU_H
#define SUDOKU_H

#include "Log.h"


class Sudoku
{
    protected:
    private:
        int base; //NxN Sudoku (n x n x n x n)
        int grid[3][3][3][3]; //[block_x][block_y][field_x][field_y] [bx][by][fx][fy]
        bool valid; //indicates whether Sudoku in 'grid' is valid
        Log log;

    public:
        Sudoku();
        virtual ~Sudoku();

        bool load_from_file(const char file[]);
        bool solve();
        int is_solved(); //returns number of fields that are not solved ('0') -- return value: '0' = solved, '>0' = unsolved
        void print();
};

#endif // SUDOKU_H
