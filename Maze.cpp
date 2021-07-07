// モバイルC(iOS)

#include "Time.cpp"
#include "Shuffle.cpp"
#include "ControlConsole.cpp"

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
#define DYNAMIC true // trueにすると迷路生成/探索の過程を動的に表示する

#define ZENKAKU true	// trueにすると全角文字で表示
#define SEED 0 // 任意のseed値を設定でき、0にするとランダムに決まる
#define SEEDFILE "maze_seed.txt" // seed値、パラメータをSEEDFILEに出力

/* 迷路生成アルゴリズムを選択 */
//#define TYPE "botaoshi" // 棒倒し法
//#define TYPE "anahori" // 穴掘り法
//#define TYPE "kabenobashi_1" // 壁伸ばし法(壁から)
#define TYPE "kabenobashi_2" // 壁伸ばし法(通路から)

/* 迷路探索アルゴリズムを選択 */
//#define SEARCH "right" // 右手法
//#define SEARCH "sight" // 直線の先に何があるか見え経路記憶しながら進む
//#define SEARCH "fool" // 直線の先に何があるか見えるが記憶力が悪い
//#define SEARCH "bfs" // 幅優先探索
#define SEARCH "dfs" // 深さ優先探索
//#define SEARCH "astar" // Ａ*アルゴリズム

/* Ａ*アルゴリズムにおけるヒューリスティック関数を選択 */
#define HEURISTIC "MANHATTAN" // マンハッタン距離
//#define HEURISTIC "EUCLIDEAN" // ユークリッド距離
//#define HEURISTIC "ZERO"	// ダイクストラ法

/* foolの賢さ(INT)を設定 */
#define INT 1 // INT回突き当たった道は通らなくなる

/* DYNAMICをtrueにしたときの表示間隔を設定(ms単位) */
#define SLEEPMAKE 0 // 迷路生成
#define SLEEPSEARCH 0 // 迷路探索
#define SLEEPSIMPLIFY 0 // 迷路簡略化( simplify_maze_visualize() )
#define ONLYSIMPLIFY true // trueにすると簡略化の過程だけ表示
#define SHOW 1000 // 簡略化前の迷路を表示する時間

/* スタートとゴールの位置を設定 */
#define CORNER false // trueにすると左上端にスタート、右下端にゴールを設定
#define FAR true // trueにすると最長経路の端にスタート/ゴールを設定
/*
CORNERとFARのいずれともfalseの場合ランダムに配置されるが
その時のスタートとゴールのユークリッド距離(D)を設定
*/
#define RATE_DIS 0.8 // D >= max(H, W)*RATE_DIS になるように設定
#define MAX 30000 // スタート/ゴールの再配置回数の上限

/* 壁や通路などを表現する記号を設定 */
// HANKAKU
#define Path " "
#define Wall "\033[47m#\033[0m"
#define Now "\033[41m.\033[0m"
#define Reached "\033[46m_\033[0m"
#define Answer "\033[41m*\033[0m"
#define Start "\033[43mS\033[0m"
#define Goal "\033[42mG\033[0m"
#define Up "\033[31m^\033[0m"
#define Right "\033[31m>\033[0m"
#define Down "\033[31mv\033[0m"
#define Left "\033[31m<\033[0m"
#define Simple "\033[47m \033[0m"
#define Never "\033[45m_\033[0m"
// ZENKAKU
#define PATH "　"
#define WALL "\033[47m■\033[0m"
#define NOW "\033[41m　\033[0m"
#define REACHED "\033[36m□\033[0m"
#define ANSWER "\033[31m■\033[0m"
#define START "\033[43mＳ\033[0m"
#define GOAL "\033[42mＧ\033[0m"
#define UP "\033[31m▲\033[0m"
#define RIGHT "\033[31m▶\033[0m"
#define DOWN "\033[31m▼\033[0m"
#define LEFT "\033[31m◀\033[0m"
#define SIMPLE "\033[47m　\033[0m"
#define NEVER "\033[31m□\033[0m"

