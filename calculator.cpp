#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)
#define REP2(i, m, n) for (int i = (m); i < (int)(n); ++i)
#define REPR(i, n) for (int i = (n)-1; i >= 0; --i)
#define REPR2(i, m, n) for (int i = (n)-1; i >= (m); --i)
#define REPx(x, a) for(auto x : a)
#define ALL(a) a.begin(), a.end()
#define REVERSE(a) reverse(ALL(a))
#define ENDL cout << endl;
using namespace std;
using ULL = unsigned long long;
using VI = vector<int>;
using VS = vector<string>;


// 文字列を利用した整数の四則演算、累乗、階乗、素数判定、素因数分解

string hm(string a, string b);
string hp(string a, string b);
clock_t st, en;

// 2数の引き算
string hm(string a, string b)
{
 bool fa = false, fb = false;
 if (a[0] == '-')
 {
  fa = true;
  a.erase(a.begin());
 }
 if (b[0] == '-')
 {
  fb = true;
  b.erase(b.begin());
 }
 if (!fa && fb) return hp(a, b);
 else if (fa && !fb)
 {
  string res = hp(a, b);
  return "-" + res;
 }
 else if (fa && fb) swap(a, b);
 short int len_a = a.size(), len_b = b.size();
 bool flg = false;
 if (len_a < len_b)
 {
  flg = true;
  swap(a, b);
  swap(len_a, len_b);
 }
 else if (len_a == len_b)
 {
  REP(i, len_a)
  {
   if (a[i] < b[i])
   {
    flg = true;
    swap(a, b);
    swap(len_a, len_b);
    break;
   }
   else if (a[i] > b[i]) break;
  }
 }
 string res = "";
 REP(i, len_a)
 {
  char ai = a[len_a-i-1];
  if (i < len_b)
  {
   char bi = b[len_b-i-1];
   if (ai >= bi) res = to_string(ai-bi) + res;
   else
   {
    res = to_string(10-(bi-'0')+(ai-'0')) + res;
    bool con = true;
    short int idx = len_a-i-2;
    while(con && idx >= 0)
    {
     if(a[idx] != '0')
     {
      a[idx] = a[idx] - 1;
      con = false;
     }
     else a[idx] = '9';
     idx--;
    }
   }
  }
  else res = ai + res;
 }
 REVERSE(res);
 REPR2(i, 1, res.size())
 {
  if (res[i]-'0') break;
  else res.pop_back();
 }
 REVERSE(res);
 if (flg) res = "-" + res;
 return res;
}

// 2数の足し算
string hp(string a, string b)
{
 bool fa = false, fb = false, flg = false;
 if (a[0] == '-')
 {
  fa = true;
  a.erase(a.begin());
 }
 if (b[0] == '-')
 {
  fb = true;
  b.erase(b.begin());
 }
 if (!fa && fb) return hm(a, b);
 else if (fa && !fb)
 {
  swap(a, b);
  return hm(a, b);
 }
 else if (fa && fb) flg = true;
 short int len_a = a.size(), len_b = b.size();
 string res = "";
 short int rr = 0;
 REP(i, max(len_a, len_b))
 {
  short int aa = 0, bb = 0, ss;
  char ai = a[len_a-i-1], bi = b[len_b-i-1];
  if (i < len_a) aa = ai - '0';
  if (i < len_b) bb = bi - '0';
  ss = aa + bb + rr;
  string ad = to_string(ss % 10);
  rr = (ss - ss % 10) / 10;
  res = ad + res;
 }
 if (rr) res = to_string(rr) + res;
 if (flg) res = '-' + res;
 return res;
}

// aに10^digを掛けた数を計算する
string zero;
string tc(int dig, string a)
{
 if (a == "0") return "0";
 a += zero.substr(0, dig);
 return a;
}

// 一桁の数(a)と複数桁の数(b)との掛け算
string mc(char a, string b)
{
 if (a == '0' || b == "0") return "0";
 string res = "0";
 REP(i, b.size()) res = hp(res, tc(i, to_string((a-'0')*(b[b.size()-i-1]-'0'))));
 return res;
}

// 2数の掛け算
string hc(string a, string b)
{
 if (a == "0" || b == "0") return "0";
 bool fa = false, fb = false;
 if (a[0] == '-')
 {
  fa = true;
  a.erase(a.begin());
 }
 if (b[0] == '-')
 {
  fb = true;
  b.erase(b.begin());
 }
 string res = "0";
 REP(i, a.size()) res = hp(res, tc(i, mc(a[a.size()-i-1], b)));
 if ((fa && !fb) || (!fa && fb)) res = "-" + res;
 return res;
}

