#include <iostream>
#include <vector>
#include <random>

using namespace std;

template<class T>
struct BiHeap
{
  vector<T> tree;

  void swap(const int&, const int&);
  int get_par(const int&);
  int get_chl(const int&);
  void upheap(const int);
  void downheap(const int);

  void push(const T);
  T pop();
  bool empty();
  int size();
};

template<class T>
void BiHeap<T>::swap(const int& n1, const int& n2)
{
  T tmp;
  tmp = tree[n1];
  tree[n1] = tree[n2];
  tree[n2] = tmp;
}

template<class T>
int BiHeap<T>::get_par(const int& n)
{
  return ((n + 1) / 2 - 1);
}

template<class T>
int BiHeap<T>::get_chl(const int& n)
{
  return ((n + 1) * 2 - 1);
}

template<class T>
void BiHeap<T>::upheap(const int n)
{
  if (n == 0) return;
  int par = get_par(n);
  if (tree[n] < tree[par])
  {
    swap(n, par);
    upheap(par);
  }
  else return;
}

template<class T>
void BiHeap<T>::downheap(const int n)
{
  int chl = get_chl(n);
  if (chl >= tree.size()) return;
  T left, right;
  left = tree[chl];
  if (chl + 1 < tree.size()) right = tree[chl + 1];
  else if (tree[n] > left)
  {
    swap(n, chl);
    return downheap(chl);
  }
  else return;
  if (right <= left)
  {
    if (tree[n] > right)
    {
      swap(n, chl + 1);
      return downheap(chl + 1);
    }
    else return;
  }
  else if (left < right)
  {
    if (tree[n] > left)
    {
      swap(n, chl);
      return downheap(chl);
    }
    else return;
  }
}

template<class T>
void BiHeap<T>::push(const T x)
{
  tree.emplace_back(x);
  upheap(tree.size()-1);
}

template<class T>
T BiHeap<T>::pop()
{
  if (tree.empty())
  {
    cerr << __PRETTY_FUNCTION__ << " : tree is empty" << endl;
    exit(-1);
  }
  T ret;
  ret = tree[0];
  tree[0] = tree[tree.size() - 1];
  tree.pop_back();
  downheap(0);
  return ret;
}

template<class T>
bool BiHeap<T>::empty()
{
  return tree.empty();
}

template<class T>
int BiHeap<T>::size()
{
  return tree.size();
}

int main()
{
  BiHeap<int> B;
  random_device rnd;
  mt19937 mt(rnd());
  for (int i = 0; i < 100; ++i)
  {
    int x = mt() % 107 * (mt() % 2 ? 1 : -1);
    if (B.empty())
    {
      B.push(x);
      for (int b: B.tree) cout << b << " ";
      cout << endl;
    }
    else
    {
      if (B.size() < 3 || mt() % 2)
      {
        B.push(x);
        for (int b: B.tree) cout << b << " ";
        cout << endl;
      }
      else cout << "pop : " << B.pop() << endl;
    }
  }
  while(!B.empty())
  {
    cout << "pop : " << B.pop() << endl;
  }

  return 0;
}