struct Maze
{
	long long seed;
	int H, W,	sx, sy, gx, gy, distance, search_num, order;
	queue<pair<int, int>> reached, made;
	queue<pair<pair<int, int>, int>> reached_dir;
	queue<int> whens;
	stack<pair<int, int>> path;
	vector<vector<int>> maze, maze_path, maze_init, count_never;
	vector<vector<bool>> simple;
	Shuffle<pair<int, int>> rest;
	ControlConsole C;
	bool C_flag, onlysimplify;

	Maze();

	bool isNumber(const string&);
	template<class T> bool iszero(const vector<vector<T>>&);
	template<class T> bool isin(const vector<T>&, const T&);

	void get_size(int hh=0, int ww=0);
	void generate_maze(string type="");
	void reset_maze_botaoshi();
	void generate_maze_botaoshi();
	void reset_maze_anahori();
	void generate_maze_anahori();
	void reset_maze_kabenobashi_1();
	void reset_maze_kabenobashi_2();
	void generate_maze_kabenobashi_1();
	void generate_maze_kabenobashi_2();
	vector<int> is_able_anahori(const int&, const int&);
	vector<int> is_able_kabenobashi_1(const int&, const int&);
	vector<int> is_able_kabenobashi_2(const int&, const int&, const vector<pair<int, int>>&);
	bool check_range(const pair<int, int>&);
	void turn(int&, const pair<int, int>&);
	pair<int, int> bfs(const pair<int, int>&);
	void set_startgoal(pair<int, int> start={-1,-1}, pair<int, int> goal={-1,-1});
	void simplify_maze();
	void simplify_maze_visualize();
	bool solve_maze();
	bool solve_maze_right();
	bool solve_maze_sight();
	bool solve_maze_fool();
	bool solve_maze_bfs();
	bool solve_maze_dfs();
	bool solve_maze_astar();
	double heuristic(const pair<int, int>&);
	double Manhattan(const pair<int, int>&);
	double Euclidean(const pair<int, int>&);
	bool sight_goal(int&, const pair<int, int>&);
	void print_maze();
	void print_maze_path(bool all=false, vector<vector<int>> MAZE={});
	void how_to_solve();
	void how_to_make(string type="");
};

Maze::Maze()
{
	H = 0;
	W = 0;
	sx = 1;
	sy = 1;
	gx = 1;
	gy = 1;
	distance = -1;
	order = 0;
	C_flag = true;
	random_device rnd;
	if (SEED == 0) seed = rnd();
	else seed = SEED;
	Shuffle<pair<int, int>> tmp(seed);
	rest = tmp;
	onlysimplify = false;
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

void Maze::get_size(int hh, int ww)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (hh > 0 && ww > 0) return;
	string h, w;
	while (true)
	{
		cout << "H, W : ";
		cin >> h >> w;
		if (isNumber(h) && isNumber(w))
		{
			H = stoi(h);
			W = stoi(w);
			break;
		}
	}
}

void Maze::generate_maze(string type)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (type == "") type = TYPE;
	if (type == "botaoshi") generate_maze_botaoshi();
	else if (type == "anahori") generate_maze_anahori();
	else if (type == "kabenobashi_1") generate_maze_kabenobashi_1();
	else if (type == "kabenobashi_2") generate_maze_kabenobashi_2();
	else
	{
		cerr << __PRETTY_FUNCTION__ << " : Type is wrong" << endl;
		exit(-1);
	}
	sx = 1;
	sy = 1;
	gx = maze[0].size() - 2;
	gy = maze.size() - 2;
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

void Maze::generate_maze_botaoshi()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : MazeSize is small" << endl;
		exit(-1);
	}
	reset_maze_botaoshi();
	ofstream ofs(SEEDFILE, ios::app);
	Time T;
	ofs << seed << " " << "H" << H << "W" << W << " botaoshi " << T.getDatetimeStr() << endl;
	mt19937 mt(seed);
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	made = {};
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
				made.push({xx, yy});
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
	made = {};
	rest.clear();
	if (H % 2 == 0) ++H;
	if (W % 2 == 0) ++W;
	vector<vector<int>> tmp(H+2, vector<int>(W+2, true));
	maze = tmp;
	maze_init = tmp;
}

