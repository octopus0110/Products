#include "Time.cpp"

#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace std;

#define DEBUG false
#define MODE 0
#define BLANK 3
#define Bomb_Threshold 10000
#define Rate_easy 0.1
#define Rate_normal 0.2
#define Rate_hard 0.3
#define ZENKAKU true
#define SEED 0
#define SEEDFILE "minesweeper_seed.txt"
#define TIMESCALE "milliseconds"
#define CLEAR true

void Clear()
{
  cout << "\x1B[2J";
}

struct Minesweeper
{
  int W, H, bomb, uc;
  bool flag;
  string diff, axiss, axiszs, number;
  vector<vector<int>> table;
  vector<vector<bool>> is_uncovered, flag_table;

  Minesweeper();

  template<class T> int find_index(const vector<T>&, const T&);
  bool is_number(const string&);
  void calc_time(const int&);

  void get_parameter(string inp = "");
  pair<int, int> get_pos();
  void clear_table();
  void set_bomb(const int&, const int&);
  void calculate_number();
  int select(const int&, const int&, const bool&);
  void write_idxw();
  void write_bar();
  void show_number(bool idx = true);
  void show_bomb(bool idx = true);
  void show_table(bool idx = true);
  void Run();
};

Minesweeper::Minesweeper():uc(0), axiss("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), axiszs("ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ"), number("０１２３４５６７８９"){}

template<class T>
int Minesweeper::find_index(const vector<T>& Array, const T& x)
{
  for (int i = 0; i < Array.size(); ++i) if (Array[i] == x) return i;
  return -1;
}

bool Minesweeper::is_number(const string& S)
{
  for (int i = 0; i < S.size(); ++i)
  {
    int n = S[i] - '0';
    if (n < 0 || n > 9) return false;
    if (i == 0 && n == 0) return false;
  }
  return true;
}

void Minesweeper::calc_time(const int& time)
{
  int hh, mm, ss;
  ss = time % 60;
  mm = (time / 60) % 60;
  hh = time / 3600;
  printf("%02d:%02d:%02d\n", hh, mm, ss);
}

  void Minesweeper::get_parameter(string inp)
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    if (inp == "")
    {
      while (true)
      {
        string w, h;
        cout << "H, W = ";
        cin >> h >> w;
        if (!is_number(w) || !is_number(h)) continue;
        W = stoi(w);
        H = stoi(h);
        if (W * H >= 2) break;
      }
      while (true)
      {
        cout << "Difficulty[easy/normal/hard/set] : ";
        cin >> diff;
        if (diff == "easy" || diff == "normal" || diff == "hard" || diff == "set") break;
      }
      if (diff == "easy") bomb = W * H * Rate_easy;
      else if (diff == "normal") bomb = W * H * Rate_normal;
      else if (diff == "hard") bomb = W * H * Rate_hard;
      else if (diff == "set")
      {
        while (true)
        {
          string b;
          cout << "Bomb number : ";
          cin >> b;
          if (!is_number(b)) continue;
          bomb = stoi(b);
          if (0 < bomb && bomb < W*H) break;
        }
      }
    }
  }

  pair<int, int> Minesweeper::get_pos()
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    vector<char> axis(axiss.size());
    string hand;
    int x, y;
    char xc, yc;
    flag = false;
    for (int i = 0; i < axiss.size(); ++i) axis[i] = axiss[i];
    while (true)
    {
      cout << "Position : ";
      cin >> hand;
      if (hand.size() < 2 || hand.size() > 3) continue;
      else if (hand.size() == 3 && hand[2] != 'f') continue;
      else
      {
        xc = hand[0];
        yc = hand[1];
        x = find_index(axis, xc);
        y = find_index(axis, yc);
        if (x < 0 || x >= W) continue;
        if (y < 0 || y >= H) continue;
        if (is_uncovered[y][x]) continue;
        if (hand.size() == 3) flag = true;
        else if (flag_table[y][x])
        {
          cout << "Flagged!!" << endl;
          continue;
        }
        break;
      }
    }
    return {x, y};
  }

  void Minesweeper::clear_table()
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    vector<vector<int>> tmp(H, vector<int>(W, -1));
    vector<vector<bool>> tmp2(H, vector<bool>(W, false)), tmp3(H, vector<bool>(W, false));
    table = tmp;
    is_uncovered = tmp2;
    flag_table = tmp3;
  }

  void Minesweeper::set_bomb(const int& x, const int& y)
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    random_device rnd;
    long long seed;
    if (SEED == 0) seed = rnd();
    else seed = SEED;
    ofstream ofs(SEEDFILE, ios::app);
    Time T;
    ofs << seed << " " << "H" << H << "W" << W << " " << diff << " " << "MODE" << MODE << " " << T.getDatetimeStr() << endl;
    mt19937 mt(seed);
    int ct = 0, flag = 0;
    if (MODE == 0)
    {
      table[y][x] = 0;
      is_uncovered[y][x] = true;
    }
    while (ct < bomb)
    {
      int xx, yy;
      xx = mt() % W;
      yy = mt() % H;
      if (table[yy][xx] == -1)
      {
        if (MODE == 1)
        {
          table[yy][xx] = -2;
          ++ct;
        }
        else if ((xx < x-BLANK || x+BLANK < xx) || (yy < y-BLANK || y+BLANK < yy))
        {
          table[yy][xx] = -2;
          ++ct;
        }
        else if (flag > Bomb_Threshold)
        {
          --table[yy][xx];
          ++ct;
        }
        else ++flag;
      }
    }
  }

  void Minesweeper::calculate_number()
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    vector<int> stepx = {0, 0, 1, 1, 1, -1, -1, -1}, stepy = {1, -1, 0, 1, -1, 0, 1, -1};
    for (int y = 0; y < H; ++y) for (int x = 0; x < W; ++x)
    {
      if (table[y][x] == -2) continue;
      table[y][x] = 0;
      for (int i = 0; i < 8; ++i)
      {
        int xx, yy;
        xx = x + stepx[i];
        yy = y + stepy[i];
        if (xx < 0 || xx >= W || yy < 0 || yy >= H) continue;
        if (table[yy][xx] == -2) ++table[y][x];
      }
    }
  }

  int Minesweeper::select(const int& x, const int& y, const bool& f)
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    if (f)
    {
      flag_table[y][x] = !flag_table[y][x];
      return 0;
    }
    is_uncovered[y][x] = true;
    ++uc;
    if (table[y][x] == -2) return -1;
    if (table[y][x] > 0) return 0;
    vector<int> stepx = {0, 0, 1, 1, 1, -1, -1, -1}, stepy = {1, -1, 0, 1, -1, 0, 1, -1};
    queue<pair<int, int>> q;
    q.push({x, y});
    while (!q.empty())
    {
      int xx, yy;
      xx = q.front().first;
      yy = q.front().second;
      q.pop();
      for (int i = 0; i < 8; ++i)
      {
        int xxx, yyy;
        xxx = xx + stepx[i];
        yyy = yy + stepy[i];
        if (xxx < 0 || xxx >= W || yyy < 0 || yyy >= H) continue;
        if (is_uncovered[yyy][xxx]) continue;
        if (table[yyy][xxx] > 0)
        {
          is_uncovered[yyy][xxx] = true;
          ++uc;
        }
        else if (table[yyy][xxx] == 0)
        {
          is_uncovered[yyy][xxx] = true;
          ++uc;
          q.push({xxx, yyy});
        }
      }
    }
    return 0;
  }

  void Minesweeper::write_idxw()
  {
    if (ZENKAKU)
    {
      cout << "　";
      for (int i = 0; i < W*3; i+=3) cout << axiszs.substr(i, 3);
      cout << endl;
    }
    else
    {
      cout << "  ";
      for (int i = 0; i < W; ++i) cout << axiss[i];
      cout << endl;
    }
  }

  void Minesweeper::write_bar()
  {
    if (ZENKAKU)
    {
      for (int i = 0; i < W + 2; ++i) cout << "──";
      cout << endl;
    }
    else
    {
      for (int i = 0; i < W + 4; ++i) cout << "-";
      cout << endl;
    }
  }

  void Minesweeper::show_number(bool idx)
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    write_bar();
    if (idx) write_idxw();
    for (int i = 0; i < W; ++i)
    {
      if (idx) cout << axiss[i] << " ";
      for (int j = 0; j < H; ++j)
      {
        if (table[j][i] != -2) cout << table[j][i] << " ";
        else cout << "b" << " ";
      }
      if (idx) cout << " " << axiss[i];
      cout << endl;
    }
    if (idx) write_idxw();
    write_bar();
  }

  void Minesweeper::show_bomb(bool idx)
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    write_bar();
    if (idx) write_idxw();
    if (ZENKAKU)
    {
      for (int y = 0; y < H; ++y)
      {
        if (idx) cout << axiszs.substr(y*3, 3);
        for (int x = 0; x < W; ++x)
        {
          if (table[y][x] == -2) cout << "※";
          else cout << "・";
        }
        if (idx) cout << axiszs.substr(y*3, 3);
        cout << endl;
      }
    }
    else
    {
      for (int y = 0; y < H; ++y)
      {
        if (idx) cout << axiss[y] << " ";
        for (int x = 0; x < W; ++x)
        {
          if (table[y][x] == -2) cout << "@";
          else cout << ".";
        }
        if (idx) cout << " " << axiss[y];
        cout << endl;
      }
    }
    if (idx) write_idxw();
    write_bar();
  }

  void Minesweeper::show_table(bool idx)
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    write_bar();
    if (idx) write_idxw();
    if (ZENKAKU)
    {
      for (int y = 0; y < H; ++y)
      {
        if (idx) cout << axiszs.substr(y*3, 3);
        for (int x = 0; x < W; ++x)
        {
          if (is_uncovered[y][x])
          {
            if (table[y][x] > 0) cout << number.substr(table[y][x]*3, 3);
            else cout << "　";
          }
          else if (flag_table[y][x]) cout << "\033[31m■\033[0m";
          else cout << "□";
        }
        if (idx) cout << axiszs.substr(y*3, 3);
        cout << endl;
      }
    }
    else
    {
      for (int y = 0; y < H; ++y)
      {
        if (idx) cout << axiss[y] << " ";
        for (int x = 0; x < W; ++x)
        {
          if (is_uncovered[y][x])
          {
            if (table[y][x] > 0) cout << table[y][x];
            else cout << " ";
          }
          else if (flag_table[y][x]) cout << "+";
          else cout << ".";
        }
        if (idx) cout << " " << axiss[y];
        cout << endl;
      }
    }
    if (idx) write_idxw();
    write_bar();
  }

  void Minesweeper::Run()
  {
    if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
    int ct = 0, x, y;
    Time T(TIMESCALE);
    get_parameter();
    clear_table();
    if (CLEAR) Clear();
    show_table();
    T.start();
    T.print_time();
    while (uc < H * W - bomb)
    {
      pair<int, int> hand;
      T.elapse();
      if (ct > 0) T.print_time();
      hand = get_pos();
      x = hand.first;
      y = hand.second;
      if (ct == 0)
      {
        set_bomb(x, y);
        calculate_number();
      }
      if (select(x, y, flag) == -1)
      {
        T.elapse();
        show_bomb();
        T.print_time();
        cout << "GAME OVER!!" << endl;
        return;
      }
      if (CLEAR) Clear();
      show_table();
      ++ct;
    }
    T.elapse();
    if (CLEAR) Clear();
    show_bomb();
    T.print_time();
    cout << "GAME CLEAR!!" << endl;
  }

int main()
{
  Minesweeper M;
  M.Run();

  return 0;
}
