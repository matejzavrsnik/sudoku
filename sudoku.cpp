/*
Copyright (C) 2013 Matej Zavrsnik <matejzavrsnik@gmail.com> (matejzavrsnik.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "sudoku.h"

using namespace std;

CSudoku::CSudoku() :
    m_numbers({1,2,3,4,5,6,7,8,9})
{
}

CSudoku::CSudoku(const CSudoku& orig) 
{
    *this = orig;
}

CSudoku::~CSudoku() 
{
}

void CSudoku::setPuzzle(const std::vector<short>& puzzle)
{
    m_puzzle = convert(puzzle);
}

std::vector<short> CSudoku::getSolution()
{
    return convert(m_puzzle);
}

void CSudoku::solve()
{
    std::vector<CSudokuCell>::iterator start = m_puzzle.begin();
    if(start->given)
        start = getNext(m_puzzle.begin());
    solve(start);
}

void CSudoku::solve(std::vector<CSudokuCell>::iterator curr)
{
    vector<short>::iterator numbersIterator = m_numbers.begin();
    if(curr == m_puzzle.end())
    {
        m_solved = true;
    }
    else
    {
        while(numbersIterator != m_numbers.end() && !m_solved)
        {
            curr->val = *numbersIterator;
            if(valid(curr))
            {
                vector<CSudokuCell>::iterator next = getNext(curr);
                solve(next);
            }
            ++numbersIterator;
        }
        if(!m_solved) curr->val = 0;
    }
}

bool CSudoku::valid(vector<CSudokuCell>::iterator current)
{
    // empty cell means invalid puzzle
    if(current->val == 0 ) return false;
    // get coordinates in sudoku puzzle
    int index = distance(m_puzzle.begin(), current);
    pair<short,short> coor = get2DCoordinatesFromIndex<short>(index,9);
    short i = coor.first;
    short j = coor.second;
    // is there a number repeated in any row or column?
    for(short k=0; k<9; k++)
    {
        if( ( k != i && cell(k,j) == cell(i,j) ) ||
            ( k != j && cell(i,k) == cell(i,j) ) )
            return false;
    }
    // which square in sudoku do these coordinate fall into?
    short row = i / 3;
    short col = j / 3;
    // what are the edges of this square?
    short row_from, row_to, col_from, col_to;
    row_from = row*3;
    row_to = row_from + 3 - 1;
    col_from = col*3;
    col_to = col_from + 3 - 1;
    // is there a number repeated inside this square?
    for( int k = row_from; k <= row_to; k++ )
        for ( int l = col_from; l <= col_to; l++ )
            if( k != i && l != j && cell(k,l) == cell(i,j) )
                return false;
    // if all checks passed, puzzle must be valid for this cell
    return true;
}

inline short CSudoku::cell(short i, short j)
{
    return m_puzzle[ (short)getIndexFrom2DCoordinates<short>( pair<short,short>(i,j) ,9 ) ].val;
}

vector<CSudoku::CSudokuCell>::iterator CSudoku::getNext(vector<CSudoku::CSudokuCell>::iterator curr)
{
    for(vector<CSudokuCell>::iterator next = ++curr; next!=m_puzzle.end(); ++next)
        if(!next->given) return next;
    return m_puzzle.end();
}

vector<short> CSudoku::convert(vector<CSudokuCell> puzzle)
{
    vector<short> result;
    for(vector<CSudokuCell>::iterator it = puzzle.begin(); it != puzzle.end(); ++it)
        result.push_back((*it).val);
    return result;
}

vector<CSudoku::CSudokuCell> CSudoku::convert(vector<short> puzzle)
{
    vector<CSudokuCell> result;
    for(vector<short>::iterator it = puzzle.begin(); it != puzzle.end(); ++it)
    {
        CSudokuCell cell;
        if(*it > 0)
        {
            cell.given = true;
            cell.val = *it;
        }
        result.push_back(cell);
    }
    return result;
}

std::ostream& operator<< (std::ostream& os, const CSudoku& puzzle)
{
    for(int j=0; j<9; ++j)
        for(int i=0; i<9; ++i)
        {
            short val = puzzle.m_puzzle[ (short)getIndexFrom2DCoordinates<short>( pair<short,short>(i,j) ,9 ) ].val;
            if(i==0)
            {
                os << endl;
                if (j%3 == 0) os << "-------------" << endl;
                os << "|";
            }
            os << val;
            if((i+1)%3==0) os << "|";
        }
    os << endl << "-------------";
    return os;
}