void Maze::generate_maze_anahori()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : MazeSize is small" << endl;
		exit(-1);
	}
	reset_maze_anahori();
	ofstream ofs(SEEDFILE, ios::app);
	Time T;
	ofs << seed << " " << "H" << H << "W" << W << " anahori " << T.getDatetimeStr() << endl;
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
	made.push({x, y});
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
			made.push({nx - stepx[dir], ny - stepy[dir]});
			made.push(next);
			maze[ny][nx] = false;
			maze[ny - stepy[dir]][nx - stepx[dir]] = false;
			x = nx;
			y = ny;
			if (!is_able_anahori(nx, ny).empty()) rest.push(next);
		}
	}
}

void Maze::reset_maze_kabenobashi_1()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : Maze size is small" << endl;
		exit(-1);
	}
	if (H % 2 == 0) ++H;
	if (W % 2 == 0) ++W;
	made = {};
	rest.clear();
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

void Maze::reset_maze_kabenobashi_2()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : Maze size is small" << endl;
		exit(-1);
	}
	if (H % 2 == 0) ++H;
	if (W % 2 == 0) ++W;
	made = {};
	rest.clear();
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

void Maze::generate_maze_kabenobashi_1()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : MazeSize is small" << endl;
		exit(-1);
	}
	reset_maze_kabenobashi_1();
	ofstream ofs(SEEDFILE, ios::app);
	Time T;
	ofs << seed << " " << "H" << H << "W" << W << " kabenobashi_1 " << T.getDatetimeStr() << endl;
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
			made.push({nx - stepx[dir], ny - stepy[dir]});
			made.push(next);
			maze[ny][nx] = true;
			maze[ny - stepy[dir]][nx - stepx[dir]] = true;
			x = nx;
			y = ny;
			if (!is_able_kabenobashi_1(nx, ny).empty()) rest.push(next);
		}
	}
}

void Maze::generate_maze_kabenobashi_2()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	if (H < 3 || W < 3)
	{
		cerr << __PRETTY_FUNCTION__ << " : MazeSize is small" << endl;
		exit(-1);
	}
	reset_maze_kabenobashi_2();
	ofstream ofs(SEEDFILE, ios::app);
	Time T;
	ofs << seed << " " << "H" << H << "W" << W << " kabenobashi_2 " << T.getDatetimeStr() << endl;
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
				made.push({xx, yy});
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

void Maze::set_startgoal(pair<int, int> start, pair<int, int> goal)
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
		if (CORNER)
		{
			cout << __PRETTY_FUNCTION__ << " : Set start/goal at CORNER" << endl;
			sx = 1;
			sy = 1;
			gx = maze[0].size() - 2;
			gy = maze.size() - 2;
		}
		else if(FAR)
		{
			cout << __PRETTY_FUNCTION__ << " : Set start/goal at FAR" << endl;
			int mx, my;
			pair<int, int> strat, goal, middle;
			random_device rnd;
			mx = 0;
			my = 0;
			while (maze[my][mx])
			{
				mx = rnd() % (maze[0].size() - 2) + 1;
				my = rnd() % (maze.size() - 2) + 1;
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
			cout << __PRETTY_FUNCTION__ << " : Set start/goal randomly" << endl;
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
				sx = rnd() % (W+2);
				sy = rnd() % (H+2);
				gx = rnd() % (W+2);
				gy = rnd() % (H+2);
				D = sqrt((sx-gx)*(sx-gx)+(sy-gy)*(sy-gy));
				if (ct > MAX) rate_dis = -1;
				else ++ct;
			}
		}
		printf("start : (%d, %d), goal : (%d, %d)\n", sx, sy, gx, gy);
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

void Maze::simplify_maze_visualize()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	onlysimplify = ONLYSIMPLIFY;
	made.push({-1, -1});
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
					made.push({xx, yy});
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
	how_to_make();
}

bool Maze::solve_maze()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	distance = 0;
	search_num = 0;
	set_startgoal();
	reached = {};
	reached_dir = {};
	path = {};
	maze_path = maze;
	if (SEARCH == "right") return solve_maze_right();
	else if (SEARCH == "sight") return solve_maze_sight();
	else if (SEARCH == "fool") return solve_maze_fool();
	else if (SEARCH == "bfs") return solve_maze_bfs();
	else if (SEARCH == "dfs") return solve_maze_dfs();
	else if (SEARCH == "astar") return solve_maze_astar();
	else
	{
		cerr << __PRETTY_FUNCTION__ << " : SEARCH is wrong" << endl;
		exit(-1);
	}
}

