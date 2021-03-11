#include <iostream>
#include<vector>
using namespace std;

// 配列Aをバブルソートする。O(n^2)
void BubbleSort(vector<int>& A, const bool& flg=true)
{
  /*
  A : ソートしたい配列
  flg : true の場合は昇順, false の場合は降順にソートする
  */
  for (int tail = A.size(); tail > 0; --tail)
  {
    for (int head = 1; head < tail; ++head)
    {
      if (flg && A[head-1] > A[head]) swap(A[head], A[head-1]);
      else if (!flg && A[head-1] < A[head]) swap(A[head], A[head-1]);
    }
  }
}

int main()
{
  int flg;
  int n;
  cin >> n >> flg;
  vector<int> A(n);
  for (int i = 0; i < n; ++i) cin >> A[i];
  BubbleSort(A, flg);
  for (int i = 0; i < n; i++) cout << A[i] << endl;

  return 0;
}