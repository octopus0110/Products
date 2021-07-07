#include "Time.cpp"
#include "Shuffle.cpp"

#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <queue>
#include <stack>
#include <chrono>
#include <thread>

using namespace std;

#define DEBUG false // trueにすると呼ばれた関数をコンソールに出力
#define ZENKAKU true	// trueにすると全角文字で表示

/* 迷路生成アルゴリズムを選択 */
//#define TYPE "botaoshi" // 棒倒し法
//#define TYPE "anahori" // 穴掘り法
//#define TYPE "kabenobashi_1" // 壁伸ばし法(壁から)
//#define TYPE "kabenobashi_2" // 壁伸ばし法(通路から)

/* 迷路探索アルゴリズムを選択 */
//#define SEARCH "right" // 右手法
//#define SEARCH "sight" // 直線の先に何があるか見え経路記憶しながら進む
//#define SEARCH "fool" // 直線の先に何があるか見えるが記憶力が悪い
//#define SEARCH "bfs" // 幅優先探索
//#define SEARCH "dfs" // 深さ優先探索
//#define SEARCH "astar" // Ａ*アルゴリズム

/* Ａ*アルゴリズムにおけるヒューリスティック関数を選択 */
//#define HEURISTIC "MANHATTAN" // マンハッタン距離
//#define HEURISTIC "EUCLIDEAN" // ユークリッド距離

/* foolの賢さ(INT)を設定 */
//#define INT 1 // INT回突き当たった道は通らなくなる

/* スタートとゴールの位置を設定 */
//#define CORNER false // trueにすると左上端にスタート、右下端にゴールを設定
//#define FAR true // trueにすると最長経路の端にスタート/ゴールを設定
/*
CORNERとFARのいずれともfalseの場合ランダムに配置されるが
その時のスタートとゴールのユークリッド距離(D)を設定
*/
#define RATE_DIS 0.8 // D >= max(H, W)*RATE_DIS になるように設定
#define MAX 30000 // スタート/ゴールの再配置回数の上限

/* 壁, 通路を表現する記号を設定 */
// HANKAKU
#define Path " "
#define Wall "\033[47m#\033[0m"
#define Answer "\033[41m*\033[0m"
#define Start "\033[43mS\033[0m"
#define Goal "\033[42mG\033[0m"
// ZENKAKU
#define PATH "　"
#define WALL "\033[47m■\033[0m"
#define ANSWER "\033[31m■\033[0m"
#define START "\033[43mＳ\033[0m"
#define GOAL "\033[42mＧ\033[0m"

struct Maze
{
	int H, W,	sx, sy, gx, gy, distance, order, search_num;
	queue<int> whens;
	vector<vector<int>> maze, maze_init, count_never;
	vector<vector<bool>> simple;

	Maze(const int&, const int&);

	bool isNumber(const string&);
	template<class T> bool iszero(const vector<vector<T>>&);
	template<class T> bool isin(const vector<T>&, const T&);

	void generate_maze(const string&, long long seed=0);
	void reset_maze_botaoshi();
	void generate_maze_botaoshi(const long long&);
	void reset_maze_anahori();
	void generate_maze_anahori(const long long&);
	void reset_maze_kabenobashi_1(const long long&, Shuffle<pair<int, int>>&);
	void reset_maze_kabenobashi_2(const long long&, Shuffle<pair<int, int>>&);
	void generate_maze_kabenobashi_1(const long long&);
	void generate_maze_kabenobashi_2(const long long&);
	vector<int> is_able_anahori(const int&, const int&);
	vector<int> is_able_kabenobashi_1(const int&, const int&);
	vector<int> is_able_kabenobashi_2(const int&, const int&, const vector<pair<int, int>>&);
	bool check_range(const pair<int, int>&);
	void turn(int&, const pair<int, int>&);
	pair<int, int> bfs(const pair<int, int>&);
	void set_startgoal(long long seed=0, string INIT="", pair<int, int> start={-1,-1}, pair<int, int> goal={-1,-1});
	void simplify_maze();
	void simplify_maze_visualize();
	bool solve_maze(const string&, int INT=1);
	bool solve_maze_right();
	bool solve_maze_sight();
	bool solve_maze_fool(const int&);
	bool solve_maze_bfs();
	bool solve_maze_dfs();
	bool solve_maze_astar(string HEU="Manhattan");
	double heuristic(const pair<int, int>&, const string&);
	double Manhattan(const pair<int, int>&);
	double Euclidean(const pair<int, int>&);
	bool sight_goal(int&, const pair<int, int>&, const int&);
	void print_maze();
};

