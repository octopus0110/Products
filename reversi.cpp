// #include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <cmath>
#include <ios>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <stack>
#include <numeric>
#include <fstream>
// #include <windows.h>
using namespace std;
template<class T> using V = vector<T>;
template<class T> using VV = V<V<T>>;
template<class T> using VVV = V<VV<T>>;
template<class T1, class T2> using P = pair<T1, T2>;
using I = int;
using D = double;
using B = bool;
using C = char;
using S = string;
using LL = long long;
using LD = long double;
using ULL = unsigned long long;
using PII = P<I, I>;
using VPII = V<PII>;
using PLL = P<LL, LL>;
using VPLL = V<PLL>;
using VI = V<I>;
using VVI = VV<I>;
using VLL = V<LL>;
using VVLL = VV<LL>;
using VC = V<C>;
using VVC = VV<C>;
using VS = V<S>;
using VVS = VV<S>;
using VB = V<B>;
using VVB = VV<B>;
#define REP(type, i, n) for (type i = 0; i < (type)(n); ++i)
#define REP2(type, i, m, n) for (type i = (m); i < (type)(n); ++i)
#define REPR(type, i, n) for (type i = (n)-1; i >= 0; --i)
#define REPR2(type, i, m, n) for (type i = (n)-1; i >= (m); --i)
#define REPx(x, a) for(auto x : a)
#define ALL(a) a.begin(), a.end()
#define SORT(a) sort(ALL(a))
#define SORTR(a, type) sort(ALL(a), greater<type>())
#define SORTF(a, comp) sort(ALL(a), comp)
#define REVERSE(a) reverse(ALL(a))
#define SIZE(a, type) ((type)(a).size())
#define bit_search(bit, n) REP(LL, bit, 1LL<<(n))
#define bit_check(bit, i) ((bit>>(i)) & 1)
#define setpre(n) fixed << setprecision((n))
#define UNIQUE(a) do {SORT(a); (a).erase(unique(ALL(a)), (a).end());} while(0)
#define MAX(a) *max_element(ALL(a))
#define MIN(a) *min_element(ALL(a))
#define bisect_left(a, x) (lower_bound(ALL(a), (x)) - a.begin())
#define bisect_right(a, x) (upper_bound(ALL(a), (x)) - a.begin())
#define INPUT(a) REPx(&x, a) cin >> x;
#define INPUT2(a) REPx(&x, a) INPUT(x);
#define INPUTP(a) REPx(&x, a) cin >> x.first >> x.second;
#define ENDL cout << endl;

// const int INF = 2e9;
const LL INF = 9e18;
const LL MOD = 1e9+7;

template<class T> using PRIORITY_QUEUE = priority_queue< T, V<T>, greater<T> >;
template<class T> inline bool chmin(T &a, T b){if (a > b) {a = b; return true;} return false;}
template<class T> inline bool chmax(T &a, T b){if (a < b) {a = b; return true;} return false;}
template<class T> inline void debug1(V<T> A){REP(int, i, SIZE(A, int)){if (A[i] == INF) cout << "I ";else cout << A[i] << " ";}ENDL}
template<class T> inline void debug2(VV<T> A){REP(int, i, SIZE(A, int)){REP(int, j, SIZE(A[i], int)){if (A[i][j] == INF) cout << "I "; else cout << A[i][j] << " ";}ENDL}}

struct Board
{
  int n, winner, black, white, ct_put, ct_is_possible;
  int v_corner=-2, v_x=5, v_next_to_corner=3;
  bool color, hint, is_back, is_give_up, mode;
  string symbol_black = "●", symbol_white = "○", symbol_space = "・", symbol_hint = "＊";
  VVI table, value;
  string log = "";
  VS number = {"１", "２", "３", "４", "５", "６", "７", "８"};
  VS alphabet = {"ａ", "ｂ", "ｃ", "ｄ", "ｅ", "ｆ", "ｇ", "ｈ"};
  VS WB = {"Ｗ", "Ｂ", "Ｆ"};
  VB flg = {0, 0, 0, 0, 0, 0, 0, 0};
  stack<VVI> log_table;
  stack<bool> log_color;

