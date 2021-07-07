// モバイルC(iOS)

#include "Time.cpp"
#include "ControlConsole.cpp"

#include <iostream>
#include <random>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

#define DEBUG false
#define ZENKAKU true
#define SEED 0
#define SEEDFILE "LifeGame_seed.txt"
#define RATE_LIVE 0.2
#define ALIVE "■"
#define DEAD "□"
#define Alive "+"
#define Dead "."
#define SLEEP 20
#define SLEEP_END 300
#define STEP 120
#define DYNAMIC true
#define HMAX 47
#define WMAX 37
#define ROOP false

struct LifeGame
{
	ControlConsole C;
	bool C_flag;
	int H, W, alive, dead;
	long long seed;
	vector<vector<bool>> now, next;
	vector<int> stepx, stepy;
	vector<vector<bool>> heart, galaxy, glider, spaceship_s, spaceship_m, spaceship_l, glider_gun, pentadecathlon, pulsar, puffer_train, puffer_train2, flying_machine, flying_machine2, block_laying_switch_engine, honeycomb;

	LifeGame();

	void get_size(int hh=0, int ww=0);
	bool isNumber(const string&);

	void init();
	void set_random();
	void set_any(const vector<vector<bool>>&, const pair<int, int>&);
	void set_center(vector<vector<bool>>&);
	pair<int, int> get_center(const vector<vector<bool>>&);
	int count_surrounding(const int&, const int&);
	bool check_range(const int&, const int&);
	bool check_cell(const int&, const int&);
	void check_cells();
	int count_now();
	bool comp_nownext();
	void print_now();
	void show(int step=0);
};

