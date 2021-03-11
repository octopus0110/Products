#include <iostream>
#include<vector>
using namespace std;

// 配列Aをマージソートする。O(nlogn)
template<class T>
void MargeSort(vector<T>& A, const int& left, const int& right, const bool& flg=true)
{
  /*
  A : ソートしたい配列
  left, right : [left, right-1] の範囲をソートする
  flg : true の場合は昇順, false の場合は降順にソートする
  */
  if (right - left == 1) return;

  int middle = (left+right+1) / 2;

  // ソート
  MargeSort(A, left, middle, flg);
  MargeSort(A, middle, right, flg);

  // 分割した配列を一時的に保持
  vector<int> buf_l, buf_r;
  for (int i = left; i < middle; ++i) buf_l.emplace_back(A[i]);
  for (int i = middle; i < right; ++i) buf_r.emplace_back(A[i]);

  // マージ
  int i_l = 0, i_r = 0;
  for (int i = left; i < right; ++i)
  {
    // flg==true なら昇順
    if (flg)
    {
      if (i_l > middle-left-1) A[i] = buf_r[i_r++];
      else if (i_r > right-middle-1) A[i] = buf_l[i_l++];
      else
      {
        if (buf_l[i_l] <= buf_r[i_r]) A[i] = buf_l[i_l++];
        else if (buf_l[i_l] > buf_r[i_r]) A[i] = buf_r[i_r++];
      }
    }
    // flg==false なら降順
    else
    {
      if (i_l > middle-left-1) A[i] = buf_r[i_r++];
      else if (i_r > right-middle-1) A[i] = buf_l[i_l++];
      else
      {
        if (buf_l[i_l] >= buf_r[i_r]) A[i] = buf_l[i_l++];
        else if (buf_l[i_l] < buf_r[i_r]) A[i] = buf_r[i_r++];
      }
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
  MargeSort(A, 0, n, flg);
  for (int i = 0; i < n; i++) cout << A[i] << endl;

  return 0;
}