  Board (int _n=8): n(_n), table(_n, VI(_n, -1)), value(_n, VI(_n, 0))
  {
      table[n/2-1][n/2-1] = 0;
      table[n/2][n/2] = 0;
      table[n/2-1][n/2] = 1;
      table[n/2][n/2-1] = 1;
      value[0][0] = v_corner;
      value[0][n-1] = v_corner;
      value[n-1][0] = v_corner;
      value[n-1][n-1] = v_corner;
      value[1][1] = v_x;
      value[1][n-2] = v_x;
      value[n-2][1] = v_x;
      value[n-2][n-2] = v_x;
      value[0][1] = v_next_to_corner;
      value[1][0] = v_next_to_corner;
      value[0][n-2] = v_next_to_corner;
      value[1][n-1] = v_next_to_corner;
      value[n-2][0] = v_next_to_corner;
      value[n-1][1] = v_next_to_corner;
      value[n-1][n-2] = v_next_to_corner;
      value[n-2][n-1] = v_next_to_corner;
      color = true;
      is_give_up = false;
      is_back = false;
      ct_put = 4;
      black = 2;
      white = 2;
      push_log();
    }

    bool check_right(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (r+1 >= n) return false;
      if (t[c][r+1] == _color || t[c][r+1] == -1) return false;
      REP2(int, i, r+2, n)
      {
        if (t[c][i] == -1) return false;
        if (t[c][i] == _color) return true;
      }
      return false;
    }

    bool check_left(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (r-1 < 0) return false;
      if (t[c][r-1] == _color || t[c][r-1] == -1) return false;
      REPR2(int, i, 0, r-1)
      {
        if (t[c][i] == -1) return false;
        if (t[c][i] == _color) return true;
      }
      return false;
    }

    bool check_up(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (c-1 < 0) return false;
      if (t[c-1][r] == _color || t[c-1][r] == -1) return false;
      REPR2(int, i, 0, c-1)
      {
        if (t[i][r] == -1) return false;
        if (t[i][r] == _color) return true;
      }
      return false;
    }

    bool check_down(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (c+1 >= n) return false;
      if (t[c+1][r] == _color || t[c+1][r] == -1) return false;
      REP2(int, i, c+2, n)
      {
        if (t[i][r] == -1) return false;
        if (t[i][r] == _color) return true;
      }
      return false;
    }

    bool check_upright(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (c-1 < 0 || r+1 >= n) return false;
      if (t[c-1][r+1] == _color || t[c-1][r+1] == -1) return false;
      REP2(int, i, 2, n)
      {
        if (c-i < 0 || r+i >= n) return false;
        if (t[c-i][r+i] == -1) return false;
        if (t[c-i][r+i] == _color) return true;
      }
      return false;
    }

    bool check_upleft(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (c-1 < 0 || r-1 < 0) return false;
      if (t[c-1][r-1] == _color || t[c-1][r-1] == -1) return false;
      REP2(int, i, 2, n)
      {
        if (c-i < 0 || r-i < 0) return false;
        if (t[c-i][r-i] == -1) return false;
        if (t[c-i][r-i] == _color) return true;
      }
      return false;
    }

    bool check_downright(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (c+1 >= n || r+1 >= n) return false;
      if (t[c+1][r+1] == _color || t[c+1][r+1] == -1) return false;
      REP2(int, i, 2, n)
      {
        if (c+i >= n || r+i >= n) return false;
        if (t[c+i][r+i] == -1) return false;
        if (t[c+i][r+i] == _color) return true;
      }
      return false;
    }

    bool check_downleft(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (c+1 >= n || r-1 < 0) return false;
      if (t[c+1][r-1] == _color || t[c+1][r-1] == -1) return false;
      REP2(int, i, 2, n)
      {
        if (c+i >= n || r-i < 0) return false;
        if (t[c+i][r-i] == -1) return false;
        if (t[c+i][r-i] == _color) return true;
      }
      return false;
    }

    bool check(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      reset_flg();
      flg[0] = check_right(t, c, r, _color);
      flg[1] = check_left(t, c, r, _color);
      flg[2] = check_up(t, c, r, _color);
      flg[3] = check_down(t, c, r, _color);
      flg[4] = check_upright(t, c, r, _color);
      flg[5] = check_upleft(t, c, r, _color);
      flg[6] = check_downright(t, c, r, _color);
      flg[7] = check_downleft(t, c, r, _color);
      if (get_sum(flg)) return true;
      else return false;
    }