Maze::Maze(const int& h, const int& w):H(h), W(w)
{
	sx = 1;
	sy = 1;
	gx = 1;
	gy = 1;
	distance = -1;
	order = 0;
}

bool Maze::isNumber(const string& s)
{
	for (char c: s)
	{
		if (c-'0' < 0 || 9 < c-'0') return false;
	}
	return true;
}

template<class T> bool iszero(const vector<vector<T>>& A)
{
	int sum = 0;
	for (auto a: A) for (auto x: a) sum += x;
	return sum == 0;
}

template<class T> bool Maze::isin(const vector<T>& A, const T& x)
{
	for (pair<int, int> a: A) if (a == x) return true;
	return false;
}

void Maze::generate_maze(const string& type, long long seed)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	random_device rnd;
	if (seed == 0) seed = rnd();
	if (type == "botaoshi") generate_maze_botaoshi(seed);
	else if (type == "anahori") generate_maze_anahori(seed);
	else if (type == "kabenobashi_1") generate_maze_kabenobashi_1(seed);
	else if (type == "kabenobashi_2") generate_maze_kabenobashi_2(seed);
	else
	{
		cerr << __PRETTY_FUNCTION__ << " : Type is wrong" << endl;
		exit(-1);
	}
	sx = 1;
	sy = 1;
	gx = maze[0].size() - 2;
	gy = maze.size() - 2;
	maze_init = maze;
}

void Maze::reset_maze_botaoshi()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : Maze size is small" << endl;
		exit(-1);
	}
	if (H % 2 == 0) ++H;
	if (W % 2 == 0) ++W;
	vector<vector<int>> tmp(H+2, vector<int>(W+2, false));
	for (int y = 0; y < H+2; ++y)
	{
		for (int x = 0; x < W+2; ++x)
		{
			if (y == 0 || y == H+1 || x == 0 || x == W+1) tmp[y][x] = true;
			else if (x % 2 == 0 && y % 2 == 0) tmp[y][x] = true;
		}
	}
	maze = tmp;
	maze_init = tmp;
}

void Maze::generate_maze_botaoshi(const long long& seed)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : MazeSize is small" << endl;
		exit(-1);
	}
	reset_maze_botaoshi();
	mt19937 mt(seed);
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	for (int y = 2; y < H; y+=2)
	{
		for (int x = 2; x < W; x+=2)
		{
			while (true)
			{
				int xx, yy, idx, dirx, diry;
				idx = mt() % 4;
				dirx = stepx[idx];
				diry = stepy[idx];
				if (y > 2 && diry < 0) continue;
				xx = x + dirx;
				yy = y + diry;
				if (maze[yy][xx]) continue;
				maze[yy][xx] = true;
				break;
			}
		}
	}
}

void Maze::reset_maze_anahori()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : Maze size is small" << endl;
		exit(-1);
	}
	if (H % 2 == 0) ++H;
	if (W % 2 == 0) ++W;
	vector<vector<int>> tmp(H+2, vector<int>(W+2, true));
	maze = tmp;
	maze_init = tmp;
}

void Maze::generate_maze_anahori(const long long& seed)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : MazeSize is small" << endl;
		exit(-1);
	}
	reset_maze_anahori();
	Shuffle<pair<int, int>> rest(seed);
	mt19937 mt(seed);
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	int x, y;
	while (true)
	{
		x = mt() % (W+2);
		y = mt() % (H+2);
		if (x == 0 || x == W+1) continue;
		if (y == 0 || y == H+1) continue;
		if (x % 2 == 0 || y % 2 == 0) continue;
		break;
	}
	maze[y][x] = false;
	rest.push({x, y});
	while(!rest.empty())
	{
		pair<int, int> pos, next;
		int idx;
		idx = mt() % rest.size();
		pos = rest.pop();
		x = pos.first;
		y = pos.second;
		while (true)
		{
			int dir, nx, ny;
			vector<int> can;
			can = is_able_anahori(x, y);
			if (can.empty()) break;
			else if (can.size() > 1) rest.push({x, y});
			dir = can[mt() % can.size()];
			nx = x + 2*stepx[dir];
			ny = y + 2*stepy[dir];
			next = {nx, ny};
			maze[ny][nx] = false;
			maze[ny - stepy[dir]][nx - stepx[dir]] = false;
			x = nx;
			y = ny;
			if (!is_able_anahori(nx, ny).empty()) rest.push(next);
		}
	}
}

