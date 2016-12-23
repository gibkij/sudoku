#pragma once
#include "Cell.h"

class BacktrackRecursive
{
public:
   BacktrackRecursive(Cell sudoku[9][9]);
   ~BacktrackRecursive(void);
   void Start();
private:
   Cell::dataType mSudoku;
   //«нак пустой €чейки
   const Cell::dataType cEmptyCell = '#';

   //ћожно ли воткнуть значение в эту €чейку?
   bool IsRightValue(size_t row, size_t column, Cell::dataType value);
   bool IsPresentInRow();
   bool IsPresentInColumn();
   bool IsPresentInSquad();

   //¬нутри зовет себ€
   bool SolveThisShit(Cell::dataType sudoku[9][9]);
};

