#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <cassert>
#include <string>
static int g_value = 0;
struct Cell
{
   typedef char dataType;
   std::set<dataType> values;

   Cell()
   {
      for (dataType i = 1; i < 10; ++i)
      {
         values.insert(i);
      }
   }

   bool IsUniquely()
   {
      return values.size() == 1;
   }

   void MakeUniquely(dataType n)
   {
      g_value++;
      values.clear();
      values.insert(n);
   }
};

class Data
{
public:
   void Init(char const* data)
   {
      size_t number = 0;

      for (char const *cur = data; *cur != 0; ++cur)
      {
         if (!((*cur >= '1' && *cur <= '9') || *cur == Any)) continue;

         ++number;
         if (*cur == Any) continue;
         size_t row = number / 9;
         size_t col = number % 9 - 1;
         Cell::dataType value = *cur - '1' + 1;
         Sudoku[row][col].MakeUniquely(value);
      }
   }

   void Print()
   {
      unsigned __int64 variants = 1;
      for (size_t i = 0; i < 9; ++i)
      {
         for (size_t j = 0; j < 9; ++j)
         {
            Cell &current = Sudoku[i][j];
            if (current.IsUniquely())
               std::cout << "#" << static_cast<int>(*current.values.begin()) << "#";
            else
            {
               std::cout << /*Any*/ "(" << current.values.size() << ")";
               variants *= current.values.size();
            }

            std::cout << Space;
            if (j == 2 || j == 5)
               std::cout << Space;
         }
         std::cout << std::endl;
         if (i == 2 || i == 5)
            std::cout << std::endl;
      }
      std::cout << "Variants: " << std::hex << variants << std::endl;
   }

   void Prepare()
   {
      while(true)
      {
         bool changes = false;

         for (size_t row = 0; row < 9; ++row)
         {
            for(size_t col = 0; col < 9; ++col)
            {
               Cell &cell = Sudoku[row][col];
               if (cell.IsUniquely())
               {
                  Cell::dataType value = *cell.values.begin();
                  bool rowRemove = RemoveFromRow(row, value);
                  bool colRemove = RemoveFromCol(col, value);
                  bool sqadRemove = RemoveFromSquad(row, col, value);

                  if (rowRemove || colRemove || sqadRemove)
                     changes = true;
               }
            }
         }

         if (!changes) break;
      }
   }

   void Solve()
   {
      /*while(true)
      {
         Prepare();

         bool changes = false;

         for (size_t i = 0; i < 3; ++i)
         {
            for(size_t j = 0; j < 3; ++j)
            {
               if (CheckSquad(i, j))
                  changes = true;
            }
         }

         Prepare();
         for (size_t i = 0; i < 9; ++i)
         {
            if (CheckRow(i))
               changes = true;
         }

         Prepare();
         for (size_t i = 0; i < 9; ++i)
         {
            if (CheckCol(i))
               changes = true;
         }

         if (!changes) break;
      }*/
     Prepare();
      g_value = 0;
     backtrackSearch();
     Print();
     std::cout << std::to_string(g_value) << std::endl;
   }

private:
   static const char Any = '*';
   static const char Space = ' ';

   Cell Sudoku[9][9]; //строка, столбец

   bool RemoveFromRow(size_t row, Cell::dataType value)
   {
      bool changes = false;

      for (size_t i = 0; i < 9; ++i)
      {
         Cell& cell = Sudoku[row][i];
         if (cell.IsUniquely()) continue;
         if (cell.values.count(value) > 0)
         {
            cell.values.erase(value);
            changes = true;
         }
      }

      return changes;
   }

   bool RemoveFromCol(size_t col, Cell::dataType value)
   {
      bool changes = false;

      for (size_t i = 0; i < 9; ++i)
      {
         Cell& cell = Sudoku[i][col];
         if (cell.IsUniquely()) continue;
         if (cell.values.count(value) > 0)
         {
            cell.values.erase(value);
            changes = true;
         }
      }

      return changes;
   }

   bool RemoveFromSquad(size_t row, size_t col, Cell::dataType value)
   {
      bool changes = false;
      size_t squadRow = row / 3;
      size_t squadCol = col / 3;

      for (size_t i = 0; i < 3; ++i)
      {
         for (size_t j = 0; j < 3; ++j)
         {
            Cell& cell = Sudoku[squadRow*3 + i][squadCol*3 + j];
            if (cell.IsUniquely()) continue;
            if (cell.values.count(value) > 0)
            {
               cell.values.erase(value);
               changes = true;
            }
         }
      }

      return changes;
   }

