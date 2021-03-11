//ある量のお肉を買いたい時、どの組み合わせで買えば一番無駄なく買えるかを計算するプログラム

#include <iostream>
#include <vector>
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)
#define REP2(i, m, n) for (int i = (m); i < (int)(n); ++i)
#define REPR(i, n) for (int i = (n)-1; i >= 0; --i)
using namespace std;
using VI = vector<int>;
using VVI = vector<VI>;
using VB = vector<bool>;
const int INF = 1e9;

int main()
{
  int n, gram_sum, price;
  // n        : 品物の個数
  // gram_sum : 必要な量
  // price    : 100グラムあたりの価格
  cout << "品物の個数 : ";
  cin >> n;
  cout << "合計グラム数 : ";
  cin >> gram_sum;
  cout << "100グラムあたりの価格 : ";
  cin >> price;

  VI gram(n);
  // gram[i] : i番目の品物の重量
  REP(i, n)
  {
    cout << "goods" << i+1 << " : ";
    cin >> gram[i];
  }

  VVI dp(n, VI(gram_sum+1, INF)), prev_g(n, VI(gram_sum+1, gram_sum));
  // dp[i][g]     : i番目までの品物の組み合わせの中で、gグラム以上になる総重量の最小値
  // prev_g[i][g] : dp[i][g]が更新されたとき、その前の総重量の値(実重量ではなくインデックスのことだが便宜上負の値を持つこともある)を格納する
  //                つまり、i番目の品物を選んだとしてdpを更新した場合はg-gram[i]、選ばない場合はgとなる (dp[i][g] が dp[i-1][prev_g[i][g]]によって更新されたことを示す)

  // dpテーブルの初期化
  REP(g, min(gram[0]+1, gram_sum+1))
  {
    dp[0][g] = g ? gram[0] : 0;
    prev_g[0][g] = g - gram[0];
  }

  // dp計算
  REP2(i, 1, n) REP(g, gram_sum+1)
  {
    if (g - gram[i] >= 0)
    {
      if (dp[i][g] > dp[i-1][g-gram[i]] + gram[i])
      {
        dp[i][g] = dp[i-1][g-gram[i]] + gram[i];
        prev_g[i][g] = g - gram[i];
      }
    }
    else if (dp[i][g] > gram[i])
    {
      dp[i][g] = gram[i];
      prev_g[i][g] = g - gram[i];
    }
    if (dp[i][g] > dp[i-1][g])
    {
      dp[i][g] = dp[i-1][g];
      prev_g[i][g] = g;
    }
  }

  // 復元
  VB res1(n, false);    // 組み合わせを格納する
  int cur_g = gram_sum; // 今見ている総重量値
  REPR(i, n)
  {
    if (prev_g[i][cur_g] == cur_g - gram[i])
    {
      res1[i] = true;
      cur_g -= gram[i];
    }
    if (cur_g <= 0) break;
  }

  VVI dp2(n, VI(gram_sum+1, 0)), prev_g2(n, VI(gram_sum+1, gram_sum));
  // dp2[i][g]      : i番目までの品物の組み合わせの中で、gグラムを超えないような総重量の最大値
  // prev_g2[i][g]  : prev_gと同様

  // dpテーブルの初期化
  REP2(g, gram[0], gram_sum+1)
  {
    dp2[0][g] = gram[0];
    prev_g2[0][g] = g-gram[0];
  }

  // dp計算
  REP2(i, 1, n) REP(g, gram_sum+1)
  {
    if (g - gram[i] >= 0)
    {
      if (dp2[i][g] < dp2[i-1][g-gram[i]] + gram[i])
      {
        dp2[i][g] = dp2[i-1][g-gram[i]] + gram[i];
        prev_g2[i][g] = g - gram[i];
      }
    }
    if (dp2[i][g] < dp2[i-1][g])
    {
      dp2[i][g] = dp2[i-1][g];
      prev_g2[i][g] = g;
    }
  }

  // 復元
  VB res2(n, false);  // 組み合わせを格納する
  cur_g = gram_sum;   // 今見ている総重量値
  REPR(i, n)
  {
    if (prev_g2[i][cur_g] == cur_g - gram[i])
    {
      res2[i] = true;
      cur_g -= gram[i];
    }
    if (cur_g <= 0) break;
  }

  // 出力
  cout << "==================================================" << endl;
  cout << gram_sum << "グラム以上で最も最適な組み合わせは";
  if (dp[n-1][gram_sum] == INF) cout << "存在しません" << endl;
  else
  {
    cout << "\n\n";
    REP(i, n) if (res1[i]) cout << gram[i] << " ";
    cout << "\n\n合計グラム数は " << dp[n-1][gram_sum] << "グラム" << endl;
    cout << "合計金額は " << price*dp[n-1][gram_sum]/100 << "円" << " 差額は +" << (price*dp[n-1][gram_sum]-price*gram_sum)/100 << "円" << endl;
  }

  cout << "==================================================" << endl;
  cout << gram_sum << "グラム以下で最も最適な組み合わせは";
  if (dp2[n-1][gram_sum] == 0) cout << "存在しません" << endl;
  else
  {
    cout << "\n\n";
    REP(i, n) if (res2[i]) cout << gram[i] << " ";
    cout << "\n\n合計グラム数は " << dp2[n-1][gram_sum] << "グラム" << endl;
    cout << "合計金額は " << price*dp2[n-1][gram_sum]/100 << "円" << " 差額は -" << abs((price*dp2[n-1][gram_sum]-price*gram_sum)/100) << "円" << endl;
  }
  cout << "==================================================" << endl;

  /*
  dpテーブルとprev_gテーブルの中身を確認するデバッグ用コード

  cout << "\n============= dp =============" << endl;
  REP(i, n)
  {
    REP(j, gram_sum+1)
    {
      if (dp[i][j] != INF) printf("%2d ", dp[i][j]);
      else cout << "un" << " ";
    }
    cout << endl;
  }
  cout << "============= prev_g =============" << endl;
  REP(i, n)
  {
    REP(j, gram_sum+1)
    {
      if (prev_g[i][j] != gram_sum) printf("%2d ", prev_g[i][j]);
      else cout << "un" << " ";
    }
    cout << endl;
  }
  */

  return 0;
}