// m^eを計算する
string ex(string m, string e)
{
 bool flg = false;
 if(m[0] == '-')
 {
  m.erase(m.begin());
  flg = true;
 }
 if (e == "0") return "1";
 string a = "1";
 if (stoi(e) % 2) a = m;
 string b = ex(m, to_string(stoi(e)/2));
 string res = hc(a, hc(b, b));
 if (flg) res = "-" + res;
 return res;
}

// 2数の割り算(商と余りを計算する)
string hd(string& a, string b)
{
  if (b == "0")
  {
    cout << endl << "Calculation error" << endl;
    exit(0);
  }
  string res = "", tmp;
  int roop = a.size();
  REPR(i, roop)
  {
    REPR(j, 10)
    {
      tmp = hm(a, hc(b, hc(to_string(j), ex("10", to_string(i)))));
      if (tmp[0] != '-')
      {
        if (res != "") res += to_string(j);
        else if (j != 0) res += to_string(j);
        a = tmp;
        break;
      }
    }
  }
  if (res == "") res = "0";
  return res;
}

// 素数判定。整数型で扱える場合はこちらで計算する。
bool primarity_testi(ULL a, bool output=false)
{
  if (a < 2) return false;
  else if (a == 2) return true;
  else if (a % 2 == 0)
  {
   if (output) cout << "2 を素因数にもちます" << endl;
   return false;
  }
  for (ULL i = 3; i*i <= a; i += 2)
  {
   if (a % i == 0)
   {
    if (output) cout << i << " を素因数にもちます" << endl;
    return false;
   }
  }
  return true;
}

// 素数判定。64bitを超えても時間が無限にあるならこちらで計算できる。とんでもなく遅い上に、1e8までの素因数しか使えないので精度も低い。
short int primarity_test(string n, bool output=false)
{
  const clock_t TIME_LIMIT = 60;
  string rem = n;
  hd(rem, "2");
  if (hm(n, "2")[0] == '-') return 0;
  else if (n == "2") return 1;
  else if (rem == "0")
  {
   if (output) cout << "2 を素因数にもちます" << endl;
   return 0;
  }

  st = clock();
  for(int i = 3;i < 1e8; i += 2)
  {
    en = clock();
    if ((double)(en-st)/CLOCKS_PER_SEC >= TIME_LIMIT)
    {
      if (output) cout << "計算時間が " << TIME_LIMIT << "s を超えました.少なくとも " << i-2 << " までの素因数は存在しません." << endl;
      return -1;
    }
    if (!primarity_testi(i)) continue;
    string p = to_string(i);
    if (hm(n, p)[0] == '-') return 1;
    rem = n;
    hd(rem, p);
    if (rem == "0")
    {
      if (output) cout << p << " を素因数にもちます" << endl;
      return 0;
    }
  }
  return 2;
}

// 素因数分解
void prime_factorize(ULL n, vector<pair<ULL, ULL>> &res)
{
  for (ULL s = 2; n >= s * s; s += 2)
  {
    ULL e = 0;
    if (n % s == 0)
    {
      while(n % s == 0)
      {
        n /= s;
        e++;
      }
      res.emplace_back(s, e);
    }
    if (s == 2) s--;
  }
  if (n != 1) res.emplace_back(n, 1);
}

// 階乗
string factorial(string n)
{
 bool flg = false;
 if (n[0] == '-')
 {
  flg = true;
  n.erase(n.begin());
 }
 string res = "1";
 REP2(i, 2, stoi(n)+1) res = hc(res, to_string(i));
 if (flg) res = "-" + res;
 return res;
}

// 階乗の素因数分解
vector<pair<ULL, ULL>> A;
void pff(ULL N)
{
  for(ULL i = 1; i < N+1; i++)
  {
    vector<pair<ULL, ULL>> result;
    prime_factorize(i, result);
    REP(i, result.size())
    {
      bool flg = false;
      REP(j, A.size())
      {
        if (result[i].first == A[j].first)
        {
          A[j].second += result[i].second;
          flg = true;
          break;
        }
      }
      if (!flg) A.emplace_back(result[i]);
    }
  }
}


////////////////////////////////////////////////////////////////////////////


