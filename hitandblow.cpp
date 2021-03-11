#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <time.h>
using namespace std;
template<class T> using V = vector<T>;
template<class T> using VV = V<V<T>>;
template<class T> using VVV = V<VV<T>>;

#define ENDL cout << endl;

struct HitandBlow
{
  bool give_up = false;
  int N, T, hit, blow, digit_N, digit_T, edge;
  VV<int> log_input;
  V<int> input, target, log_hit, log_blow;

  HitandBlow(int _edge=2): edge(_edge){}

  int make_random(int n)
  {
    random_device rnd;
    int a = rnd()/485139, b = rnd()/213092, c = rnd()/7411;
    return (a * b + c) % n;
  }

  void make_target(bool write=true)
  {
    int ct = 0, idx = -1;
    V<int> res(N, 0);
    V<bool> judge(N, false);
    while (ct < N)
    {
      int tmp = make_random(10);
      if (!judge[tmp])
      {
        judge[tmp] = true;
        res[++idx] = tmp;
        ++ct;
      }
    }
    target = res;
    if (write)
    {
      ofstream ofs("./target.txt");
      for (int i = 0; i < N; ++i) ofs << target[i] << " ";
      ofs << endl;
    }
  }

  V<int> compare(V<int> original, V<int> suggestion, bool update=false)
  {
    int h = 0; int b = 0;
    V<bool> O(10, false), S(10, false);
    for (int i = 0; i < N; ++i)
    {
      O[original[i]] = true;
      S[suggestion[i]] = true;
      if (original[i] == suggestion[i])
      {
        O[original[i]] = false;
        S[suggestion[i]] = false;
        ++h;
      }
    }
    for (int i = 0; i < 10; ++i) if (O[i] && S[i]) ++b;
    if (update)
    {
      hit = h;
      blow = b;
      ++T;
      digit_T = log10(T) + 1;
      add_logs();
    }
    return {h, b};
  }

  void write_header(int which, int edge=2, string who="YOU")
  {
    int width;
    who = "(" + who + ")";
    if (which == 0)
    {
      for (int i = 0; i < edge; ++i) cout << " ";
      width = max(int(who.size()), digit_T)+2*edge;
      cout << who;
      for (int i = 0; i < width-who.size(); ++i) cout << " ";
    }
    if (which == 1)
    {
      width = max(2*N-1+edge*2, 5+edge*2);
      for (int i = 0; i < edge; ++i) cout << " ";
      cout << "input";
      for (int i = 0; i < width-edge-5; ++i) cout << " ";
    }
    if (which == 2)
    {
      width = max(digit_N+edge*2, 3+edge*2);
      for (int i = 0; i < edge; ++i) cout << " ";
      cout << "hit";
      for (int i = 0; i < width-edge-3; ++i) cout << " ";
    }
    if (which == 3)
    {
      width = max(digit_N+edge*2, 4+edge*2);
      for (int i = 0; i < edge; ++i) cout << " ";
      cout << "blow";
      for (int i = 0; i < width-edge-4; ++i) cout << " ";
    }
    cout << "|";
  }

