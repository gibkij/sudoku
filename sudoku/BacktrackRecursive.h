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
   //���� ������ ������
   const Cell::dataType cEmptyCell = '#';

   //����� �� �������� �������� � ��� ������?
   bool IsRightValue(size_t row, size_t column, Cell::dataType value);
   bool IsPresentInRow();
   bool IsPresentInColumn();
   bool IsPresentInSquad();

   //������ ����� ����
   bool SolveThisShit(Cell::dataType sudoku[9][9]);
};