void Maze::reset_maze_kabenobashi_1(const long long& seed, Shuffle<pair<int, int>>& rest)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : Maze size is small" << endl;
		exit(-1);
	}
	if (H % 2 == 0) ++H;
	if (W % 2 == 0) ++W;
	vector<vector<int>> tmp(H+2, vector<int>(W+2, false));
	for (int x = 0; x < W+2; ++x)
	{
		tmp[0][x] = true;
		tmp[H+1][x] = true;
		if (x % 2 == 0)
		{
			rest.push({x, 0});
			rest.push({x, H+1});
		}
	}
	for (int y = 1; y < H+1; ++y)
	{
		tmp[y][0] = true;
		tmp[y][W+1] = true;
		if (y % 2 == 0)
		{
			rest.push({0, y});
			rest.push({W+1, y});
		}
	}
	maze = tmp;
	maze_init = tmp;
}

void Maze::reset_maze_kabenobashi_2(const long long& seed, Shuffle<pair<int, int>>& rest)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : Maze size is small" << endl;
		exit(-1);
	}
	if (H % 2 == 0) ++H;
	if (W % 2 == 0) ++W;
	vector<vector<int>> tmp(H+2, vector<int>(W+2, false));
	for (int y = 0; y < H+2; ++y)
	{
		for (int x = 0; x < W+2; ++x)
		{
			if (y == 0 || y == H+1) tmp[y][x] = true;
			else if (x == 0 || x == W+1) tmp[y][x] = true;
			else if (x % 2 == 0 && y % 2 == 0) rest.push({x, y});
		}
	}
	maze = tmp;
	maze_init = tmp;
}

void Maze::generate_maze_kabenobashi_1(const long long& seed)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : MazeSize is small" << endl;
		exit(-1);
	}
	Shuffle<pair<int, int>> rest(seed);
	reset_maze_kabenobashi_1(seed, rest);
	mt19937 mt(seed);
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	while(!rest.empty())
	{
		pair<int, int> pos, next;
		int x, y, idx;
		idx = mt() % rest.size();
		pos = rest.pop();
		x = pos.first;
		y = pos.second;
		while (true)
		{
			int dir, nx, ny;
			vector<int> can;
			can = is_able_kabenobashi_1(x, y);
			if (can.empty()) break;
			else if (can.size() > 1) rest.push({x, y});
			dir = can[mt() % can.size()];
			nx = x + 2*stepx[dir];
			ny = y + 2*stepy[dir];
			next = {nx, ny};
			maze[ny][nx] = true;
			maze[ny - stepy[dir]][nx - stepx[dir]] = true;
			x = nx;
			y = ny;
			if (!is_able_kabenobashi_1(nx, ny).empty()) rest.push(next);
		}
	}
}

void Maze::generate_maze_kabenobashi_2(const long long& seed)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : MazeSize is small" << endl;
		exit(-1);
	}
	Shuffle<pair<int, int>> rest(seed);
	reset_maze_kabenobashi_2(seed, rest);
	mt19937 mt(seed);
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	while(!rest.empty())
	{
		vector<pair<int, int>> self = {};
		pair<int, int> pos;
		int x, y, idx, x_ini, y_ini;
		bool flag = false;
		idx = mt() % rest.size();
		pos = rest.pop();
		x = pos.first;
		y = pos.second;
		if (maze[y][x]) continue;
		self.push_back({x, y});
		x_ini = x;
		y_ini = y;
		while (true)
		{
			int dir, nx, ny;
			vector<int> can;
			can = is_able_kabenobashi_2(x, y, self);
			if (can.empty())
			{
				vector<pair<int, int>> emp = {};
				if (is_able_kabenobashi_2(x_ini, y_ini, emp).size() > 1) rest.push({x_ini, y_ini});
				break;
			}
			dir = can[mt() % can.size()];
			nx = x + 2*stepx[dir];
			ny = y + 2*stepy[dir];
			self.push_back({nx - stepx[dir], ny - stepy[dir]});
			if (maze[ny][nx])
			{
				flag = true;
				break;
			}
			self.push_back({nx, ny});
			x = nx;
			y = ny;
		}
		if (flag)
		{
			for (pair<int, int> p: self)
			{
				int xx, yy;
				xx = p.first;
				yy = p.second;
				maze[yy][xx] = true;
			}
		}
	}
}

vector<int> Maze::is_able_anahori(const int& x, const int& y)
{
	vector<int> ret = {}, stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	for (int i = 0; i < 4; ++i)
	{
		int xx, yy;
		xx = x + 2 * stepx[i];
		yy = y + 2 * stepy[i];
		if (!check_range({xx, yy})) continue;
		if (xx == 0 || xx == W+1) continue;
		if (yy == 0 || yy == H+1) continue;
		if (maze[yy][xx]) ret.emplace_back(i);
	}
	return ret;
}