  void write_log(int which, int ct, int edge=2, string who="YOU")
  {
    int width;
    who = "(" + who + ")";
    if (which == 0)
    {
      for (int i = 0; i < edge; ++i) cout << " ";
      int digit_ct = log10(ct)+1;
      width = max(int(who.size()), digit_T)+2*edge;
      for (int i = 0; i < width-digit_ct-edge; ++i) cout << " ";
      cout << ct;
      for (int i = 0; i < edge; ++i) cout << " ";
    }
    if (which == 1)
    {
      width = max(2*N-1+edge*2, 5+edge*2);
      for (int i = 0; i < edge; ++i) cout << " ";
      for (int i = 0; i < width-2*edge-2*N+1; ++i) cout << " ";
      for (int i = 0; i < N; ++i)
      {
        cout << log_input[ct-1][i];
        if (i < N-1) cout << " ";
      }
      for (int i = 0; i < edge; ++i) cout << " ";
    }
    if (which == 2)
    {
      width = max(digit_N+edge*2, 3+edge*2);
      for (int i = 0; i < edge; ++i) cout << " ";
      for (int i = 0; i < width-2*edge-digit_N; ++i) cout << " ";
      cout << log_hit[ct-1];
      for (int i = 0; i < edge; ++i) cout << " ";
    }
    if (which == 3)
    {
      width = max(digit_N+edge*2, 4+edge*2);
      for (int i = 0; i < edge; ++i) cout << " ";
      for (int i = 0; i < width-2*edge-digit_N; ++i) cout << " ";
      cout << log_blow[ct-1];
      for (int i = 0; i < edge; ++i) cout << " ";
    }
    cout << "|";
  }

  void write_logs(int edge=2, string who="YOU")
  {
    cout << "|";
    for (int i = 0; i < 4; ++i) write_header(i, edge, who);
    ENDL
    for (int i = 0; i < T; ++i)
    {
      cout << "|";
      for (int j = 0; j < 4; ++j) write_log(j, i+1, edge, who);
      ENDL
    }
  }

  void write_logs_withCPU(HitandBlow cpu, int edge=2)
  {
    cout << "|";
    for (int i = 0; i < 4; ++i) write_header(i, edge, "YOU");
    for (int i = 0; i < edge*2; ++i) cout << " ";
    cout << "|";
    for (int i = 0; i < 4; ++i) cpu.write_header(i, edge, "CPU");
    ENDL
    for (int i = 0; i < T; ++i)
    {
      cout << "|";
      for (int j = 0; j < 4; ++j) write_log(j, i+1, edge, "YOU");
      if (cpu.T > i)
      {
        for (int i = 0; i < edge*2; ++i) cout << " ";
        cout << "|";
        for (int j = 0; j < 4; ++j) cpu.write_log(j, i+1, edge, "CPU");
      }
      ENDL
    }
  }

  void add_logs()
  {
    log_hit.emplace_back(hit);
    log_blow.emplace_back(blow);
    log_input.emplace_back(input);
  }

  void update_parameter(V<int> t)
  {
    target = t;
    N = t.size();
    digit_N = log10(N)+1;
  }

  void get_N(bool get=true, int n=0)
  {
    if (get)
    {
      string line;
      cout << "桁数を入力してください" << endl << ">> ";
      while(true)
      {
        if (get) getline(cin, line);
        if (line.size() > 1)
        {
          cerr << "9以下の数字を入力してください" << endl << ">> ";
          continue;
        }
        char num = line[0];
        if (num < '0' || '9' < num)
        {
          cerr << "数字以外の文字が入力されています" << endl << ">> ";
          continue;
        }
        N = num - '0';
        break;
        get = false;
      }
    }
    else N = n;
    digit_N = log10(N)+1;
    V<int> in(N);
    input = in;
  }

  void get_input()
  {
    string line;
    if (T == 0) cout << N << "桁の数字を空白区切りで入力してください" << endl;
    cout << ">> ";
    int ct = 0;
    while(ct != N)
    {
      ct = 0;
      getline(cin, line);
      if (line == "give up")
      {
        give_up = true;
        break;
      }
      if (line.size() < 2*N-1)
      {
        cerr << "入力された文字数が少ないです" << endl << ">> ";
        continue;
      }
      V<bool> judge(10, false);
      for (int i = 0; i < N; ++i)
      {
        int idx = i*2;
        char num = line[idx];
        if (num < '0' || '9' < num)
        {
          cerr << "数字以外の文字が入力されています" << endl << ">> ";
          break;
        }
        if (judge[num-'0'])
        {
          cerr << "数字が重複しています" << endl << ">> ";
          break;
        }
        judge[num-'0'] = true;
        input[i] = num-'0';
        ++ct;
      }
    }
  }

