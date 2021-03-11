/*若干の速度改善
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

#define ENDL cout << endl;

struct Table
{
  int ct = 0;
  int N;
  VV<int> pazzle, last_idx;
  VV<bool> fixed;
  VVV<bool> options;
  VVV<int> options_list;
  bool is_impossible = false;

  Table(){}

  void input(bool write=false)
  {
    cin >> N;
    VV<int> A(N, V<int>(N, 0));
    VV<bool> B(N, V<bool>(N, false));
    VVV<bool> C(N, VV<bool>(N, V<bool>(N, true)));
    VV<int> D(N+1, V<int>(N+1, 0));
    for (int i = 0; i < N; ++i) for(int j = 0; j < N; ++j)
    {
      string tmp;
      cin >> tmp;
      if ((tmp.size() == 1 && '0' < tmp[0] && tmp[0] <= '9') || tmp.size() > 1)
      {
        int n = stoi(tmp);
        A[i][j] = n;
        B[i][j] = true;
        for (int k = 0; k < N; ++k)
        {
          if (k == n-1) continue;
          C[i][j][k] = false;
        }
      }
    }
    pazzle = A;
    fixed = B;
    options = C;
    if (write) output();
    check_options();
    is_impossible = convert_options();
    if (is_impossible) return;
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) D[i][j] = options_list[i][j][0];
    last_idx = D;
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

  void update(VV<int> A)
  {
    VV<bool> B(N, V<bool>(N, false));
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
    for (int i = 0; i < int(sqrt(N)); ++i) for (int j = 0; j < int(sqrt(N)); ++j) if (pazzle[rr*int(sqrt(N))+i][cc*int(sqrt(N))+j] == n) return false;
    return true;
  }

  bool check_rule(int r, int c, int n)
  {
    return ( check_row(r, c, n) && check_column(r, c, n) && check_square(r, c, n) );
  }

  void modify_rc(int& r, int& c)
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
  }

  int get_number(V<bool> A)
  {
    for (int i = 0; i < N; ++i) if (A[i]) return i+1;
  }

  void fix_option(int r, int c)
  {
    int sum = 0;
    for (int i = 0; i < N; ++i) sum += options[r][c][i];
    if (sum == 1)
    {
      pazzle[r][c] = get_number(options[r][c]);
      fixed[r][c] = true;
    }
  }

  void check_option(int r, int c, bool fix=true)
  {
    if (fixed[r][c]) return;
    for(int k = 0; k < N; ++k) if (options[r][c][k]) options[r][c][k] = check_rule(r, c, k+1);
    if (fix) fix_option(r, c);
  }

  void check_options(bool fix=true, int roop=1)
  {
    for (int ct = 0; ct < roop; ++ct)
    {
      for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) check_option(i, j, fix);
    }
  }

  bool convert_options()
  {
    VVV<int> A(N, VV<int>(N, V<int>()));
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        for (int k = 0; k < N; ++k)
        {
          if (options[i][j][k]) A[i][j].emplace_back(k+1);
        }
        if (A[i][j].empty()) return true;
      }
    }
    options_list = A;
    return false;
  }

  bool backward(int r, int c)
  {
    last_idx[r][c] = 0;
    if (!fixed[r][c]) pazzle[r][c] = 0;
    --c;
    modify_rc(r, c);
    if (r < 0) return false;
    if (fixed[r][c]) return backward(r, c);
    return solve(r, c, last_idx[r][c]+1, false);
  }

  bool forward(int r, int c, int idx)
  {
    last_idx[r][c] = idx;
    ++c;
    modify_rc(r, c);
    if (r >= N) return true;
    if (fixed[r][c]) return forward(r, c, 0);
    return solve(r, c, 0, true);
  }

  bool solve(int r, int c, int idx, bool is_forward=true)
  {
    if (is_impossible) return false;
    modify_rc(r, c);
    if (r >= N) return true;
    if (r < 0) return false;
    if (fixed[r][c])
    {
      if (is_forward) return forward(r, c, idx);
      else return backward(r, c);
    }
    if (idx >= options_list[r][c].size()) return backward(r, c);
    if (check_rule(r, c, options_list[r][c][idx]))
    {
      pazzle[r][c] = options_list[r][c][idx];
      return forward(r, c, idx);
    }
    else return solve(r, c, idx+1, is_forward);

    return false;
  }

  // デバッグ用
  void output_options()
  {
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        for (int k = 0; k < N; ++k)
        {
          cout << options[i][j][k];
        }
        cout << " ";
      }
      ENDL
    }
  }

  void output_options_list()
  {
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        for (int k : options_list[i][j])
        {
          cout << k << ",";
        }
        cout << " ";
      }
      ENDL
    }
  }

  void output_fixed()
  {
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        cout << fixed[i][j] << " ";
      }
    ENDL
    }
  }

  void output_last_idx()
  {
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        cout << last_idx[i][j] << " ";
      }
      ENDL
    }
  }
};

int main()
{
  Table T;
  T.input(1);
  ENDL
  if (T.solve(0, 0, 0)) T.output();
  else cout << "Impossible" << endl;

  return 0;
}