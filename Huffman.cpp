#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>

using namespace std;
#define INF 1000000000

template<class T>
void p(vector<T> A)
{
  for (auto a: A) cout << a << " ";
  cout << endl;
}

struct Huffman
{
  int len_bit, len_code;
  string text, code;
  double compression_rate;
  vector<char> appeared_alp;
  vector<int> count_alp, tree, parent, left, right;
  vector<bool> fixed;
  unordered_map<char, string> encodealp;
  unordered_map<string, char> decodebi;

  void get_len();
  void count_appear(const string&);
  void init_tree();
  void make_tree(const int);
  pair<int, int> find_nodes();
  void encode_alp(const int, const string&);
  string Encode(const string&);
  string Decode(const string&);
};

void Huffman::get_len()
{
  len_bit = (int)ceil(log2(appeared_alp.size()));
  len_bit *= text.size();
  len_code = code.size();
}

void Huffman::count_appear(const string& S)
{
  appeared_alp.clear();
  count_alp.clear();
  unordered_map<char, int> M;
  for (char c: S) ++M[c];
  for (pair<char, int> p: M)
  {
    appeared_alp.emplace_back(p.first);
    count_alp.emplace_back(p.second);
  }
}

void Huffman::init_tree()
{
  for (int i: count_alp)
  {
    tree.emplace_back(i);
    parent.push_back(-1);
    left.push_back(-1);
    right.push_back(-1);
    fixed.push_back(false);
  }
}

void Huffman::make_tree(const int depth)
{
  if (depth == 0) init_tree();
  pair<int, int> nodes;
  nodes = find_nodes();
  if (nodes.second == -1) return;
  int n1, n2;
  n1 = nodes.first;
  n2 = nodes.second;
  tree.emplace_back(tree[n1]+tree[n2]);
  left.emplace_back(n1);
  right.emplace_back(n2);
  parent.emplace_back(-1);
  fixed.emplace_back(false);
  parent[n1] = tree.size()-1;
  parent[n2] = tree.size()-1;
  fixed[n1] = true;
  fixed[n2] = true;
  make_tree(depth+1);
}

pair<int, int> Huffman::find_nodes()
{
  pair<int, int> nodes = {-1, -1};
  int min1 = INF, min2 = INF;
  for (int i = 0; i < tree.size(); ++i)
  {
    if (fixed[i]) continue;
    if (min1 > tree[i])
    {
      min2 = min1;
      nodes.second = nodes.first;
      min1 = tree[i];
      nodes.first = i;
    }
    else if (min2 > tree[i])
    {
      min2 = tree[i];
      nodes.second = i;
    }
  }
  return nodes;
}

void Huffman::encode_alp(const int node, const string& code)
{
  int l, r;
  string code_l, code_r;
  l = left[node];
  r = right[node];
  code_l = code + "0";
  code_r = code + "1";
  if (left[l] == -1 && right[l] == -1)
  {
    encodealp[appeared_alp[l]] = code_l;
    decodebi[code_l] = appeared_alp[l];
  }
  else encode_alp(l, code_l);
  if (left[r] == -1 && right[r] == -1)
  {
    encodealp[appeared_alp[r]] = code_r;
    decodebi[code_r] = appeared_alp[r];
  }
  else encode_alp(r, code_r);
  return;
}

string Huffman::Encode(const string& _text)
{
  text = _text;
  code = "";
  count_appear(text);
  make_tree(0);
  encode_alp(tree.size()-1, "");
  for (char c: text) code += encodealp[c];
  get_len();
  compression_rate = (double)len_code / (double)len_bit;
  return code;
}

string Huffman::Decode(const string& _code)
{
  string _text = "", key = "";
  for (char c: _code)
  {
    key += c;
    if (decodebi[key] != 0)
    {
      _text += decodebi[key];
      key = "";
    }
  }
  return _text;
}

int main()
{
  Huffman H;
  string text, code, decode;
  cout << "text : ";
  getline(cin, text);
  cout << "text : " << text << endl;
  code = H.Encode(text);
  decode = H.Decode(code);
  printf("Encode : %s\n", code.c_str());
  printf("Decode : %s\n", decode.c_str());
  printf("Commpression Rate : %lf\n", H.compression_rate);
  printf("Code Length : %d\n", H.len_code);
  vector<char> appeared_alp = H.appeared_alp;
  sort(appeared_alp.begin(), appeared_alp.end());
  for (char c: appeared_alp) printf("%c : %s\n", c, H.encodealp[c].c_str());

  return 0;
}