    void count_is_possible(const VVI& t, const bool& _color)
    {
      ct_is_possible = 0;
      REP(int, i, n) REP(int, j, n)
      {
        if (t[i][j] != -1) continue;
        if (check(t, i, j, _color)) ++ct_is_possible;
      }
    }

    VVB make_is_possible_table(const VVI& t, const bool& _color)
    {
      VVB is_possible_table(n, VB(n, false));
      REP(int, i, n) REP(int, j, n) is_possible_table[i][j] = false;
      REP(int, i, n) REP(int, j, n)
      {
        if (t[i][j] != -1) continue;
        if (check(t, i, j, _color)) is_possible_table[i][j] = true;
      }
      return is_possible_table;
    }

    VVI make_hint(const VVI& t, const bool& _color)
    {
      VVI ret = t;
      VVB is_possible_table = make_is_possible_table(ret, _color);
      REP(int, i, n) REP(int, j, n) if (is_possible_table[i][j]) ret[i][j] = 2;
      return ret;
    }

    void write_upside(const int& N)
    {
      cout << "┌";
      REP(int, i, N) cout << "─";
      cout << "┐" << endl;
    }

    void write_downside(const int& N)
    {
      cout << "└";
      REP(int, i, N) cout << "─";
      cout << "┘" << endl;
    }

    void write_ceil()
    {
      cout << "┌─┬";
      REP(int, i, n) cout << "─";
      cout << "┐" << endl;
    }

    void write_row(const int& _color)
    {
      cout << "│" << WB[_color] << "│";
      REP(int, i, n) cout << alphabet[i];
      cout << "│" << endl;
    }

    void write_wall()
    {
      cout << "├─┼";
      REP(int, i, n) cout << "─";
      cout << "┤" << endl;
    }

    void write_floor()
    {
      cout << "└─┴";
      REP(int, i, n) cout << "─";
      cout << "┘" << endl;
    }

    void write_body(const VVI& t)
    {
      int c = -1;
      REPx(x, t)
      {
        cout << "│" << number[++c] << "│";
        REPx(y, x)
        {
          if (y == -1) cout << symbol_space;
          else if (y == 0) cout << symbol_black;
          else if (y == 1) cout << symbol_white;
          else cout << symbol_hint;
        }
        cout << "│" << endl;
      }
    }

    void write_board(const int& _color, bool _hint=false)
    {
      VVI t;
      if (_hint) t = make_hint(table, _color);
      else t = table;
      cout << endl;
      write_ceil();
      write_row(_color);
      write_wall();
      write_body(t);
      write_floor();
    }

    void reset_flg()
    {
      REP(int, i, n) flg[i] = false;
    }

    PII convert_str(const string& s)
    {
      int r = s[0] - 'a';
      int c = s[1] - '1';
      return {c, r};
    }

    string convert_int(int c, int r)
    {
      string hand = "abcdefgh"[r]+to_string(c+1);
      return hand;
    }

    VVI put(const VVI& t, const int& c, const int& r, const bool& _color)
    {
      VVI ret = t;
      if (!(0 <= r && r < n && 0 <= c && c < n)) return ret;
      ret[c][r] = _color;
      return ret;
    }

    bool flip_right(VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (flg[0])
      {
        REP2(int, i, r+1, n)
        {
          if (t[c][i] == _color) break;
          t[c][i] = !t[c][i];
        }
        return true;
        }
      else return false;
    }

    bool flip_left(VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (flg[1])
      {
        REPR2(int, i, 0, r)
        {
          if (t[c][i] == _color) break;
          t[c][i] = !t[c][i];
        }
        return true;
      }
      else return false;
    }

    bool flip_up(VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (flg[2])
      {
        REPR2(int, i, 0, c)
        {
          if (t[i][r] == _color) break;
          t[i][r] = !t[i][r];
        }
        return true;
      }
      else return false;
    }

    bool flip_down(VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (flg[3])
      {
        REP2(int, i, c+1, n)
        {
          if (t[i][r] == _color) break;
          t[i][r] = !t[i][r];
        }
        return true;
        }
      else return false;
    }