LifeGame::LifeGame():H(0), W(0), C_flag(true)
{
	stepx = {0, 0, 1, 1, 1, -1, -1, -1};
	stepy = {1, -1, 0, 1, -1, 0, 1, -1};
	heart = {{1, 0, 1},\
		        {1, 1, 1},\
		        {1, 1, 1}};
	galaxy = {{1, 1, 1, 1, 1, 1, 0, 1, 1},\
	          {1, 1, 1, 1, 1, 1, 0, 1, 1},\
	          {0, 0, 0, 0, 0, 0, 0, 1, 1},\
	          {1, 1, 0, 0, 0, 0, 0, 1, 1},\
	          {1, 1, 0, 0, 0, 0, 0, 1, 1},\
	          {1, 1, 0, 0, 0, 0, 0, 1, 1},\
	          {1, 1, 0, 0, 0, 0, 0, 0, 0},\
	          {1, 1, 0, 1, 1, 1, 1, 1, 1},\
	          {1, 1, 0, 1, 1, 1, 1, 1, 1}};
	glider = {{1, 1, 1},\
	          {1, 0, 0},\
	          {0, 1, 0}};
	spaceship_s = {{0, 1, 0, 0, 1},\
	               {1, 0, 0, 0, 0},\
	               {1, 0, 0, 0, 1},\
	               {1, 1, 1, 1, 0}};
	spaceship_m = {{0, 0, 0, 1, 0, 0},\
		             {0, 1, 0, 0, 0, 1},\
	               {1, 0, 0, 0, 0, 0},\
	               {1, 0, 0, 0, 0, 1},\
	               {1, 1, 1, 1, 1, 0}};
	spaceship_l = {{0, 0, 0, 1, 1, 0, 0},\
		             {0, 1, 0, 0, 0, 0, 1},\
	               {1, 0, 0, 0, 0, 0, 0},\
	               {1, 0, 0, 0, 0, 0, 1},\
	               {1, 1, 1, 1, 1, 1, 0}};
 glider_gun = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},\
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},\
               {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
               {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	pentadecathlon = {{0, 1, 0, 0, 0, 0, 1, 0},\
	                  {1, 1, 0, 0, 0, 0, 1, 1},\
	                  {0, 1, 0, 0, 0, 0, 1, 0}};
	pulsar = {{1, 0, 0, 0, 1},\
	          {1, 0, 1, 0, 1},\
	          {1, 0, 0, 0, 1}};
	puffer_train = {{0, 0, 0, 1, 0},\
	                {0, 0, 0, 0, 1},\
	                {1, 0, 0, 0, 1},\
	                {0, 1, 1, 1, 1},\
	                {0, 0, 0, 0, 0},\
	                {0, 0, 0, 0, 0},\
	                {0, 0, 0, 0, 0},\
	                {1, 0, 0, 0, 0},\
	                {0, 1, 1, 0, 0},\
	                {0, 0, 1, 0, 0},\
	                {0, 0, 1, 0, 0},\
	                {0, 1, 0, 0, 0},\
	                {0, 0, 0, 0, 0},\
	                {0, 0, 0, 0, 0},\
	                {0, 0, 0, 1, 0},\
	                {0, 0, 0, 0, 1},\
	                {1, 0, 0, 0, 1},\
	                {0, 1, 1, 1, 1}};
	puffer_train2 = {{0, 0, 0, 1, 1, 0, 0, 0},\
	                 {1, 1, 1, 0, 1, 1, 0, 0},\
	                 {1, 1, 1, 1, 1, 0, 0, 0},\
	                 {0, 1, 1, 1, 0, 0, 0, 0},\
	                 {0, 0, 0, 0, 0, 0, 0, 0},\
	                 {0, 0, 0, 0, 0, 0, 0, 0},\
	                 {1, 1, 1, 1, 1, 1, 1, 1},\
	                 {0, 0, 0, 0, 0, 0, 0, 1},\
	                 {0, 0, 0, 0, 0, 0, 0, 1},\
	                 {0, 0, 0, 0, 0, 0, 1, 0},\
	                 {0, 0, 0, 0, 0, 0, 0, 0},\
	                 {0, 0, 0, 0, 0, 0, 1, 0},\
	                 {0, 0, 0, 0, 0, 0, 0, 1},\
	                 {0, 0, 0, 0, 0, 0, 0, 1},\
	                 {1, 1, 1, 1, 1, 1, 1, 1},\
	                 {0, 0, 0, 0, 0, 0, 0, 0},\
	                 {0, 0, 0, 0, 0, 0, 0, 0},\
	                 {0, 1, 1, 1, 0, 0, 0, 0},\
	                 {1, 1, 1, 1, 1, 0, 0, 0},\
	                 {1, 1, 1, 0, 1, 1, 0, 0},\
	                 {0, 0, 0, 1, 1, 0, 0, 0}};
	flying_machine = {{0, 0, 0, 0, 0, 1, 1, 1, 1},\
	                  {0, 0, 0, 0, 1, 0, 0, 0, 1},\
	                  {0, 0, 0, 0, 0, 0, 0, 0, 1},\
	                  {1, 1, 0, 0, 1, 0, 0, 1, 0},\
	                  {1, 1, 1, 0, 0, 0, 0, 0, 0},\
	                  {1, 1, 0, 0, 1, 0, 0, 1, 0},\
	                  {0, 0, 0, 0, 0, 0, 0, 0, 1},\
	                  {0, 0, 0, 0, 1, 0, 0, 0, 1},\
	                  {0, 0, 0, 0, 0, 1, 1, 1, 1}};
	flying_machine2 = {{0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
	                   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
	                   {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
	                   {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
		                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},\
	                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},\
	                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},\
	                   {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0},\
	                   {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},\
	                   {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0},\
	                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},\
	                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},\
	                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},\
	                   {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
	                   {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
	                   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},\
	                   {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	block_laying_switch_engine = {{0, 0, 0, 0, 0, 0, 1, 0},\
	                              {0, 0, 0, 0, 1, 0, 1, 1},\
	                              {0, 0, 0, 0, 1, 0, 1, 0},\
	                              {0, 0, 0, 0, 1, 0, 0, 0},\
	                              {0, 0, 1, 0, 0, 0, 0, 0},\
	                              {1, 0, 1, 0, 0, 0, 0, 0}};
	honeycomb = {{0, 1, 0},\
	             {1, 0, 1},\
	             {1, 0, 1},\
	             {1, 1, 1}};
}

void LifeGame::get_size(int hh, int ww)
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

bool LifeGame::isNumber(const string& s)
{
	for (char c: s)
	{
		if (c-'0' < 0 || 9 < c-'0') return false;
	}
	return true;
}

void LifeGame::init()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	vector<vector<bool>> tmp(H, vector<bool>(W, false));
	now = tmp;
	next = tmp;
	alive = 0;
	dead = H * W;
}

void LifeGame::set_random()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	init();
	random_device rnd;
	if (SEED == 0) seed = rnd();
	else seed = SEED;
	ofstream ofs(SEEDFILE, ios::app);
	Time T;
	ofs << seed << " " << "H" << H << "W" << W << " " << T.getDatetimeStr() << endl;
	mt19937 mt(seed);
	int rate_live = RATE_LIVE * 100;
	for (int y = 0; y < H; ++y)
	{
		for (int x = 0; x < W; ++x)
		{
			bool is_alive;
			is_alive = ((mt() % 100) < rate_live);
			now[y][x] = is_alive;
		}
	}
}

void LifeGame::set_any(const vector<vector<bool>>& target, const pair<int, int>& base)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	int h, w, x_idx, y_idx, x_base, y_base;
	h = target.size();
	w = target[0].size();
	x_idx = 0;
	y_idx = 0;
	x_base = base.first;
	y_base = base.second;
	if (H < h || W < w)
	{
		cerr << __PRETTY_FUNCTION__ << " : Out of size" << endl;
		exit(-1);
	}
	if (x_base < 0 || W <= x_base || y_base < 0 || W <= y_base)
	{
		cerr << __PRETTY_FUNCTION__ << " : Out of size" << endl;
		exit(-1);
	}
	for (int y = y_base; y < y_base+h; ++y)
	{
		for (int x = x_base; x < x_base+w; ++x)
		{
			now[y][x] = target[y_idx][x_idx];
			++x_idx;
		}
		++y_idx;
		x_idx = 0;
	}
}