  void game(HitandBlow cpu, bool get=true, int n=-1, bool with_cpu=false)
  {
    T = 0;
    if (!get) cout << endl << "======== 推測した数列を入力してください(あなたのターン) ========" << endl;
    get_N(get, n);
    make_target();
    time_t start = time(NULL);
    while(true)
    {
      get_input();
      if (give_up) break;
      compare(target, input, true);
      if (with_cpu) write_logs_withCPU(cpu, edge);
      else write_logs(edge);
      if (hit == N) break;
      if (T > 100) break;
    }
    time_t end = time(NULL);
    if (give_up) T = 99;
    cout << "GAME OVER" << endl;
    cout << "TIME : " << (end-start) << " s" << endl;
  }

// デバッグ用
  void write_input()
  {
    for (int i = 0; i < N; ++i) cout << input[i] << " ";
    ENDL
  }

  void write_target()
  {
    for (int i = 0; i < N; ++i) cout << target[i] << " ";
    ENDL
  }
};

struct CPU : HitandBlow
{
  int last_num = 0;
  VV<bool> options;

  CPU(bool flg)
  {
    if (flg)
    {
      T = 0;
      cout << endl << "======== CPUに渡す数列を入力してください(CPUのターン) ========" << endl;
      get_N();
      get_input();
      target = input;
      VV<bool> tmp(N, V<bool>(10, true));
      options = tmp;
    }
  }

  bool first_step_rationally()
  {
    int sum = 0, ct = 0;
    for (int i = 0; i < 10/N; ++i)
    {
      ct += N;
      V<int> tmp;
      for (int j = i*N; j < i*N+N; ++j)
      {
        int k = j;
        if (k > 9) k -= 10;
        tmp.emplace_back(k);
      }
      input = tmp;
      compare(target, input, true);
      if (hit == N) return true;
      if (hit+blow == 0) for (int j = 0; j < N; ++j) for (int k = 0; k < N; ++k) options[j][input[k]] = false;
      sum += hit + blow;
      if (sum >= N) break;
    }
    if (ct < 10) for (int i = ct; i < 10; ++i) for (int j = 0; j < N; ++j) options[j][i] = false;
    digit_T = log10(T)+1;
    return false;
  }

  bool is_in_array(V<int> a, VV<int> A)
  {
    for (V<int> aa : A) if (a == aa) return true;
    return false;
  }

  bool first_step_randomly()
  {
    VV<int> judge;
    int sum = 0, occur=floor(10.0/double(N));
    for (int i = 0; i < occur; ++i)
    {
      int ct = -1;
      bool flg = true;
      V<int> tmp(N, 0);
      V<bool> judge2(10, false);
      while(ct < N-1)
      {
        int number = make_random(10);
        if (judge2[number]) continue;
        judge2[number] = true;
        tmp[++ct] = number;
      }
      if (is_in_array(tmp, judge))
      {
        --i;
        continue;
      }
      input = tmp;
      compare(target, input, true);
      if (hit == N) return true;
      if (hit+blow == 0) for (int j = 0; j < N; ++j) for (int k = 0; k < N; ++k) options[j][input[k]] = false;
      sum += hit + blow;
      if (sum >= N) break;
    }
    return false;
  }

  bool first_step_randomly2()
  {
    VV<int> judge;
    for (int i = 0; i < N; ++i)
    {
      int ct = -1;
      bool flg = true;
      V<int> tmp(N, 0);
      V<bool> judge2(10, false);
      while(ct < N-1)
      {
        int number = make_random(10);
        if (judge2[number]) continue;
        judge2[number] = true;
        tmp[++ct] = number;
      }
      if (is_in_array(tmp, judge))
      {
        --i;
        continue;
      }
      input = tmp;

      compare(target, input, true);
      if (hit == N) return true;
      if (hit+blow == 0) for (int j = 0; j < N; ++j) for (int k = 0; k < N; ++k) options[j][input[k]] = false;
    }
    return false;
  }

