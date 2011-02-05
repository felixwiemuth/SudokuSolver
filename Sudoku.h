#ifndef SUDOKU_H
#define SUDOKU_H

#include "Log.h"


class Sudoku
{
    protected:
    private:
        int grid[3][3][3][3]; //[block_x][block_y][field_x][field_y] [bx][by][fx][fy]
        Log log;

    public:
        Sudoku();
        virtual ~Sudoku();

        bool load_from_file(const char file[]);
        bool solve();
        void print();
        void test();
};

#endif // SUDOKU_H