vector<int> Maze::is_able_kabenobashi_1(const int& x, const int& y)
{
	vector<int> ret = {}, stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	for (int i = 0; i < 4; ++i)
	{
		int xx, yy;
		xx = x + 2 * stepx[i];
		yy = y + 2 * stepy[i];
		if (!check_range({xx, yy})) continue;
		if (!maze[yy][xx]) ret.emplace_back(i);
	}
	return ret;
}

vector<int> Maze::is_able_kabenobashi_2(const int& x, const int& y, const vector<pair<int, int>>& self)
{
	vector<int> ret = {}, stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	for (int i = 0; i < 4; ++i)
	{
		int xx, yy;
		xx = x + 2 * stepx[i];
		yy = y + 2 * stepy[i];
		pair<int, int> pos = {xx, yy};
		if (!check_range(pos)) continue;
		if (isin(self, pos)) continue;
		ret.emplace_back(i);
	}
	return ret;
}

bool Maze::check_range(const pair<int, int>& pos)
{
	int x, y, h, w;
	x = pos.first;
	y = pos.second;
	h = maze.size();
	w = maze[0].size();
	if (0 <= x && x < w && 0 <= y && y < h) return true;
	else return false;
}


void Maze::turn(int& dir, const pair<int, int>& now)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	int x, y, xx, yy, xr, yr, right_hand;
	x = now.first;
	y = now.second;
	right_hand = dir + 1;
	if (right_hand > 3) right_hand = 0;
	xx = x + stepx[dir];
	yy = y + stepy[dir];
	xr = x + stepx[right_hand];
	yr = y + stepy[right_hand];
	if (!maze[yr][xr])
	{
		++dir;
		if (dir > 3) dir = 0;
		return;
	}
	else
	{
		if (!maze[yy][xx]) return;
		--dir;
		if (dir < 0) dir = 3;
		turn(dir, now);
	}
}

pair<int, int> Maze::bfs(const pair<int, int>& point)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	int x, y, x_max, y_max, dis_max;
	x = point.first;
	y = point.second;
	x_max = x;
	y_max = y;
	dis_max = 0;
	vector<vector<int>> dis(maze.size(), vector<int>(maze[0].size(), -1));
	queue<pair<int, int>> q;
	dis[y][x] = 0;
	q.push({x, y});
	while(!q.empty())
	{
		int xx, yy;
		xx = q.front().first;
		yy = q.front().second;
		q.pop();
		for (int i = 0; i < 4; ++i)
		{
			int xxx, yyy;
			xxx = xx + stepx[i];
			yyy = yy + stepy[i];
			if (!check_range({xxx, yyy})) continue;
			if (maze[yyy][xxx]) continue;
			if (dis[yyy][xxx] >= 0) continue;
			dis[yyy][xxx] = dis[yy][xx] + 1;
			q.push({xxx, yyy});
			if (dis[yyy][xxx] > dis_max)
			{
				x_max = xxx;
				y_max = yyy;
			}
		}
	}
	return {x_max, y_max};
}

void Maze::set_startgoal(long long seed, string INIT, pair<int, int> start, pair<int, int> goal)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	sx = start.first;
	sy = start.second;
	gx = goal.first;
	gy = goal.second;
	if (sx > -1 && sy > -1 && gx > -1 && gy > -1)
	{
		if (!check_range(start) || !check_range(goal) || maze[sy][sx] || maze[gy][gx])
		{
			cerr << __PRETTY_FUNCTION__ << " : Can't set start/goal" << endl;
			exit(-1);
		}
	}
	else
	{
		int which;
		random_device rnd;
		if (seed == 0) seed = rnd();
		mt19937 mt(seed);
		if (INIT == "CORNER") which = 0;
		else if (INIT == "FAR") which = 1;
		else which = 2;
		if (which == 0)
		{
			sx = 1;
			sy = 1;
			gx = maze[0].size() - 2;
			gy = maze.size() - 2;
		}
		else if(which == 1)
		{
			int mx, my;
			pair<int, int> strat, goal, middle;
			mx = 0;
			my = 0;
			while (maze[my][mx])
			{
				mx = mt() % (maze[0].size() - 2) + 1;
				my = mt() % (maze.size() - 2) + 1;
			}
			middle = {mx, my};
			start = bfs(middle);
			goal = bfs(start);
			sx = start.first;
			sy = start.second;
			gx = goal.first;
			gy = goal.second;
		}
		else
		{
			sx = 1;
			sy = 1;
			gx = 1;
			gy = 1;
			random_device rnd;
			int ct = 0;
			double rate_dis = RATE_DIS, D;
			D = sqrt((sx-gx)*(sx-gx)+(sy-gy)*(sy-gy));
			while (maze[sy][sx] || maze[gy][gx] || D < 2 || D < max(H, W)*rate_dis)
			{
				sx = mt() % (W+2);
				sy = mt() % (H+2);
				gx = mt() % (W+2);
				gy = mt() % (H+2);
				D = sqrt((sx-gx)*(sx-gx)+(sy-gy)*(sy-gy));
				if (ct > MAX) rate_dis = -1;
				else ++ct;
			}
		}
		maze[sy][sx] = 3;
		maze[gy][gx] = 4;
	}
}