bool Maze::solve_maze_right()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	pair<int, int> start, goal, now;
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
		++search_num;
		reached_dir.push({now, dir});
		maze_path[y][x] = 2;
		turn(dir, now);
		xx = x + stepx[dir];
		yy = y + stepy[dir];
		now = {xx, yy};
	}
	return true;
}

bool Maze::solve_maze_sight()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	vector<vector<vector<int>>> corner(maze.size(), vector<vector<int>>(maze[0].size(), vector<int>(4, 0)));
	pair<int, int> start, goal, now;
	int dir = 0, limit = 0;
	bool find_goal;
	start = {sx, sy};
	goal = {gx, gy};
	simplify_maze();
	now = start;
	find_goal = sight_goal(dir, now);
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
		++search_num;
		dir_prev = dir;
		count = 0;
		reached_dir.push({now, dir});
		maze_path[y][x] = 2;
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
		/*
		if (x == dx && y == dy) ++dct;
		else dct = 0;
		cout << "x : " << x << ", y : " << y;
		printf(", count : %d, dir : %d, counter : %d\n", count, dir, counter);
		if (dct > 3) exit(-1);
		dx = x;
		dy = y;
		*/
		if (count == 0)
		{
			how_to_solve();
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
			find_goal = sight_goal(dir, now);
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
		if (dir != dir_prev) find_goal = sight_goal(dir, now);
	}
	return true;
}

bool Maze::solve_maze_fool()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	const static vector<int> stepx = {0, 1, 0, -1};
	const static vector<int> stepy = {-1, 0, 1, 0};
	vector<vector<vector<int>>> corner(maze.size(), vector<vector<int>>(maze[0].size(), vector<int>(4, 0)));
	vector<vector<int>> tmp(maze.size(), vector<int>(maze[0].size(), 0));
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
	find_goal = sight_goal(dir, now);
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
		++search_num;
		dir_prev = dir;
		count = 0;
		counter = dir + 2;
		if (counter > 3) counter -= 4;
		reached_dir.push({now, dir});
		maze_path[y][x] = 2;
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
			how_to_solve();
			cout << __PRETTY_FUNCTION__ << " : Surrounded" << endl;
			cout << "(x, y) = (" << x << ", " << y << ")" << endl;
			exit(-1);
		}
		else if (count >= 1)
		{
			int xx, yy;
			xx = x + stepx[dir];
			yy = y + stepy[dir];
			find_goal = sight_goal(dir, now);
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
			if (S.empty()) how_to_solve();
			dir = S.pop();
		}
		int xx, yy;
		xx = x + stepx[dir];
		yy = y + stepy[dir];
		now = {xx, yy};
		if (count >= 1) find_goal = sight_goal(dir, now);
	}
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
		reached.push({x, y});
		maze_path[y][x] = 3;
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
			path.push(pos);
			x = pos.first;
			y = pos.second;
			maze_path[y][x] = 2;
			if (x == sx && y == sy) break;
			pos = dis[y][x].second;
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
		reached.push({x, y});
		maze_path[y][x] = 3;
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
			path.push(pos);
			x = pos.first;
			y = pos.second;
			maze_path[y][x] = 2;
			if (x == sx && y == sy) break;
			pos = dis[y][x].second;
		}
		return true;
	}
}

