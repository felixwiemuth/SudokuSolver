#include "Sudoku.h"

#include <fstream>
#include <iostream>
#include <set>
#include <boost/lexical_cast.hpp>

using namespace std;

Sudoku::Sudoku()
{
    //ctor
}

Sudoku::~Sudoku()
{
    //dtor
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
    return true;
}

bool Sudoku::solve()
{
    log.add("Start to solve Sudoku...");
    bool solved = false;
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
        }
    }

    for (int n = 1; ; n++)
    {
        stringstream sstr;
        sstr << "Starting cycle " << n << "...";
        log.add(sstr.str());
        bool changed = false;
        //check lines
        for (int by = 0; by < 3; by++)
        {
            for (int fy = 0; fy < 3; fy++)
            {
                for (int bx = 0; bx < 3; bx++)
                {
                    for (int fx = 0; fx < 3; fx++)
                    {
                        if (grid[bx][by][fx][fy] == 0)
                        {
                            sstr.str("");
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

                            stringstream sstr;
                            sstr << " -> Candidates are:";
                            for (vector<int>::iterator it = found.begin(); it != found.end(); it++)
                            {
                                sstr << " " << *it;
                            }
                            if (found.size() == 1)
                            {
                                grid[bx][by][fx][fy] = found[0];
                                lines[by][fy].insert(grid[bx][by][fx][fy]);
                                columns[bx][fx].insert(grid[bx][by][fx][fy]);
                                squares[bx][by].insert(grid[bx][by][fx][fy]);
                                changed = true;
                                sstr << " --> inserted!";
                            }
                            log.add(sstr.str());
                        }
                    }
                }
            }
        }
        sstr.str("");
        sstr << "...finished cycle " << n;
        log.add(sstr.str());
        if (changed == false) //no digit entered --> no more digits can be found
            break;
    }
    log.add("...finisehd solving");
    return true; //TODO check if solved!
}

void Sudoku::print()
{
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