void Maze::simplify_maze()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	vector<vector<bool>> tmp(maze.size(), vector<bool>(maze[0].size(), false));
	simple = tmp;
	tmp.clear();
	for (int y = 1; y < maze.size()-1; ++y)
	{
		for (int x = 1; x < maze[0].size()-1; ++x)
		{
			if (x == sx && y == sy) continue;
			if (x == gx && y == gy) continue;
			if (maze[y][x]) continue;
			int count, dir;
			count = 0;
			for (int i = 0; i < 4; ++i)
			{
				int xx, yy;
				xx = x + stepx[i];
				yy = y + stepy[i];
				if (!maze[yy][xx])
				{
					++count;
					dir = i;
				}
			}
			if (count == 1)
			{
				int xx, yy;
				xx = x;
				yy = y;
				while (true)
				{
					if (xx == sx && yy == sy) break;
					if (xx == gx && yy == gy) break;
					simple[yy][xx] = true;
					bool flag;
					int xxx, yyy, counter;
					flag = false;
					counter = dir + 2;
					if (counter > 3) counter -= 4;
					xxx = xx + stepx[dir];
					yyy = yy + stepy[dir];
					for (int i = 0; i < 4; ++i)
					{
						if (i == dir || i == counter) continue;
						int xxxx, yyyy;
						xxxx = xxx + stepx[i];
						yyyy = yyy + stepy[i];
						if (!maze[yyyy][xxxx])
						{
							flag = true;
							break;
						}
					}
					if (flag) break;
					xx = xxx;
					yy = yyy;
				}
			}
		}
	}
}

bool Maze::solve_maze(const string& search, int INT)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	distance = 0;
	search_num = 0;
	maze = maze_init;
	bool ret;
	if (search == "right") ret = solve_maze_right();
	else if (search == "sight") ret = solve_maze_sight();
	else if (search == "fool") ret = solve_maze_fool(INT);
	else if (search == "bfs") ret = solve_maze_bfs();
	else if (search == "dfs") ret = solve_maze_dfs();
	else if (search == "astar") ret = solve_maze_astar();
	else
	{
		cerr << __PRETTY_FUNCTION__ << " : SEARCH is wrong" << endl;
		exit(-1);
	}
	maze[sy][sx] = 3;
	maze[gy][gx] = 4;
	return ret;
}

bool Maze::solve_maze_right()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	pair<int, int> start, goal, now;
	vector<vector<int>> tmp = maze;
	int dir = 0, count = 0;
	start = {sx, sy};
	goal = {gx, gy};
	now = start;
	while(now != goal)
	{
		if (++count > H*W*100) return false;
		int x, y, xx, yy;
		x = now.first;
		y = now.second;
		tmp[y][x] = 2;
		++search_num;
		turn(dir, now);
		xx = x + stepx[dir];
		yy = y + stepy[dir];
		now = {xx, yy};
	}
	maze = tmp;
	return true;
}

