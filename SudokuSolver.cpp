#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <string>
#include <unordered_set>
#include <ctime>

using namespace std;

const int N = 9;
double fitness(vector<vector<int> >&);
bool mutate(vector<vector<int> >&, double);
int nextRowUnused(vector<vector<int> >&, int);
int nextColUnused(vector<vector<int> >&, int);
int nextBlockUnused(vector<vector<int> > &, int, int);
void swap(vector<vector<int> > &, int, int, int, int);

int main(int argc, char **agrv) {
  srand(time(NULL));

  // a valid sudoku
  // vector<vector<int> > sudoku({{7,3,5,6,1,4,8,9,2},{8,4,2,9,7,3,5,6,1},{9,6,1,2,8,5,3,7,4},{2,8,6,3,4,9,1,5,7},{4,1,3,8,5,7,9,2,6},{5,7,9,1,2,6,4,3,8},{1,5,7,4,9,2,6,8,3},{6,9,4,7,3,8,2,1,5},{3,2,8,5,6,1,7,4,9}});

  vector<vector<int> > sudoku;
  vector<int> row({1, 2, 3, 4, 5, 6, 7, 8, 9});
  for (int i = 0; i < N; i++) {
    sudoku.push_back(row);
  }

  cout << mutate(sudoku, 1.0) << endl;
  cout << "fitness: " << fitness(sudoku) << endl;
  
  
}

double fitness(vector<vector<int> > &sudoku) {
  vector<int> row_count(N, 0);
  vector<int> col_count(N, 0);
  vector<int> block_count(N, 0);
  double row_sum = 0, col_sum = 0, block_sum = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      row_count[sudoku[i][j] - 1]++;
    }
    unordered_set<int> row_count_set(row_count.begin(), row_count.end());
    row_sum += (1.0 / row_count_set.size()) / N;
    vector<int> tmp(N, 0);
    row_count = tmp;
  }

  for (int j = 0; j < N; j++) {
    for (int i = 0; i < N; i++) {
      row_count[sudoku[i][j] - 1]++;
    }
    unordered_set<int> col_count_set(col_count.begin(), col_count.end());
    col_sum += (1.0 / col_count_set.size()) / N;
    vector<int> tmp(N, 0);
    col_count = tmp;
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
          block_count[sudoku[i * 3 + k][j * 3 + l] - 1]++;
        }
      }
      unordered_set<int> block_count_set(block_count.begin(), block_count.end());
      block_sum += (1.0 / block_count_set.size()) / N;
      vector<int> tmp(N, 0);
      block_count = tmp;
    }
  }

  // cout << "row sum: " << (int)row_sum << ", col sum: " << (int)col_sum << ", block sum: " << (int)block_sum << endl;
  return (int)row_sum == 1 && (int)col_sum == 1 && (int)block_sum == 1 ? 1.0 : col_sum * block_sum;
}

bool mutate(vector<vector<int> > &sudoku, double mutation_rate) {
  double random = rand() / (double)RAND_MAX;
  if (random < mutation_rate) {
    while (true) {
      int row = rand() % N;
      int col1 = rand() % N;
      int col2 = rand() % N;
      while (col1 == col2) {
        col1 = rand() % N;
        col2 = rand() % N;
      }
      swap(sudoku, row, col1, row, col2);
      if (nextColUnused(sudoku, col1) != -1 && nextColUnused(sudoku, col2) != -1 && nextBlockUnused(sudoku, row / 3, col1 / 3) != -1 && nextBlockUnused(sudoku, row / 3, col2 / 3) != -1) {
        return true;
      } else {
        swap(sudoku, row, col1, row, col2);
      }
    }
  }
  return false;
}

// return -1 if this row is not valid (have duplicate)
// return 0 if all 9 numbers are used
// return the next unused number
int nextRowUnused(vector<vector<int> > &sudoku, int row) {
  bool repeat[N];
  for (int i = 0; i < N; i++) {
    if (repeat[sudoku[row][i]]) {
      return -1;
    }
    repeat[sudoku[row][i] - 1] = true;
  }
  for (int i = 0; i < N; i++) {
    if (!repeat[i]) {
      return i + 1;
    }
  }
  return 0;
}

int nextColUnused(vector<vector<int> > &sudoku, int col) {
  bool repeat[N];
  for (int i = 0; i < N; i++) {
    if (repeat[sudoku[i][col]]) {
      return -1;
    }
    repeat[sudoku[i][col] - 1] = true;
  }
  for (int i = 0; i < N; i++) {
    if (!repeat[i]) {
      return i + 1;
    }
  }
  return 0;
}

// (row, col) = top left cell of the block
int nextBlockUnused(vector<vector<int> > &sudoku, int row, int col) {
  bool repeat[N];
  for (int i = row; i < row + 3; i++) {
    for (int j = col; j < col + 3; j++) {
      if (repeat[sudoku[i][j] - 1]) {
        return -1;
      }
      repeat[sudoku[i][j] - 1] = true;
    }
  }

  for (int i = 0; i < N; i++) {
    if (!repeat[i]) {
      return i + 1;
    }
  }
  return 0;
}

void swap(vector<vector<int> > &sudoku, int r1, int c1, int r2, int c2) {
  int tmp = sudoku[r1][c1];
  sudoku[r1][c1] = sudoku[r2][c2];
  sudoku[r2][c2] = tmp;
}