   bool CheckSquad(size_t squadRow, size_t squadCol)
   {
      bool changes = false;

      //Подсчет частот для каждой цифры в квадрате (уже определенные цифры не учатсвуют)
      std::map<Cell::dataType, size_t> freq; //контейнер для частот
      for (size_t i = 0; i < 3; ++i)
      {
         for (size_t j = 0; j < 3; ++j)
         {
            size_t row = squadRow * 3 + i;
            size_t col = squadCol * 3 + j;
            Cell& cell = Sudoku[row][col];

            if (cell.IsUniquely()) continue;
            for (auto const value: cell.values)
               ++freq[value];
         }
      }

      //Для каждой единичной частоты выставляем определенное значение:
      for (auto const pair: freq)
      {
         if (pair.second != 1 ) continue;

         const Cell::dataType& digit = pair.first;
         for (size_t i = 0; i < 9; ++i)
         {
            size_t row = squadRow * 3 + i % 3;
            size_t col = squadCol * 3 + i / 3;
            Cell& cell = Sudoku[row][col];

            if (cell.values.count(digit) > 0)
            {
               cell.MakeUniquely(digit);
               changes = true;
               break;
            }
         }
      }

      return changes;
   }

   bool CheckRow(size_t row)
   {
      bool changes = false;

      //Подсчет частот для каждой цифры в строке (уже определенные цифры не учатсвуют)
      std::map<Cell::dataType, size_t> freq; //контейнер для частот
      for (size_t i = 0; i < 9; ++i)
      {
         Cell& cell = Sudoku[row][i];

         if (cell.IsUniquely()) continue;
         for (auto const value: cell.values)
            ++freq[value];
      }

      //Для каждой единичной частоты выставляем определенное значение:
      for (auto const pair: freq)
      {
         if (pair.second != 1 ) continue;

         const Cell::dataType& digit = pair.first;
         for (size_t i = 0; i < 9; ++i)
         {
            Cell& cell = Sudoku[row][i];

            if (cell.values.count(digit) > 0)
            {
               cell.MakeUniquely(digit);
               changes = true;
               break;
            }
         }
      }

      return changes;
   }

   bool CheckCol(size_t col)
   {
      bool changes = false;

      //Подсчет частот для каждой цифры в строке (уже определенные цифры не учатсвуют)
      std::map<Cell::dataType, size_t> freq; //контейнер для частот
      for (size_t i = 0; i < 9; ++i)
      {
         Cell& cell = Sudoku[i][col];

         if (cell.IsUniquely()) continue;
         for (auto const value: cell.values)
            ++freq[value];
      }

      //Для каждой единичной частоты выставляем определенное значение:
      for (auto const pair: freq)
      {
         if (pair.second != 1 ) continue;

         const Cell::dataType& digit = pair.first;
         for (size_t i = 0; i < 9; ++i)
         {
            Cell& cell = Sudoku[i][col];

            if (cell.values.count(digit) > 0)
            {
               cell.MakeUniquely(digit);
               changes = true;
               break;
            }
         }
      }

      return changes;
   }

   bool FindInCol(size_t col, Cell::dataType value)
   {
      for (size_t i = 0; i < 9; ++i)
      {
         Cell& cell = Sudoku[i][col];
         if (cell.IsUniquely() && cell.values.count(value) == 1)
            return true;
      }
      return false;
   }

   bool FindInRow(size_t row, Cell::dataType value)
   {
      for (size_t i = 0; i < 9; ++i)
      {
         Cell& cell = Sudoku[row][i];
         if (cell.IsUniquely() && cell.values.count(value) == 1)
            return true;
      }
      return false;
   }

   bool FindInSquad(size_t row, size_t col, Cell::dataType value)
   {
      size_t squadRow = row / 3;
      size_t squadCol = col / 3;

      for (size_t i = 0; i < 3; ++i)
      {
         for (size_t j = 0; j < 3; ++j)
         {
            Cell& cell = Sudoku[squadRow * 3 + i][squadCol * 3 + j];
            if (cell.IsUniquely() && cell.values.count(value) == 1) 
               return true;
         }
      }

      return false;
   }