bool Maze::solve_maze_sight()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	vector<vector<vector<int>>> corner(maze.size(), vector<vector<int>>(maze[0].size(), vector<int>(4, 0)));
	vector<vector<int>> tmp = maze;
	pair<int, int> start, goal, now;
	int dir = 0, limit = 0;
	bool find_goal;
	start = {sx, sy};
	goal = {gx, gy};
	simplify_maze();
	now = start;
	find_goal = sight_goal(dir, now, 0);
	/*
	int dx, dy, dct;
	dx = -1;
	dy = -1;
	dct = 0;
	*/
	while(now != goal)
	{
		int x, y, dir_prev, count;
		x = now.first;
		y = now.second;
		tmp[y][x] = 2;
		++search_num;
		dir_prev = dir;
		count = 0;
		if (find_goal)
		{
			now = {x+stepx[dir], y+stepy[dir]};
			continue;
		}
		if (++limit > H*W*100) return false;
		for (int i = 0; i < 4; ++i)
		{
			int xx, yy, counter;
			xx = x + stepx[i];
			yy = y + stepy[i];
			if (maze[yy][xx]) continue;
			if (simple[yy][xx]) continue;
			++count;
		}
		int counter;
		counter = dir + 2;
		if (counter > 3) counter -= 4;
		if (count == 0)
		{
			cout << __PRETTY_FUNCTION__ << " : Surrounded" << endl;
			cout << "(x, y) = (" << x << ", " << y << ")" << endl;
			exit(-1);
		}
		else if (count <= 2)
		{
			int xx, yy;
			xx = x + stepx[dir];
			yy = y + stepy[dir];
			if (maze[yy][xx] || simple[yy][xx])
			{
				for (int i = 0; i < 4; ++i)
				{
					if (count == 2 && i == counter) continue;
					if (!maze[y+stepy[i]][x+stepx[i]] && !simple[y+stepy[i]][x+stepx[i]])
					{
						dir = i;
						break;
					}
				}
			}
		}
		else if (count >= 3)
		{
			find_goal = sight_goal(dir, now, 0);
			if (find_goal)
			{
				now = {x+stepx[dir], y+stepy[dir]};
				continue;
			}
			vector<int> memo(3, 0);
			for (int i = 0; i < 4; ++i) ++memo[corner[y][x][i]];
			if (memo[0] == 4)
			{
				corner[y][x][counter] = 1;
				for (int i = 0; i < 4; ++i)
				{
					if (i == counter) continue;
					else if (maze[y+stepy[i]][x+stepx[i]] || simple[y+stepy[i]][x+stepx[i]]) continue;
					else
					{
						corner[y][x][i] = 2;
						dir = i;
					}
				}
				corner[y][x][dir] = 0;
			}
			else if (memo[2] > 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					if (corner[y][x][i] == 2)
					{
						corner[y][x][i] = 0;
						dir = i;
						break;
					}
				}
			}
			else if (memo[2] == 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					if (corner[y][x][i] == 1)
					{
						corner[y][x][i] = 0;
						dir = i;
						break;
					}
				}
			}
		}
		now = {x+stepx[dir], y+stepy[dir]};
		if (dir != dir_prev) find_goal = sight_goal(dir, now, 0);
	}
	maze = tmp;
	return true;
}

bool Maze::solve_maze_fool(const int& INT)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (INT <= 0)
	{
		cout << __PRETTY_FUNCTION__ << " : INT must be > 0" << endl;
		exit(-1);
	}
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	vector<vector<vector<int>>> corner(maze.size(), vector<vector<int>>(maze[0].size(), vector<int>(4, 0)));
	vector<vector<int>> tmp(maze.size(), vector<int>(maze[0].size(), 0));
	vector<vector<int>> tmp2 = maze;
	pair<int, int> start, goal, now;
	int dir, limit, when;
	bool find_goal, flag_never;
	count_never = tmp;
	start = {sx, sy};
	goal = {gx, gy};
	simplify_maze();
	now = start;
	dir = 0;
	limit = 0;
	when = -1;
	find_goal = sight_goal(dir, now, INT);
	flag_never = false;
	while(now != goal)
	{
		bool touch_wall, touch_never;
		int x, y, dir_prev, count, counter;
		++when;
		touch_wall = false;
		touch_never = false;
		x = now.first;
		y = now.second;
		tmp2[y][x] = 2;
		++search_num;
		dir_prev = dir;
		count = 0;
		counter = dir + 2;
		if (counter > 3) counter -= 4;
		if (find_goal)
		{
			now = {x+stepx[dir], y+stepy[dir]};
			continue;
		}
		if (++limit > H*W*100) return false;
		for (int i = 0; i < 4; ++i)
		{
			int xx, yy;
			xx = x + stepx[i];
			yy = y + stepy[i];
			if (maze[yy][xx] || simple[yy][xx]) touch_wall = true;
			else if (count_never[yy][xx] == INT) touch_never = true;
			else ++count;
		}
		if (count == 1) flag_never = true;
		else if (count == 2)
		{
			if (touch_never) flag_never = false;
		}
		else if (count >= 3) flag_never = false;
		if (flag_never && count_never[y][x] < INT)
		{
			++count_never[y][x];
			if (count_never[y][x] == INT) whens.push(when);
		}
		if (count == 0)
		{
			cout << __PRETTY_FUNCTION__ << " : Surrounded" << endl;
			cout << "(x, y) = (" << x << ", " << y << ")" << endl;
			exit(-1);
		}
		else if (count >= 1)
		{
			int xx, yy;
			xx = x + stepx[dir];
			yy = y + stepy[dir];
			find_goal = sight_goal(dir, now, INT);
			if (find_goal)
			{
				now = {xx, yy};
				continue;
			}
			Shuffle<int> S;
			for (int i = 0; i < 4; ++i)
			{
				if (count >= 2 && i == counter) continue;
				if (!maze[y+stepy[i]][x+stepx[i]] && !simple[y+stepy[i]][x+stepx[i]] && count_never[y+stepy[i]][x+stepx[i]] < INT)
				{
					S.push(i);
				}
			}
			dir = S.pop();
		}
		int xx, yy;
		xx = x + stepx[dir];
		yy = y + stepy[dir];
		now = {xx, yy};
		if (count >= 1) find_goal = sight_goal(dir, now, INT);
	}
	maze = tmp2;
	return true;
}

