#include <iostream>
#include <vector>
#include <random>

using namespace std;

template<class T>
struct Shuffle
{
 long long seed;
 mt19937 mt;
 vector<T> array;

 Shuffle(long long _seed=0);

 void push(const T&);
 T pop();
 int size();
 bool empty();
 void clear();
};

template<class T>
Shuffle<T>::Shuffle(long long _seed)
{
 array = {};
 if (_seed == 0)
 {
  random_device rnd;
  seed = rnd();
 }
 else seed = _seed;
 mt19937 tmp(seed);
 mt = tmp;
}

template<class T>
void Shuffle<T>::push(const T& a)
{
 array.emplace_back(a);
}

template<class T>
T Shuffle<T>::pop()
{
 if (array.empty())
 {
  cerr << __PRETTY_FUNCTION__<< " : Array is empty" << endl;
  exit(-1);
 }
 int idx;
 T ret;
 idx = mt() % array.size();
 ret = array[idx];
 array[idx] = array[array.size()-1];
 array.pop_back();
 return ret;
}

template<class T>
int Shuffle<T>::size()
{
 return array.size();
}

template<class T>
bool Shuffle<T>::empty()
{
 return (array.size() == 0);
}

template<class T>
void Shuffle<T>::clear()
{
 array.clear();
}

/*
int main()
{
 Shuffle<pair<int, int>> s;
 for (int i = 0; i < 10; ++i) s.push({i, i*i});
 while(!s.empty())
 {
  int a = s.pop().first;
  cout << a << endl;
 }
 s.clear();
 s.pop();

 return 0;
}
*/