  bool first_step(int which)
  {
    if (which == 0) return first_step_rationally();
    else if (which == 1 || which == 2) return first_step_randomly();
    else if (which == 3) return first_step_randomly2();
  }

  void make_array(int depth, V<int>& A, int add)
  {
    A[depth] = add;
    if (add > 9)
    {
      --depth;
      if (depth < 0) return;
      make_array(depth, A, A[depth]+1);
    }
    else if (depth == N-1)
    {
      // for (int i = 0; i < N; ++i) cout << A[i] << " ";
      // ENDL
      bool flg = true;
      V<bool> judge(N, false);
      for (int i = 0; i < T; ++i)
      {
        if (A == log_input[i])
        {
          flg = false;
          break;
        }
        V<int> tmp = compare(A, log_input[i]);
        int h, b;
        h = tmp[0]; b = tmp[1];
        if (h != log_hit[i] || b != log_blow[i])
        {
          flg = false;
          break;
        }
      }
      for (int i = 0; i < N; ++i)
      {
        if (judge[A[i]])
        {
          flg = false;
          break;
        }
        judge[A[i]] = true;
      }
      if (flg) return;
      if (add < 9) make_array(depth, A, add+1);
      else make_array(N-2, A, A[N-2]+1);

    }
    else  make_array(depth+1, A, 0);
  }

  void make_array2()
  {
    for (int i = last_num+1; i < int(pow(10, N)); ++i)
    {
      bool flg = false;
      V<bool> judge(10, false);
      for (int j = 0; j < N; ++j)
      {
        int k = i % (int(pow(10, N-j))) / int(pow(10, N-j-1));
        if (judge[k])
        {
          flg = true;
          break;
        }
        judge[k] = true;
        input[j] = k;
      }
      if (flg) continue;
      flg = true;
      for (int j = 0; j < T; ++j)
      {
        if (input == log_input[j])
        {
          flg = false;
          break;
        }
        V<int> tmp = compare(input, log_input[j]);
        int h, b;
        h = tmp[0]; b = tmp[1];
        if (h != log_hit[j] || b != log_blow[j])
        {
          flg = false;
          break;
        }
      }
      if (flg)
      {
        last_num = i;
        break;
      }
    }
  }

  void solve()
  {
    int which = make_random(4);
    if (!first_step(which))
    {
      while(hit != N)
      {
        // make_array(0, input, 0);
        make_array2();
        compare(target, input, true);
      }
    }
  }

// デバッグ用
  void write_options()
  {
    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < 10; ++j) cout << options[i][j];
      ENDL
    }
  }
};

int main()
{
  int edge = 1;
  bool with_cpu;
  string ans;
  cout << "CPUと対戦しますか?[y/n]" << endl << ">> ";
  getline(cin,ans);
  with_cpu = !(ans == "n" || ans == "N" || ans == "no" || ans == "NO" || ans == "nO" || ans == "No");
  CPU C(with_cpu);
  if (with_cpu) C.solve();
  HitandBlow HB(edge);
  HB.game(C, !with_cpu, C.N, with_cpu);

  if (with_cpu)
  {
    ENDL
    C.write_logs(edge, "CPU");
  }
  ENDL
  if (HB.give_up)
  {
    cout << "answer : ";
    HB.write_target();
  }
  ENDL
  cout << "YOUR SCORE : " << HB.T << endl;
  if (with_cpu)
  {
    cout << " CPU SCORE : " << C.T << endl;
    if (HB.T < C.T) cout << "YOU WIN!(^w^)" << endl;
    else if (HB.T > C.T) cout << "YOU LOSE!(>w<)" << endl;
    else if (HB.T == C.T) cout << "DRAW!(.m.)" << endl;
  }

  return 0;
}