bool Maze::solve_maze_bfs()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	vector<vector<pair<int, pair<int, int>>>> dis(maze.size(), vector<pair<int, pair<int, int>>>(maze[0].size(), {-1, {-1, -1}}));
	queue<pair<int, int>> q;
	dis[sy][sx] = {0, {sy, sx}};
	q.push({sx, sy});
	while (!q.empty())
	{
		int x, y;
		x = q.front().first;
		y = q.front().second;
		q.pop();
		++search_num;
		for (int i = 0; i < 4; ++i)
		{
			int xx, yy;
			xx = x + stepx[i];
			yy = y + stepy[i];
			if (!check_range({xx, yy})) continue;
			if (maze[yy][xx]) continue;
			if (dis[yy][xx].first >= 0) continue;
			dis[yy][xx] = {dis[y][x].first+1, {x, y}};
			if (xx == gx && yy == gy)
			{
				q = {};
				break;
			}
			q.push({xx, yy});
		}
	}
	distance = dis[gy][gx].first;
	if (distance == -1) return false;
	else
	{
		pair<int, int> pos;
		pos = {gx, gy};
		while (true)
		{
			int x, y;
			x = pos.first;
			y = pos.second;
			if (x == sx && y == sy) break;
			pos = dis[y][x].second;
			maze[y][x] = 2;
		}
		return true;
	}
}

bool Maze::solve_maze_dfs()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	vector<vector<pair<int, pair<int, int>>>> dis(maze.size(), vector<pair<int, pair<int, int>>>(maze[0].size(), {-1, {-1, -1}}));
	stack<pair<int, int>> q;
	dis[sy][sx] = {0, {sy, sx}};
	q.push({sx, sy});
	while (!q.empty())
	{
		int x, y;
		x = q.top().first;
		y = q.top().second;
		q.pop();
		++search_num;
		for (int i = 0; i < 4; ++i)
		{
			int xx, yy;
			xx = x + stepx[i];
			yy = y + stepy[i];
			if (!check_range({xx, yy})) continue;
			if (maze[yy][xx]) continue;
			if (dis[yy][xx].first >= 0) continue;
			dis[yy][xx] = {dis[y][x].first+1, {x, y}};
			if (xx == gx && yy == gy)
			{
				q = {};
				break;
			}
			q.push({xx, yy});
		}
	}
	distance = dis[gy][gx].first;
	if (distance == -1) return false;
	else
	{
		pair<int, int> pos;
		pos = {gx, gy};
		while (true)
		{
			int x, y;
			x = pos.first;
			y = pos.second;
			if (x == sx && y == sy) break;
			pos = dis[y][x].second;
			maze[y][x] = 2;
		}
		return true;
	}
}

bool Maze::solve_maze_astar(string HEU)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	vector<vector<pair<int, pair<int, int>>>> dis(maze.size(), vector<pair<int, pair<int, int>>>(maze[0].size(), {-1, {-1, -1}}));
	vector<vector<double>> dis_sum(maze.size(), vector<double>(maze[0].size(), 0.0));
	priority_queue< pair<pair<double, int>, pair<int, int>>, vector<pair<pair<double, int>, pair<int, int>>>, greater<pair<pair<double, int>, pair<int, int>>> > q;
	dis[sy][sx] = {0, {sy, sx}};
	dis_sum[sy][sx] = (double)dis[sy][sx].first + heuristic({gx, gy}, HEU);
	q.push({{dis_sum[sy][sx], ++order}, {sx, sy}});
	while (!q.empty())
	{
		int x, y;
		x = q.top().second.first;
		y = q.top().second.second;
		q.pop();
		++search_num;
		for (int i = 0; i < 4; ++i)
		{
			int xx, yy;
			xx = x + stepx[i];
			yy = y + stepy[i];
			if (!check_range({xx, yy})) continue;
			if (maze[yy][xx]) continue;
			if (dis[yy][xx].first >= 0) continue;
			dis[yy][xx] = {dis[y][x].first+1, {x, y}};
			dis_sum[yy][xx] = (double)dis[yy][xx].first + heuristic({xx, yy}, HEU);
			if (xx == gx && yy == gy)
			{
				q = {};
				break;
			}
			q.push({{dis_sum[yy][xx], ++order}, {xx, yy}});
		}
	}
	distance = dis[gy][gx].first;
	if (distance == -1) return false;
	else
	{
		pair<int, int> pos;
		pos = {gx, gy};
		while (true)
		{
			int x, y;
			x = pos.first;
			y = pos.second;
			if (x == sx && y == sy) break;
			pos = dis[y][x].second;
			maze[y][x] = 2;
		}
		return true;
	}
}