bool Maze::solve_maze_astar()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	vector<int> stepx = {0, 0, 1, -1}, stepy = {1, -1, 0, 0};
	vector<vector<pair<int, pair<int, int>>>> dis(maze.size(), vector<pair<int, pair<int, int>>>(maze[0].size(), {-1, {-1, -1}}));
	vector<vector<double>> dis_sum(maze.size(), vector<double>(maze[0].size(), 0.0));
	priority_queue< pair<pair<double, int>, pair<int, int>>, vector<pair<pair<double, int>, pair<int, int>>>, greater<pair<pair<double, int>, pair<int, int>>> > q;
	dis[sy][sx] = {0, {sy, sx}};
	dis_sum[sy][sx] = (double)dis[sy][sx].first + heuristic({gx, gy});
	q.push({{dis_sum[sy][sx], ++order}, {sx, sy}});
	while (!q.empty())
	{
		int x, y;
		x = q.top().second.first;
		y = q.top().second.second;
		q.pop();
		++search_num;
		reached.push({x, y});
		maze_path[y][x] = 3;
		for (int i = 0; i < 4; ++i)
		{
			int xx, yy;
			xx = x + stepx[i];
			yy = y + stepy[i];
			if (!check_range({xx, yy})) continue;
			if (maze[yy][xx]) continue;
			if (dis[yy][xx].first >= 0) continue;
			dis[yy][xx] = {dis[y][x].first+1, {x, y}};
			dis_sum[yy][xx] = (double)dis[yy][xx].first + heuristic({xx, yy});
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
			path.push(pos);
			x = pos.first;
			y = pos.second;
			maze_path[y][x] = 2;
			if (x == sx && y == sy) break;
			pos = dis[y][x].second;
		}
		return true;
	}
}

double Maze::heuristic(const pair<int, int>& pos)
{
	if (HEURISTIC == "MANHATTAN") return Manhattan(pos);
	else if (HEURISTIC == "EUCLIDEAN") return Euclidean(pos);
	else if (HEURISTIC == "ZERO") return 0.0;
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

bool Maze::sight_goal(int& dir, const pair<int, int>& pos)
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
		if (SEARCH == "fool" && count_never[yy][xx] == INT) continue;
		while (!maze[yy][xx] && !simple[yy][xx])
		{
			if (SEARCH == "fool" && count_never[yy][xx] == INT) break;
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
				if (square) cout << Wall;
				else cout << Path;
			}
			else
			{
				if (square) cout << WALL;
				else cout << PATH;
			}
		}
		cout << endl;
	}
}

void Maze::print_maze_path(bool all, vector<vector<int>> MAZE)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	int x, y;
	x = 0;
	y = 0;
	if (MAZE.empty()) MAZE = maze_path;
	vector<string> M(MAZE.size());
	for (auto squares: MAZE)
	{
		string m = "";
		for (auto square: squares)
		{
			if (!ZENKAKU)
			{
				if (x == sx && y == sy) m += Start;
				else if (x == gx && y == gy) m += Goal;
				else if (square == 0) m += Path;
				else if (square == 1) m += Wall;
				else if (square == 2) m += Answer;
				else if (square == 3)
				{
					if (all) m += Reached;
					else m += Path;
				}
				else if (square == 4)
				{
					if (all) m += Now;
					else m += Wall;
				}
				else if (square == 5)
				{
					if (all) m += Up;
					else m += Wall;
				}
				else if (square == 6)
				{
					if (all) m += Right;
					else m += Wall;
				}
				else if (square == 7)
				{
					if (all) m += Down;
					else m += Wall;
				}
				else if (square == 8)
				{
					if (all) m += Left;
					else m += Wall;
				}
				else if (square == 9)
				{
					if (all) m += Simple;
					else m += Wall;
				}
				else if (square == 10)
				{
					if (all) m += Never;
					else m += Path;
				}
			}
			else
			{
				if (x == sx && y == sy) m += START;
				else if (x == gx && y == gy) m += GOAL;
				else if (square == 0) m += PATH;
				else if (square == 1) m += WALL;
				else if (square == 2) m += ANSWER;
				else if (square == 3)
				{
					if (all) m += REACHED;
					else m += PATH;
				}
				else if (square == 4)
				{
					if (all) m += NOW;
					else m += WALL;
				}
				else if (square == 5)
				{
					if (all) m += UP;
					else m += WALL;
				}
				else if (square == 6)
				{
					if (all) m += RIGHT;
					else m += WALL;
				}
				else if (square == 7)
				{
					if (all) m += DOWN;
					else m += WALL;
				}
				else if (square == 8)
				{
					if (all) m += LEFT;
					else m += WALL;
				}
				else if (square == 9)
				{
					if (all) m += SIMPLE;
					else m += WALL;
				}
				else if (square == 10)
				{
					if (all) m += NEVER;
					else m += PATH;
				}
			}
			++x;
		}
		M[y] = m;
		++y;
		x = 0;
	}
	if (DYNAMIC)
	{
		if (C_flag)
		{
			C.EraseDisplayEntire();
			C.Puts(M);
			C_flag = false;
		}
		else C.RewriteDisplay(M);
	}
	else
	{
		for (string s: M) cout << s << endl;
	}
}