    bool flip_upright(VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (flg[4])
      {
        REP2(int, i, 1, n)
        {
          if (t[c-i][r+i] == _color) break;
          t[c-i][r+i] = !t[c-i][r+i];
        }
        return true;
      }
      else return false;
    }

    bool flip_upleft(VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (flg[5])
      {
        REP2(int, i, 1, n)
        {
          if (t[c-i][r-i] == _color) break;
          t[c-i][r-i] = !t[c-i][r-i];
        }
        return true;
      }
      else return false;
    }

    bool flip_downright(VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (flg[6])
      {
        REP2(int, i, 1, n)
        {
          if (t[c+i][r+i] == _color) break;
          t[c+i][r+i] = !t[c+i][r+i];
        }
        return true;
      }
      else return false;
    }

    bool flip_downleft(VVI& t, const int& c, const int& r, const bool& _color)
    {
      if (flg[7])
      {
        REP2(int, i, 1, n)
        {
          if (t[c+i][r-i] == _color) break;
          t[c+i][r-i] = !t[c+i][r-i];
        }
        return true;
      }
      else return false;
    }

    void flip(VVI& t, const int& c, const int& r, const bool& _color)
    {
      check(t, c, r, _color);
      flip_right(t, c, r, _color);
      flip_left(t, c, r, _color);
      flip_up(t, c, r, _color);
      flip_down(t, c, r, _color);
      flip_upright(t, c, r, _color);
      flip_upleft(t, c, r, _color);
      flip_downright(t, c, r, _color);
      flip_downleft(t, c, r, _color);
    }

    bool is_possible(const VVI& t, const bool& _color)
    {
      count_is_possible(t, _color);
      return ct_is_possible > 0;
    }

    template <class T>
    int get_sum(const V<T>& A)
    {
      int sum = 0;
      REPx(x, A) sum += x;
      return sum;
    }

    template <class T>
    int get_sum2(const VV<T>& A)
    {
      int sum = 0;
      REPx(x, A) sum += get_sum(x);
      return sum;
    }

    void write_log()
    {
      ofstream ofs("./reversi_log.txt");
      ofs << log;
    }

    void push_log()
    {
      log_table.push(table);
      log_color.push(color);
    }

    void pop_log()
    {
      log_table.pop();
      log_color.pop();
      log.pop_back();
      log.pop_back();
    }

    void update()
    {
      table = log_table.top();
      color = log_color.top();
    }

    void write_num_stone()
    {
      black = get_sum2(table)+n*n-ct_put;
      white = ct_put-black;
      cout << "# of BLACK : " << black << endl;
      cout << "# of WHITE : " << white << endl;
    }

    void decide_winner()
    {
      if (is_give_up) return;
      if (black == white) winner = -1;
      else if (black < white) winner = 0;
      else winner = 1;
    }

    void write_result()
    {
      if (winner == -1)
      {
      write_upside(5);
      cout << "│ＤＲＡＷ！│" << endl;
      write_downside(5);
      }
      else
      {
      write_upside(10);
      if (winner == 1) cout << "│ＢＬＡＣＫ　ＷＩＮ！│" << endl;
      else if (winner == 0) cout << "│ＷＨＩＴＥ　ＷＩＮ！│" << endl;
      write_downside(10);
      }
    }

    void pass()
    {
      color = !color;
      cout << endl;
      write_upside(4);
      cout << "│ＰＡＳＳ│" << endl;
      write_downside(4);
    }

    bool back()
    {
      is_back = false;
      if (ct_put == 4) cout << "これ以上戻れません" << endl;
      else if (mode)
      {
        pop_log();
        pop_log();
        write_log();
        update();
        is_back = true;
        --ct_put;
        --ct_put;
      }
      else
      {
        pop_log();
        write_log();
        update();
        is_back = true;
        --ct_put;
      }
      return is_back;
    }

    bool get_hint()
    {
      string ans;
      while (true)
      {
        cout << "手の候補を表示しますか？[y/n]" << endl << ">> ";
        getline(cin, ans);
        transform (ans.cbegin(), ans.cend(), ans.begin(), ::tolower);
        if (ans == "y" || ans == "yes") return true;
        else if (ans == "n" || ans == "no") return false;
      }
    }