double Maze::heuristic(const pair<int, int>& pos, const string& HEU)
{
	if (HEU == "Manhattan") return Manhattan(pos);
	else if (HEU == "Euclidian") return Euclidean(pos);
	else
	{
			cerr << __PRETTY_FUNCTION__ << " : HEURISTIC is wrong" << endl;
			exit(-1);
	}
}

double Maze::Manhattan(const pair<int, int>& pos)
{
	double x, y, dis;
	x = (double)pos.first;
	y = (double)pos.second;
	dis = abs(x - (double)gx) + abs(y - (double)gy);
	return dis;
}

double Maze::Euclidean(const pair<int, int>& pos)
{
	double x, y, dis;
	x = (double)pos.first;
	y = (double)pos.second;
	dis = sqrt(abs(x - gx)*abs(x - gx)  + abs(y - gy)*abs(y - gy));
	return dis;
}

bool Maze::sight_goal(int& dir, const pair<int, int>& pos, const int& INT)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	int x, y;
	x = pos.first;
	y = pos.second;
	if (maze[y][x] || simple[y][x]) return false;
	for (int i = 0; i < 4; ++i)
	{
		int xx, yy;
		xx = x + stepx[i];
		yy = y + stepy[i];
		if (INT > 0 && count_never[yy][xx] == INT) continue;
		while (!maze[yy][xx] && !simple[yy][xx])
		{
			if (INT > 0 && count_never[yy][xx] == INT) break;
			if (xx == gx && yy == gy)
			{
				dir = i;
				return true;
			}
			xx += stepx[i];
			yy += stepy[i];
		}
	}
	return false;
}

void Maze::print_maze()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	for (auto squares: maze)
	{
		for (auto square: squares)
		{
			if (!ZENKAKU)
			{
				if (square == 1) cout << Wall;
				else if (square == 0) cout << Path;
				else if (square == 2) cout << Answer;
				else if (square == 3) cout << Start;
				else if (square == 4) cout << Goal;
			}
			else
			{
				if (square == 1) cout << WALL;
				else if (square == 0) cout << PATH;
				else if (square == 2) cout << ANSWER;
				else if (square == 3) cout << START;
				else if (square == 4) cout << GOAL;
			}
		}
		cout << endl;
	}
}

/*
int main()
{
	Maze M(45, 35);
	Time T("microseconds");

	random_device rnd;
	long long seed = rnd();

	M.generate_maze("kabenobashi_2", seed);
	M.set_startgoal(seed, "FAR");
	printf("start : (%d, %d), goal : (%d, %d)\n\n", M.sx, M.sy, M.gx, M.gy);

	T.start();
	M.solve_maze("astar");
	T.elapse();
	M.print_maze();
	printf("A*    : distance %d, search %d\n", M.distance, M.search_num);
	T.print_time();
	cout << endl;

	T.start();
	M.solve_maze("bfs");
	T.elapse();
	cout << endl;
	M.print_maze();
	printf("BFS   : distance %d, search %d\n", M.distance, M.search_num);
	T.print_time();
	cout << endl;

	T.start();
	M.solve_maze("dfs");
	T.elapse();
	cout << endl;
	M.print_maze();
	printf("DFS   : distance %d, search %d\n", M.distance, M.search_num);
	T.print_time();
	cout << endl;

	T.start();
	M.solve_maze("right");
	T.elapse();
	cout << endl;
	M.print_maze();
	printf("Right : distance %d, search %d\n", M.distance, M.search_num);
	T.print_time();
	cout << endl;

	T.start();
	M.solve_maze("sight");
	T.elapse();
	cout << endl;
	M.print_maze();
	printf("Sight : distance %d, search %d\n", M.distance, M.search_num);
	T.print_time();
	cout << endl;

	T.start();
	M.solve_maze("fool");
	T.elapse();
	cout << endl;
	M.print_maze();
	printf("Fool  : distance %d, search %d\n", M.distance, M.search_num);
	T.print_time();
	cout << endl;

	return 0;
}
*/
