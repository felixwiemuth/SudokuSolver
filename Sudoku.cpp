#include "Sudoku.h"

#include <fstream>
#include <iostream>
#include <set>
#include <cmath>
#include <boost/lexical_cast.hpp>

using namespace std;

Sudoku::Sudoku()
{
    init();
}

Sudoku::Sudoku(const char file[])
{
    init();
    load_from_file(file);
}

Sudoku::~Sudoku()
{
    //dtor
}

void Sudoku::init()
{
    base = 3; //3x3 Sudoku
    valid = false;
}

bool Sudoku::load_from_file(const char file[])
{
    ifstream infile(file); //create filestream to read, open file
    if (infile.is_open() == false)
    {
        log.err("File could not be opened!");
        return false; //file not found or couldn´t be opened -> abort and return false
    }

    int by = 0;
    int fy = 0;
    int pos = 0; //position in string

    string line; //buffer for single lines of the file
    while ( !(infile.eof()) )
    {
        getline(infile, line);
        if (line.length() != 9)
        {
            log.err("File not a correct Sudoku!");
            return false;
        }

        //move horizontally
        for (int bx = 0; bx < 3; bx++)
        {
            for (int fx = 0; fx < 3; fx++)
            {
                grid[bx][by][fx][fy] = boost::lexical_cast<int>(line[pos]);
                pos++;
            }
        }
        pos = 0;
        //move down a line
        if (fy < 2)
        {
            fy++;
        }
        else
        {
            fy = 0;
            by++;
        }
    }
    log.add("Sudoku successfully loaded!");
    valid = true;
    return true;
}

bool Sudoku::solve()
{
    if (!valid)
    {
        log.err("Cannot solve an invalid Sudoku!");
        return false;
    }
    log.add("Start to solve Sudoku...");
    stringstream sstr; //stream to make up log entries
    int zeros_begin = is_solved(); //unsolved fields before solving
    int checks = 0; //number of checks needed

    //switches indicating if a line, coulmn, or square was changed by inserting digits
    //so it has to be considered again during next solving cycle
    bool ch_lines[3][3]; //[by][fy]
    bool ch_columns[3][3]; //[bx][fx] //TODO: init
    bool ch_squares[3][3]; //[bx][by] //TODO: init

    //sets including the digits that are already used in respective category
    set<int> lines[3][3]; //[by][fy]
    set<int> columns[3][3]; //[bx][fx]
    set<int> squares[3][3]; //[bx][by]

    //fill sets with current values
    for (int by = 0; by < 3; by++)
    {
        for (int fy = 0; fy < 3; fy++)
        {
            for (int bx = 0; bx < 3; bx++)
            {
                for (int fx = 0; fx < 3; fx++)
                {
                    lines[by][fy].insert(grid[bx][by][fx][fy]);
                    columns[bx][fx].insert(grid[bx][by][fx][fy]);
                    squares[bx][by].insert(grid[bx][by][fx][fy]);
                }
            }
            ch_lines[by][fy] = true; //initialize ch_lines[][]
            ch_columns[by][fy] = true; //initialize ch_columns[][]
            ch_squares[by][fy] = true; //initialize ch_squares[][]
        }
    }

    //go over changed fields and search for possible inputs, input if only one possibility
    for (int n = 1; ; n++)
    {
        stringstream sstr;
        sstr << "Starting cycle " << n << "...";
        log.add(sstr.str());
        bool changed = false;

        for (int by = 0; by < 3; by++)
        {
            for (int fy = 0; fy < 3; fy++)
            {
//                if (ch_lines[by][fy] == true) //TODO check only on changed lines, columns, squares... but how???
//                {
//                    ch_lines[by][fy] = false;
                    for (int bx = 0; bx < 3; bx++)
                    {
                        for (int fx = 0; fx < 3; fx++)
                        {
//                            if (ch_columns[bx][fx] == true)
//                            {
//                                ch_columns[bx][fx] = false;
//                                cout << "FOUND INSERTED FIELD: " << bx << "/" << by << "/" << fx << "/" << fy << ")" << endl;

                                if (grid[bx][by][fx][fy] == 0)
                                {
                                    checks++;
                                    stringstream sstr;
                                    sstr << "Checking for (" << bx << "/" << by << "/" << fx << "/" << fy << ")";
                                    log.add(sstr.str());
                                    vector<int> found; //possible digits
                                    for (int i = 1; i <= 9; i++)
                                    {
                                        if (lines[by][fy].find(i) == lines[by][fy].end() && columns[bx][fx].find(i) == columns[bx][fx].end() && squares[bx][by].find(i) == squares[bx][by].end())
                                        {
                                            found.push_back(i);
                                        }
                                    }

                                    sstr.str("");
                                    sstr << " -> Candidates are:";
                                    for (vector<int>::iterator it = found.begin(); it != found.end(); it++)
                                    {
                                        sstr << " " << *it;
                                    }
                                    if (found.size() == 1) //only one possibility --> insert digit
                                    {
                                        grid[bx][by][fx][fy] = found[0];
                                        lines[by][fy].insert(grid[bx][by][fx][fy]);
                                        columns[bx][fx].insert(grid[bx][by][fx][fy]);
                                        squares[bx][by].insert(grid[bx][by][fx][fy]);
                                        changed = true;
                                        ch_lines[by][fy] = true;
                                        ch_columns[bx][fx] = true;
                                        ch_squares[bx][by] = true;
                                        sstr << " --> inserted!";
                                    }
                                    log.add(sstr.str());
                                }
                            //}//chcolumns
                        }
                    }
                //}//chlines
            }
        }
        sstr.str("");
        sstr << "...finished cycle " << n;
        log.add(sstr.str());
        if (changed == false) //no digit entered --> no more digits can be found
            break;
    }
    log.add("...finished solving");
    int zeros = is_solved();
    if (zeros == 0)
    {
        sstr.str("");
        sstr << "--> Sudoku solved completely (solved " << zeros_begin - zeros << " of " << (pow(base, 4)) << " fields, needing " << checks << " checks).";
        log.add(sstr.str());
        return true;
    }
    else
    {
        sstr.str("");
        sstr << "--> " << zeros << " of " << (pow(base, 4)) << " fields left usolved (solved " << zeros_begin - zeros << " fields).";
        log.add(sstr.str());
        return false;
    }
}

int Sudoku::is_solved()
{
    int cnt = 0; //number of '0' in 'grid[][][][]'
    for (int by = 0; by < 3; by++)
        for (int fy = 0; fy < 3; fy++)
            for (int bx = 0; bx < 3; bx++)
                for (int fx = 0; fx < 3; fx++)
                    if (grid[bx][by][fx][fy] == 0)
                        cnt++;
    return cnt;
}

void Sudoku::print()
{
    if (!valid)
    {
        log.err("Cannot print an invalid Sudoku!");
        return;
    }
    cout << endl;
    for (int by = 0; by < 3; by++)
    {
        for (int fy = 0; fy < 3; fy++)
        {
            for (int bx = 0; bx < 3; bx++)
            {
                for (int fx = 0; fx < 3; fx++)
                {
                    //cout << "Print: POS(" << bx << "/" << by << "/" << fx << "/" << fy << "): " << grid[bx][by][fx][fy] << endl;
                    cout << grid[bx][by][fx][fy] << " ";
                }
                cout << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
}