    bool get_player_color()
    {
      string ans;
      while (true)
      {
        cout << "黒でプレイしますか？[y/n]" << endl << ">> ";
        getline(cin, ans);
        transform (ans.cbegin(), ans.cend(), ans.begin(), ::tolower);
        if (ans == "y" || ans == "yes") return true;
        else if (ans == "n" || ans == "no") return false;
      }
    }

    bool get_mode()
    {
      string ans;
      while (true)
      {
        cout << "COMと対戦しますか？[y/n]" << endl << ">> ";
        getline(cin, ans);
        transform (ans.cbegin(), ans.cend(), ans.begin(), ::tolower);
        if (ans == "y" || ans == "yes") return true;
        else if (ans == "n" || ans == "no") return false;
      }
    }

    bool check_point(int c, int r)
    {
      if (!(0 <= c && c < n && 0 <= r && r < n)) return false;
      else if (table[c][r] != -1) return false;
      else return true;
    }
};

struct PLAYER : Board
{
  bool color;
  Board* B;
  PLAYER(Board* _B, bool _color): B(_B), color(_color){}

  bool play()
  {
    if (!B->is_possible(B->table, color)) return false;
    B->write_board(color, B->hint);
    string hand;
    cout << WB[color] << " : ";
    getline(cin, hand);
    int c = convert_str(hand).first;
    int r = convert_str(hand).second;
    while (!B->check_point(c, r) || !B->check(B->table, c, r, color))
    {
      if (hand == "back")
      {
        if (B->back()) return true;
      }
      else if (hand == "give up")
      {
        B->winner = !color;
        B->is_give_up = true;
        return true;
      }
      else cout << "置けません" << endl;
      cout << WB[color] << " : ";
      getline(cin, hand);
      c = convert_str(hand).first;
      r = convert_str(hand).second;
    }
    B->table = put(B->table, c, r, color);
    B->flip(B->table, c, r, color);
    B->color = !B->color;
    B->log += hand;
    B->write_log();
    B->push_log();
    ++B->ct_put;
    return true;
  }
};

struct COM : Board
{
  bool color;
  int which;
  Board* B;
  COM(Board* _B, bool _color, int _which): B(_B), color(_color), which(_which){}

  bool play()
  {
    if (!B->is_possible(B->table, color)) return false;
    B->write_board(color, false);
    int c, r;
    PII hand = level(which, color);
    c = hand.first;
    r = hand.second;
    B->table = put(B->table, c, r, color);
    B->flip(B->table, c, r, color);
    B->color = !B->color;
    B->log += convert_int(c, r);
    B->write_log();
    B->push_log();
    ++B->ct_put;
    return true;
  }

  PII level(int which, bool color)
  {
    PII ret;
    if (which == 1) ret = algorithm1(color);
    else if (which == 2)
    {
      if (B->ct_put < double(n)*0.8) ret = algorithm2(color);
      else ret = algorithm1(color);
    }
    return ret;
  }

  PII algorithm1(bool color)
  {
    PII ret;
    VVI t;
    PRIORITY_QUEUE<VI> q;
    VVB is_possible_table1 = B->make_is_possible_table(B->table, color);
    REP(int, i, B->n) REP(int, j, B->n)
    {
      if (!is_possible_table1[i][j]) continue;
      t = B->put(B->table, i, j, color);
      B->flip(t, i, j, color);
      B->count_is_possible(t, !color);
      q.push({B->ct_is_possible+B->value[i][j], i, j});
    }
    ret = {q.top()[1], q.top()[2]};
    return ret;
  }

  PII algorithm2(bool color)
  {
    PII ret;
    priority_queue<VI> q;
    VVB is_possible_table1 = B->make_is_possible_table(B->table, color);
    REP(int, i, B->n) REP(int, j, B->n)
    {
      if (!is_possible_table1[i][j]) continue;
      VVI t = B->put(B->table, i, j, color);
      B->flip(t, i, j, color);
      VVB is_possible_table2 = B->make_is_possible_table(t, !color);
      REP(int, k, B->n) REP(int, l, B->n)
      {
        if (!is_possible_table2[k][l]) continue;
        VVI tt = B->put(t, i, j, !color);
        B->flip(tt, i, j, !color);
        B->count_is_possible(tt, color);
        q.push({B->ct_is_possible+B->value[i][j], i, j});
      }
    }
    ret = {q.top()[1], q.top()[2]};
    return ret;
  }

};