pair<int, int> LifeGame::get_center(const vector<vector<bool>>& target)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	int h, w, diff_w, diff_h, x_base, y_base;
	h = target.size();
	w = target[0].size();
	diff_h = H - h;
	diff_w = W - w;
	if (diff_h < 0 || diff_w < 0)
	{
		cerr << __PRETTY_FUNCTION__ << " : Out of size" << endl;
		exit(-1);
	}
	x_base = diff_w / 2;
	y_base = diff_h / 2;
	return {x_base, y_base};
}

void LifeGame::set_center(vector<vector<bool>>& target)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	pair<int, int> center = get_center(target);
	set_any(target, center);
}

int LifeGame::count_surrounding(const int& x, const int& y)
{
	// if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	int ct = 0;
	for (int i = 0; i < 8; ++i)
	{
		int xx, yy;
		xx = x + stepx[i];
		yy = y + stepy[i];
		if (!check_range(xx, yy)) continue;
		ct += now[yy][xx];
	}
	return ct;
}

bool LifeGame::check_range(const int& x, const int& y)
{
	if (x < 0 || W <= x) return false;
	if (y < 0 || H <= y) return false;
	return true;
}

bool LifeGame::check_cell(const int& x, const int& y)
{
	// if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	int ct = count_surrounding(x, y);
	if (now[y][x])
	{
		if (ct <= 1) return false;
		else if (ct == 2 || ct == 3) return true;
		else if (ct >= 4) return false;
	}
	else if (ct == 3) return true;
	else return false;
}

void LifeGame::check_cells()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	for (int y = 0; y < H; ++y)
	{
		for (int x = 0; x < W; ++x)
		{
			next[y][x] = check_cell(x, y);
		}
	}
}

int LifeGame::count_now()
{
	alive = 0;
	dead = 0;
	for (int y = 0; y < H; ++y)
	{
		for (int x = 0; x < W; ++x)
		{
			if (now[y][x]) ++alive;
			else ++dead;
		}
	}
	return alive;
}

bool LifeGame::comp_nownext()
{
	for (int y = 0; y < H; ++y)
	{
		for (int x = 0; x < W; ++x)
		{
			if (now[y][x] != next[y][x]) return true;
		}
	}
	return false;
}

void LifeGame::print_now()
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	vector<string> S(H);
	for (int y = 0; y < min(H, HMAX); ++y)
	{
		string s = "";
		for (int x = 0; x < min(W, WMAX); ++x)
		{
			if (ZENKAKU)
			{
				if (now[y][x]) s += ALIVE;
				else s += DEAD;
			}
			else
			{
				if (now[y][x]) s += Alive;
				else s += Dead;
			}
		}
		S[y] = s;
	}
	if (DYNAMIC)
	{
		if (C_flag)
		{
			C.EraseDisplayEntire();
			C.Puts(S);
			C_flag = false;
		}
		else C.RewriteDisplay(S);
	}
	else
	{
		for (string s: S) cout << s << endl;
	}
}

void LifeGame::show(int step)
{
	if (DEBUG) cout << "--" << __PRETTY_FUNCTION__ << " Called" << endl;
	C_flag = true;
	if (step == 0) step = STEP;
	print_now();
	if (DYNAMIC) Sleep(SLEEP);
	vector<vector<bool>> ini;
	ini = now;
	for (int t = 0; t < step; ++t)
	{
		if (count_now() == 0)
		{
			if (ROOP)
			{
				now = ini;
				if (DYNAMIC) Sleep(SLEEP_END);
				continue;
			}
			else break;
		}
		check_cells();
		if (!comp_nownext())
		{
			if (ROOP)
			{
				now = ini;
				if (DYNAMIC) Sleep(SLEEP_END);
				continue;
			}
			else break;
		}
		now = next;
		print_now();
		if (DYNAMIC) Sleep(SLEEP);
	}
}

int main()
{
	LifeGame L;
	L.get_size();
	L.init();
//	L.set_random();
	//L.set_center(L.honeycomb);
	//L.set_any(L.glider_gun, {0, 0});	// H >= 11
	//L.set_any(L.puffer_train, {0, 0});	// H >= 19
	L.set_any(L.puffer_train2, {0, 2}); // H >= 25
	//L.set_any(L.flying_machine, {4, 1});	// H >= 11
	//L.set_any(L.flying_machine2, {0, 1}); // H >= 19
	//L.set_any(L.block_laying_switch_engine, {10, 20});
	L.show();

	return 0;
}
