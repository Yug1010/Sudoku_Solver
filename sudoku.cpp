#include <iostream>
using namespace std;

class Sudoku
{
private:
    int grid[9][9];

    bool isSafe(int row, int col, int num)
    {
        for (int c = 0; c < 9; c++)
            if (grid[row][c] == num)
                return false;

        for (int r = 0; r < 9; r++)
            if (grid[r][col] == num)
                return false;

        int startRow = row - (row % 3);
        int startCol = col - (col % 3);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (grid[startRow + i][startCol + j] == num)
                    return false;

        return true;
    }

    bool isValidInput()
    {
        for (int r = 0; r < 9; r++)
            for (int c = 0; c < 9; c++)
                if (grid[r][c] < 0 || grid[r][c] > 9)
                    return false;

        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                int val = grid[r][c];
                if (val == 0)
                    continue;

                grid[r][c] = 0;

                if (!isSafe(r, c, val))
                {
                    grid[r][c] = val;
                    return false;
                }

                grid[r][c] = val;
            }
        }
        return true;
    }

    int countCandidates(int row, int col)
    {
        int count = 0;
        for (int num = 1; num <= 9; num++)
            if (isSafe(row, col, num))
                count++;
        return count;
    }

    bool findMRVCell(int &bestRow, int &bestCol)
    {
        int minCandidates = 10;
        bestRow = -1;
        bestCol = -1;

        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                if (grid[r][c] != 0)
                    continue;

                int count = countCandidates(r, c);

                if (count == 0)
                    return false;

                if (count < minCandidates)
                {
                    minCandidates = count;
                    bestRow = r;
                    bestCol = c;

                    if (minCandidates == 1)
                        return true;
                }
            }
        }

        return true;
    }

    bool backtrack()
    {
        int row, col;

        if (!findMRVCell(row, col))
            return false;

        if (row == -1)
            return true;

        for (int num = 1; num <= 9; num++)
        {
            if (!isSafe(row, col, num))
                continue;

            grid[row][col] = num;

            if (backtrack())
                return true;

            grid[row][col] = 0;
        }

        return false;
    }

public:
    void input();
    void display();
    bool validateInput();
    bool solve();
};

void Sudoku::input()
{
    cout << "Enter Sudoku puzzle row by row (0 for empty cells):\n";
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            cin >> grid[i][j];
}

void Sudoku::display()
{
    cout << "\n+-------+-------+-------+\n";
    for (int i = 0; i < 9; i++)
    {
        if (i == 3 || i == 6)
            cout << "+-------+-------+-------+\n";
        for (int j = 0; j < 9; j++)
        {
            if (j == 0 || j == 3 || j == 6)
                cout << "| ";
            cout << (grid[i][j] == 0 ? '.' : (char)('0' + grid[i][j])) << " ";
        }
        cout << "|\n";
    }
    cout << "+-------+-------+-------+\n";
}

bool Sudoku::validateInput()
{
    return isValidInput();
}

bool Sudoku::solve()
{
    return backtrack();
}

int main()
{
    Sudoku s;
    s.input();

    cout << "\nInput Sudoku:";
    s.display();

    if (!s.validateInput())
    {
        cout << "\nInvalid puzzle! The given clues conflict with each other.\n";
        cout << "Check for duplicate numbers in any row, column, or 3x3 box.\n";
        return 1;
    }

    cout << "\nPuzzle is valid. Solving...\n";

    if (s.solve())
    {
        cout << "\nSolved Sudoku:";
        s.display();
    }
    else
    {
        cout << "\nNo solution exists for this puzzle.\n";
    }

    return 0;
}