struct Game: Board
{
  Board* B;
  Game(Board* _B): B(_B){}

  void play_player(PLAYER P1, PLAYER P2)
  {
    while(B->ct_put < B->n*B->n)
    {
      if (B->color == P1.color)
      {
        if (!P1.play())
        {
          B->pass();
          if (!P2.play()) break;
        }
        else if (B->is_back)
        {
          B->is_back = false;
          continue;
        }
        else if (B->is_give_up) break;
      }
      else if (B->color == P2.color)
      {
        if (!P2.play())
        {
          B->pass();
          if (!P1.play()) break;
        }
        else if (B->is_back)
        {
          B->is_back = false;
          continue;
        }
        else if (B->is_give_up) break;
      }
    }
    B->write_board(2, false);
    B->write_num_stone();
    B->decide_winner();
    B->write_result();
  }

  void play_com(PLAYER P1, COM P2)
  {
    while(B->ct_put < B->n*B->n)
    {
      if (B->color == P1.color)
      {
        if (!P1.play())
        {
          B->pass();
          if (!P2.play()) break;
        }
        else if (B->is_back)
        {
          B->is_back = false;
          continue;
        }
        else if (B->is_give_up) break;
      }
      else if (B->color == P2.color)
      {
        if (!P2.play())
        {
          B->pass();
          if (!P1.play()) break;
        }
        else if (B->is_back)
        {
          B->is_back = false;
          continue;
        }
        else if (B->is_give_up) break;
      }
    }
    B->write_board(2, false);
    B->write_num_stone();
    B->decide_winner();
    B->write_result();
  }

  void test_com(COM P1, COM P2)
  {
    while(B->ct_put < B->n*B->n)
    {
      if (B->color == P1.color)
      {
        if (!P1.play())
        {
          B->pass();
          if (!P2.play()) break;
        }
        else if (B->is_back)
        {
          B->is_back = false;
          continue;
        }
        else if (B->is_give_up) break;
      }
      else if (B->color == P2.color)
      {
        if (!P2.play())
        {
          B->pass();
          if (!P1.play()) break;
        }
        else if (B->is_back)
        {
          B->is_back = false;
          continue;
        }
        else if (B->is_give_up) break;
      }
    }
    B->write_board(2, false);
    B->write_num_stone();
    B->decide_winner();
    B->write_result();
  }
};

int get_n(bool& test)
{
  string str;
  cout << "一辺のマス数 : ";
  getline(cin, str);
  while(str.size() > 1 || str[0]-'0' < 2 || str[0]-'0' > 8)
  {
    if (str == "test") test = true;
    cout << "一辺のマス数 : ";
    getline(cin, str);
  }
  return str[0]-'0';
}

int get_which()
{
  static int ct = 0;
  int a1, a2;
  string str;
  cout << "アルゴリズム" << ++ct << " : ";
  getline(cin, str);
  while(str.size() > 1 || str[0]-'0' < 1 || str[0]-'0' > 2)
  {
    cout << "アルゴリズム" << ct << " : ";
   getline(cin, str);
  }
  return str[0]-'0';
}

void Reversi(bool test=false)
{
  int n;
  n = get_n(test);
  Board Board(n);
  Game game(&Board);
  if (test)
  {
    int a1, a2;
    bool color_com1, color_com2;
    color_com1 = true;
    color_com2 = !color_com1;
    a1 = get_which();
    a2 = get_which();
    COM com1(&Board, color_com1, a1), com2(&Board, color_com2, a2);
    game.test_com(com1, com2);
  }
  else
  {
    Board.mode = Board.get_mode();
    Board.hint = Board.get_hint();
    if (Board.mode)
    {
      int which;
      bool color_player, color_com;
      which = get_which();
      color_player = Board.get_player_color();
      color_com = !color_player;
      PLAYER player(&Board, color_player);
      COM com(&Board, color_com, 1);
      game.play_com(player, com);
    }
    else
    {
      bool color_player1, color_player2;
      color_player1 = true;
      color_player2 = !color_player1;
      PLAYER player1(&Board, color_player1), player2(&Board, color_player2);
      game.play_player(player1, player2);
    }
  }
}

int main()
{
  Reversi();

  return 0;
}