   bool IsLegal(size_t row, size_t col, Cell::dataType value)
   {
      return !(FindInCol(col, value) || FindInRow(row, value) || FindInSquad(row, col, value));
   }

   struct Command
   {
      int column;
      int row;
      std::set<Cell::dataType> originalData;
      Cell::dataType value;
   };

   void backtrackSearch()
   {
      std::stack<Command> backtrackStack;
      bool addNew = true; //or next
      for (size_t row = 0; row < 9; row++)
      {
         for (size_t column = 0; column < 9; column++)
         {
            Cell& cell = Sudoku[row][column];
            //std::cout << std::endl;
            //Print();
            //Если цифра на месте идем дальше
            if (cell.IsUniquely())
            {
               continue;
            }
            else
            {
               Cell::dataType value;
               bool isFindVariant = false;
               for (auto& val : cell.values)
               {
                  if (IsLegal(row, column, val))
                  {
                     isFindVariant = true;
                     value = val;
                     break;
                  }
               }

               if (isFindVariant)
               {
                  Command comm;
                  comm.column = column;
                  comm.row = row;
                  comm.originalData = cell.values;
                  comm.value = value;
                  backtrackStack.push(comm);
                  cell.MakeUniquely(value);
               }
               else
               {
                  assert(!backtrackStack.empty());
                  while (true)
                  {
                     bool breakInFor = false;
                     Command comm = backtrackStack.top();
                     backtrackStack.pop();
                     auto it = comm.originalData.find(comm.value);
                     it++;
                     
                     if (it == comm.originalData.end())
                     {
                        Sudoku[comm.row][comm.column].values = comm.originalData;
                        continue;
                     }

                     for (it; it != comm.originalData.end(); ++it)
                     {
                        if (IsLegal(comm.row, comm.column, *it))
                        {
                           row = comm.row;
                           column = comm.column;
                           Command newComm(comm);
                           newComm.value = *it;
                           Sudoku[comm.row][comm.column].MakeUniquely(*it);
                           backtrackStack.push(newComm);
                           breakInFor = true;
                           break;
                        }
                     }
                     if (breakInFor)
                        break;
                     else
                        Sudoku[comm.row][comm.column].values = comm.originalData;
                     
                  }
               }

            }
            
         }
      }
   }

   
};

int main(int argc, char **argv)
{
   Data test;
   test.Init(/*
             "* * *  * 3 7  6 * *"
             "* * *  6 * *  * 9 *"
             "* * 8  * * *  * * 4"

             "* 9 *  * * *  * * 1"
             "6 * *  * * *  * * 9"
             "3 * *  * * *  * 4 *"

             "7 * *  * * *  8 * *"
             "* 1 *  * * 9  * * *"
             "* * 2  5 4 *  * * *"//*/
             /*
             "* * *  1 * *  9 2 *"
             "* 6 1  * * *  * * *"
             "5 * *  * * *  8 * *"

             "* 4 *  * 1 *  5 * *"
             "* * *  * 8 *  * * 7"
             "6 1 *  * * 5  * * 3"

             "* 7 9  3 * *  * * 5"
             "* * 8  * * *  3 * *"
             "* * *  * * *  * 1 *"//*/
             /*
             "* * *  * * *  * * *"
             "* * *  * * *  * * *"
             "* * *  * * *  * * *"

             "* * *  1 2 3  * * *"
             "* * *  4 * 6  * * *"
             "* * *  7 8 9  * * *"

             "* * *  * * *  * * 5"
             "* * *  * * *  3 * *"
             "* * *  * * *  * * *"//*/

             "9 * *  * * 5  * * 1"
             "* * *  * 4 *  * * 8"
             "* * *  7 * *  3 2 *"

             "* 9 7  2 6 *  * 5 *"
             "* * *  * * *  * * *"
             "* 2 *  * 5 4  7 8 *"

             "* 4 1  * * 6  * * *"
             "3 * *  * 1 *  * * *"
             "6 * *  8 * *  * * 9"
             );
   std::cout << "Before:" << std::endl;
   test.Print();

   /*test.Prepare();
   std::cout << std::endl << "Prepared:" << std::endl;
   test.Print();*/

   test.Solve();
   std::cout << std::endl << "After:" << std::endl;
   test.Print();
   return 0;
}