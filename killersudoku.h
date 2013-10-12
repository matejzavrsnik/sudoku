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

#ifndef KILLERSUDOKU_H
#define	KILLERSUDOKU_H

#include <vector>
#include <bitset>
#include <iostream>
#include "genetic.h"

class CKillerSudoku {
public:
    // puzzle in killer sudoku consists of regions, that themselves consist of sums and vector of indices
    using region_t = std::pair<int, std::vector<short> >;
    using puzzle_t = std::vector<region_t>;
    // solution is simply a sudoku, a vector of short
    using solution_t = std::vector<short>;
    
    CKillerSudoku();
    virtual ~CKillerSudoku();
    
    void setPuzzle(puzzle_t& puzzle);
    void solve();
    solution_t getSolution();
    
private:
    // 4 bits needed for numbers from 1..9 * 81 cells in sudoku = 324 bits needed
    static const size_t cell_bits_count = 4;
    static const size_t cells_count = 81;
    static const int bitset_size = cell_bits_count * cells_count;
    using sudoku_bitset_t = Genetic<CKillerSudoku, CKillerSudoku::bitset_size>::genome_bitset;
    
    solution_t m_solution;
    puzzle_t m_puzzle;
    
    int fitnessFunction(const sudoku_bitset_t& candidate);
    int validate(solution_t& solution, solution_t::iterator current);
    
    sudoku_bitset_t convert(solution_t solution);
    solution_t convert(const sudoku_bitset_t& solution);
    inline short cell(solution_t& solution, short i, short j);
    
    friend std::ostream& operator<< (std::ostream& os, const CKillerSudoku& puzzle);
};

#endif	/* KILLERSUDOKU_H */

