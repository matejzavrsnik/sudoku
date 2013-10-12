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

#include "killersudoku.h"
#include <tools.h>
#include "genetic.cpp"
#include <climits>

using namespace std;

CKillerSudoku::CKillerSudoku() 
{
}

CKillerSudoku::~CKillerSudoku() 
{
}

void CKillerSudoku::setPuzzle(puzzle_t& puzzle)
{
    m_puzzle = puzzle;
}

void CKillerSudoku::solve()
{
    Genetic<CKillerSudoku, bitset_size>::genome_bitset candidate;
    Genetic<CKillerSudoku, bitset_size> genetic(candidate, &CKillerSudoku::fitnessFunction, this);
    genetic.SetGenomesInGeneration(1000);
    genetic.SetMutationRate(10); // how many bits to zap each generation?
    genetic.SetSurviveRate(800);
    int currentBestScore = INT_MIN;
    do
    {
        genetic.Generation();
        cout << "."; cout.flush();
        if(genetic.GetBestScore() > currentBestScore)
        {
            currentBestScore = genetic.GetBestScore();
            cout << endl << "New best score: " << currentBestScore << "!" << endl;
            m_solution = convert(genetic.GetBestGenome());
            cout << *this << endl;
        };
    }
    while(genetic.GetBestScore() == 0);
    m_solution = convert(genetic.GetBestGenome());
}

CKillerSudoku::solution_t CKillerSudoku::getSolution()
{
    return m_solution;
}

int CKillerSudoku::fitnessFunction(const sudoku_bitset_t& candidate)
{
    solution_t candidateSolution = convert(candidate);
    int score = 0;
    for(solution_t::iterator cell = candidateSolution.begin(); cell != candidateSolution.end(); ++cell)
    {
        score += validate(candidateSolution,cell);
    }
    return score;
}

int CKillerSudoku::validate(solution_t& solution, solution_t::iterator current)
{
    int score = 0; // each score point means more invalid puzzle
    // not in range 1..9 means invalid puzzle
    // high penalty, because these numbers should not even be considered
    if(*current>9) score -= 10*(*current-9);
    if(*current<1) score -= 10*1;
    // get coordinates in sudoku puzzle
    int index = distance(solution.begin(), current);
    pair<short,short> coor = get2DCoordinatesFromIndex<short>(index,9);
    short i = coor.first;
    short j = coor.second;
    // is there a number repeated in any row or column?
    for(short k=0; k<9; k++)
    {
        if( ( k != i && cell(solution,k,j) == cell(solution,i,j) ) ||
            ( k != j && cell(solution,i,k) == cell(solution,i,j) ) )
            --score;
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
            if( k != i && l != j && cell(solution,k,l) == cell(solution,i,j) )
                --score;
    // killer sudoku part: find a region this cell belongs to, 
    // sum up and compare to what it would need to be
    for(region_t region: m_puzzle)
    {
        std::vector<short>& regionCells = region.second;
        std::vector<short>::iterator found = std::find(regionCells.begin(), regionCells.end(), (short)index);
        if(found != region.second.end())
        {
            int sum = 0;
            for(auto cell: region.second)
            {
                sum += cell;
            }
            score -= 2*abs(sum-region.first); // difference in sum
            break;
        }
    }
    // if all checks passed, puzzle must be valid for this cell
    return score;
}

std::ostream& operator<< (std::ostream& os, const CKillerSudoku& puzzle)
{
    for(int j=0; j<9; ++j)
        for(int i=0; i<9; ++i)
        {
            short val = puzzle.m_solution[ (short)getIndexFrom2DCoordinates<short>( pair<short,short>(i,j) ,9 ) ];
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

CKillerSudoku::sudoku_bitset_t CKillerSudoku::convert(solution_t solution)
{
    sudoku_bitset_t result;
    size_t pos = 0;
    for(auto cell_value : solution)
    {
        bitset<4> cell_bits(cell_value);
        for(size_t i = 0; i<cell_bits.size(); ++i)
        {
            result.set(pos++, cell_bits.test(i));
        }
    }
    return result;
}

CKillerSudoku::solution_t CKillerSudoku::convert(const sudoku_bitset_t& solution)
{
    solution_t result;
    std::bitset<cell_bits_count> cell_bitset;
    size_t pos =0;
    for(size_t cell=0; cell<cells_count; ++cell)
    {
        for(size_t cell_bit=0; cell_bit<cell_bitset.size(); ++cell_bit, ++pos)
        {
            cell_bitset.set(cell_bit, solution.test(pos)); 
        }
        short cell_value = (short)cell_bitset.to_ulong();
        result.push_back(cell_value);
    }
    return result;
}

inline short CKillerSudoku::cell(solution_t& solution, short i, short j)
{
    return solution[ (short)getIndexFrom2DCoordinates<short>( pair<short,short>(i,j) ,9 ) ];
}