void Maze::how_to_solve()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	C_flag = true;
	vector<vector<int>> how;
	how = maze;
	if (SEARCH == "right" || SEARCH == "sight" || SEARCH == "fool")
	{
		int ct = 0;
		while (!reached_dir.empty())
		{
			int x, y, dir, id;
			x = reached_dir.front().first.first;
			y = reached_dir.front().first.second;
			dir = reached_dir.front().second + 5;
			reached_dir.pop();
			how[y][x] = dir;
			print_maze_path(true, how);
			if (!whens.empty())
			{
				if (whens.front() == ct)
				{
					how[y][x] = 10;
					whens.pop();
				}
				else how[y][x] = 3;
			}
			else how[y][x] = 3;
			if (DYNAMIC) Sleep(SLEEPSEARCH);
			++ct;
		}
	}
	else
	{
		while (!reached.empty())
		{
			int x, y;
			x = reached.front().first;
			y = reached.front().second;
			reached.pop();
			how[y][x] = 4;
			print_maze_path(true, how);
			how[y][x] = 3;
			if (DYNAMIC) Sleep(SLEEPSEARCH);
		}
	}
	print_maze_path(true);
	if (DYNAMIC) Sleep(SLEEPSEARCH);
}

void Maze::how_to_make(string type)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	C_flag = true;
	if (type == "") type = TYPE;
	int sleepmake = SLEEPMAKE;
	bool simplify;
	vector<vector<int>> how;
	simplify = false;
	how = maze_init;
	print_maze_path(true, how);
	if (DYNAMIC) Sleep(sleepmake);
	while (!made.empty())
	{
		int x, y;
		x = made.front().first;
		y = made.front().second;
		made.pop();
		if (x == -1 || y == -1)
		{
			simplify = true;
			sleepmake = SLEEPSIMPLIFY;
			print_maze_path(true, how);
		 if (DYNAMIC) Sleep(SHOW);
			continue;
		}
		how[y][x] = 4;
		if (onlysimplify)
		{
			if (simplify) print_maze_path(true, how);
		}
		else print_maze_path(true, how);
		if (simplify) how[y][x] = 9;
		else if (type == "botaoshi" || type == "kabenobashi_1" || type == "kabenobashi_2") how[y][x] = 1;
		else if (type == "anahori") how[y][x] = 0;
		else
		{
			cerr << "Type is wrong" << endl;
			exit(-1);
		}
		if (onlysimplify)
		{
			if (DYNAMIC && simplify) Sleep(sleepmake);
		}
		else if (DYNAMIC) Sleep(sleepmake);
	}
		print_maze_path(true, how);
		if (DYNAMIC) Sleep(sleepmake);
}

int main()
{
	Maze M;
	Time T("microseconds");
	string t_gnrt, t_slv;
	M.get_size();

	T.start();
	M.generate_maze();
	T.elapse();
	t_gnrt = T.time_s;

//	M.set_startgoal(); M.simplify_maze_visualize();

	T.start();
	M.solve_maze();
	T.elapse();
	t_slv = T.time_s;

	M.how_to_make();
	M.how_to_solve();

//	M.solve_maze();
//	M.print_maze();
//	M.print_maze_path();
//	M.how_to_make();
//	M.how_to_solve();
//	M.print_maze();
//	M.print_maze_path();

	if (DYNAMIC) M.C.MoveCursorEnd();
	cout << endl;
	printf("           SEED : %ld\n           TYPE : %s\n         SEARCH : %s\n       Distance : %d\n     Search-Num : %d\nGenerating Time : %s\n   Solving Time : %s\n", M.seed, TYPE, SEARCH, M.distance, M.search_num, t_gnrt.c_str(), t_slv.c_str());

	return 0;
}
