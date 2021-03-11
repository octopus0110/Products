/*
数独を解く(正方形)
入力は一行目に一辺の長さ次の行から盤面
ex)
1  |  9
2  |  5 o o o o o 9 o o
3  |  o o 7 5 o o o o 2
4  |  o 4 1 o 9 o o o 3
5  |  7 1 6 o 2 o o o o
6  |  o 8 4 3 6 5 1 o o
7  |  9 5 o 4 7 1 o o 8
8  |  o o o 2 1 9 8 3 6
9  |  o 3 8 7 4 6 o 2 o
10 |  o 2 9 o 5 3 4 7 o
*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
template<class T> using V = vector<T>;
template<class T> using VV = V<V<T>>;
template<class T> using VVV = V<VV<T>>;
using I = int;
using B = bool;
using VI = V<I>;
using VVI = VV<I>;
using VB = V<B>;
using VVB = VV<B>;

#define ENDL cout << endl;

struct Table
{
  int N;
  VVI pazzle;
  VVB fixed;

  Table(){}

  void input()
  {
    cin >> N;
    VVI A(N, VI(N, 0));
    VVB B(N, VB(N, false));
    for (int i = 0; i < N; ++i) for(int j = 0; j < N; ++j)
    {
      string tmp;
      cin >> tmp;
      if ((tmp.size() == 1 && '0' < tmp[0] && tmp[0] <= '9') || tmp.size() > 1)
      {
        int n = stoi(tmp);
        A[i][j] = n;
        B[i][j] = true;
      }
    }
    pazzle = A;
    fixed = B;
  }

  void output()
  {
    for (int i = 0; i < N; ++i)
    {
      for(int j = 0; j < N; ++j)
      {
        cout << pazzle[i][j] << " ";
      }
      ENDL
    }
  }

  void update(VVI A)
  {
    VVB B(N, VB(N, false));
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) if (A[i][j] > 0) B[i][j] = true;
    pazzle = A;
    fixed = B;
  }

  bool check_row(int r, int c, int n)
  {
    for (int i = 0; i < N; ++i) if (pazzle[r][i] == n) return false;
    return true;
  }

  bool check_column(int r, int c, int n)
  {
    for (int i = 0; i < N; ++i) if (pazzle[i][c] == n) return false;
    return true;
  }

  bool check_square(int r, int c, int n)
  {
    int rr = r / int(sqrt(N)), cc = c / int(sqrt(N));
    for (int i = 0; i < int(sqrt(N)); ++i) for (int j = 0; j < int(sqrt(N)); ++j)
    {
      if (pazzle[rr*int(sqrt(N))+i][cc*int(sqrt(N))+j] == n) return false;
    }
    return true;
  }

  bool check_rule(int r, int c, int n)
  {
    return ( check_row(r, c, n) && check_column(r, c, n) && check_square(r, c, n) );
  }

  bool back(int r, int c, int in)
  {
    --c;
    if (c < 0)
    {
      c = N-1;
      --r;
    }
    if (r < 0) return false;
    return solve(r, c, pazzle[r][c]+1, false);
  }

  bool solve(int r, int c, int in, bool is_forward)
  {
    if (c >= N)
    {
      c = 0;
      ++r;
    }
    if (c < 0)
    {
      c = N-1;
      --r;
    }
    if (r >= N) return true;
    if (r < 0) return false;
    if (fixed[r][c])
    {
      if (is_forward) return solve(r, c+1, 1, true);
      else return back(r, c, in);
    }
    if (in > N)
    {
      pazzle[r][c] = 0;
      return back(r, c, in);
    }
    if (check_rule(r, c, in))
    {
      pazzle[r][c] = in;
      return solve(r, c+1, 1, true);
    }
    else return solve(r, c, in+1, is_forward);

    return false;
  }
};

int main()
{
  Table T;
  T.input();
  T.output();
  ENDL
  if (T.solve(0, 0, 1, true)) T.output();
  else cout << "Impossible" << endl;

  return 0;
}