// main
int main()
{
 ifstream fin("zero.txt");
 if (!fin)
 {
  cout << "'zero.txt' does not exist" << endl;
  return -1;
 }
 getline(fin, zero);

 string s;
 short int flg = 0;
 cin >> s;

 // 数字の末尾がpなら素数判定をする
 if (s.back() == 'p')
 {
  ENDL
  s.pop_back();
  if (hm("18446744073709551615", s)[0] == '-')
  {
   cout << "ullを超えました" << endl;
   flg = primarity_test(s, true);
  }
  else flg = primarity_testi(stoull(s), true);
  if (flg == 1) cout << "素数です" << endl;
  else if (flg == 0) cout << "素数ではありません" << endl;
  else if (flg == 2) cout << "少なくとも1e8までの素因数は存在しません" << endl;
  else cout << "TLE" << endl;
  return 0;
 }
 // 数字の末尾がfなら素因数分解をする
 else if(s.back() == 'f')
 {
  ENDL
  s.pop_back();
  if(s.back() == '!') // 階乗の素因数分解
  {
   s.pop_back();
   pff(stoull(s));
   cout << "= ";
   REP(i, (int)A.size())
   {
    cout << A[i].first;
    if (A[i].second != 1) cout << "^" << A[i].second;
    if (i < (int)A.size()-1) cout << "*";
    if (i == (int)A.size() - 1) ENDL
   }
   return 0;
  }
  if (hm("18446744073709551615", s)[0] == '-')
  {
   cout << "ullを超えました" << endl;
   return 0;
  }
  vector<pair<ULL, ULL>> res;
  prime_factorize(stoull(s), res);
  if (res.size() == 1 && res[0].second == 1) cout << "素数です" << endl;
  else
  {
    REP(i, (int)res.size())
    {
    if (i == 0) cout << "= ";
    cout << res[i].first;
    if (res[i].second != 1) cout << "^" << res[i].second;
    if(i < (int)res.size()-1) cout << "*";
    if (i == (int)res.size() - 1) ENDL
    }
  }
  return 0;
 }

 string num = "", num2;
 VI symn(100, -1);
 VS syms(100, "0");
 int idx = 0, ct = 0;
 bool exp = false;
 REPx(x, s)
 {
  if ((x == '*' || x == '+' || x == '-' || x == '/') && exp)
  {
   num = ex(num2, num);
   exp = false;
  }
  else if (x == '^' && exp)
  {
    cout << "^ は続けて入力できません" << endl;
    return 0;
  }
  else if (x == '!')
  {
   num = factorial(num);
   continue;
  }
  if (x != '*' && x != '+' && x != '-' && x != '^' && x != '/') num += x;
  else if (x == '*')
  {
   syms[idx] = num;
   symn[ct] = idx;
   num = "";
   idx++;
   ct++;
  }
  else if (x == '+')
  {
    syms[idx] = num;
    num = "";
    idx++;
  }
  else if (x == '-')
  {
    if (num == "") num += "-";
    else
    {
     syms[idx] = num;
     num = "-";
     idx++;
    }
  }
  else if (x == '^')
  {
   num2 = num;
   num = "";
   exp = true;
  }
  else if (x == '/')
  {
    syms[idx] = num;
    num = "";
    idx++;
    flg = true;
  }
 }
 if (exp) syms[idx] = ex(num2, num);
 else syms[idx] = num;

if (flg) // 割り算
 {
  if (hm("18446744073709551615", syms[0])[0] == '-')
  {
   cout << "ullを超えました" << endl;
   string quo = hd(syms[0], syms[1]);
   cout << endl << "= " << quo << endl;
   if (syms[0] != "0") cout << "... " << syms[0] << endl;
  }
  else
  {
   if (syms[1] == "0")
   {
    cout << endl << "Calculation Error" << endl;
    return 0;
   }
   cout << endl << "= " << stoull(syms[0]) / stoull(syms[1]) << endl;
   ULL rem = stoull(syms[0]) % stoull(syms[1]);
   if (rem) cout << "... " << rem << endl;
  }
  return 0;
 }

 REPx(x, symn)
 {
  if (x == -1) break;
  syms[x+1] = hc(syms[x], syms[x+1]);
  syms[x] = "0";
 }

 string res = "0";
 REPx(x, syms) res = hp(res, x);

 cout << endl << "= " << res << endl;

 return 0;
}