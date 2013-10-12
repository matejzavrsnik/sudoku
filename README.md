# Sudoku
A library that solves sudoku. 
CSudoku class will solve sudoku puzzle with backtracking.
CKillerSudoku will attempt to solve killer sudoku with genetic algorithm, but will probably fail to complete task.

## Usage example
Example of solving normal sudoku.

    vector<short> puzzle {0,5,9, 0,2,0, 4,6,0, 
                          1,0,0, 4,0,3, 0,0,8,
                          3,0,0, 0,7,0, 0,0,2,
                          
                          0,3,0, 8,0,9, 0,2,0,
                          6,0,5, 0,0,0, 3,0,7,
                          0,1,0, 7,0,6, 0,4,0,
    
                          2,0,0, 0,1,0, 0,0,4,
                          9,0,0, 3,0,2, 0,0,5,
                          0,7,8, 0,6,0, 2,3,0};
    CSudoku sudoku, solved;
    sudoku.setPuzzle(puzzle);
    solved = sudoku;
    solved.solve();
    cout << "Original sudoku: " << sudoku << endl;
    cout << "Solved sudoku: " << solved << endl;

Example of solving killer sudoku. The puzzle is somewhat hard to represent in ascii art.
What the definition of puzzle means is, for instance, the first region has sum 11, and consists of cells 0 and 9.

    CKillerSudoku::puzzle_t puzzle;
    puzzle = // sample puzzle
    {
        {11, {0,9}},
        {30, {1,2,3,10,11,12}},
        {7,  {4,5,6}},
        {23, {7,8,16,17}},
        {32, {13,14,20,21,22}},
        {25, {15,24,25,26,34}},
        {11, {18,27}}, 
        {14, {19,28,29}},
        {20, {23,32,33}},
        {8,  {30,39}},
        {9,  {31,40,49}},
        {10, {35,42,43,44}},
        {21, {36,37,38,45}},
        {14, {41,50}},
        {24, {46,54,55,56,65}},
        {17, {47,48,57}},
        {17, {51,52,61}},
        {11, {53,62}},
        {24, {58,59,60,66,67}},
        {21, {63,64,72,73}},
        {25, {68,69,70,77,78,79}},
        {15, {71,80}},
        {16, {74,75,76}}
    };
    // create solver object and solve the puzzle
    CKillerSudoku killer;
    killer.setPuzzle(puzzle);
    killer.solve();
    cout << killer;
    return 0;

## Licensing
Sudoku is distributed under MIT licence as stated on http://opensource.org/licenses/MIT, which is very permissive. You can do anything you like with this code. Commercial use is allowed. See LICENCE file for details.

## Copyright
Copyright (c) 2013 Matej Zavrsnik <matejzavrsnik.com>