#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct RungeKutta
{
  /*
  ルンゲクッタ法による数値計算を行う.
  2変数, 3変数, 4変数の3通りについて計算できる.
  オブジェクトの宣言時に以下のように引数を変えることで使い分けられる.
    2変数(t, x)       -> RungeKutta RK(Fx, dt, xi, tf, ti=0.0);
    3変数(t, x, y)    -> RungeKutta RK(Fx, Fy, dt, xi, yi, tf, ti=0.0);
    4変数(t, x, y, z) -> RungeKutta RK(Fx, Fy, Fz, dt, xi, yi, zi, tf, ti=0.0);

  ----- Parameters -----
  t, x, y, z : 変数
  Fx, Fy, Fz : x, y, z の t についての微分式. 使用する変数の数によらず
               (t, x, y, z)を引数にして関数を宣言しておく必要がある
               double Fx(double t, double x, double y, double z)
  xi, yi, zi : x, y, z の初期値
  ti, tf     : t の初期値と最終値
  dt         : 刻み幅
  */

  // 変数
  double dt, x_ini, y_ini, z_ini, t_ini, t_fin, dim;
  double(*func[3])(double, double, double, double);

  // コンストラクタ
  RungeKutta(double(*fx)(double, double, double, double), const double& dt, const double& xi, const double& tf, const double& ti=0.0): dt(dt), x_ini(xi), t_fin(tf), t_ini(ti), dim(2)
  {
    func[0] = fx;
  }
  RungeKutta(double(*fx)(double, double, double, double), double(*fy)(double, double, double, double), const double& dt, const double& xi, const double& yi, const double& tf, const double& ti=0.0): dt(dt), x_ini(xi), y_ini(yi), t_fin(tf), t_ini(ti), dim(3)
  {
    func[0] = fx;
    func[1] = fy;
  }
  RungeKutta(double(*fx)(double, double, double, double), double(*fy)(double, double, double, double), double(*fz)(double, double, double, double), const double& dt, const double& xi, const double& yi, const double& zi, const double& tf, const double& ti=0.0): dt(dt), x_ini(xi), y_ini(yi), z_ini(zi), t_fin(tf), t_ini(ti), dim(4)
  {
    func[0] = fx;
    func[1] = fy;
    func[2] = fz;
  }

  // ルンゲクッタ法による数値計算を行い出力する
  void calc()
  {
    vector<vector<double>> k(4, vector<double>(3));
    double x, y, z, t, dx, dy, dz, step;
    x = x_ini;
    y = y_ini;
    z = z_ini;
    t = t_ini;
    step = (t_fin-t_ini+dt) / dt;

    output(t, x, y, z); // 初期値の出力

    for (int i = 1; i < step; ++i)
    {
      for (int j = 0; j < dim-1; ++j) k[0][j] = dt * func[j](t, x, y, z);
      for (int j = 0; j < dim-1; ++j) k[1][j] = dt * func[j](t+dt/2.0, x+k[0][0]/2.0, y+k[0][1]/2.0, z+k[0][2]/2.0);
      for (int j = 0; j < dim-1; ++j) k[2][j] = dt * func[j](t+dt/2.0, x+k[1][0]/2.0, y+k[1][1]/2.0, z+k[1][2]/2.0);
      for (int j = 0; j < dim-1; ++j) k[3][j] = dt * func[j](t+dt, x+k[2][0], y+k[2][1], z+k[2][2]);

      dx = (k[0][0] + 2*k[1][0] + 2*k[2][0] + k[3][0]) / 6.0;
      x += dx;
      if (dim >= 3)
      {
        dy = (k[0][1] + 2*k[1][1] + 2*k[2][1] + k[3][1]) / 6.0;
        y += dy;
      }
      if (dim == 4)
      {
        dz = (k[0][2] + 2*k[1][2] + 2*k[2][2] + k[3][2]) / 6.0;
        z += dz;
      }
      t += dt;

      output(t, x, y, z); // 出力
    }
  }

  // 値を出力する
  void output(const double& t, const double& x, const double& y, const double& z)
  {
    if (dim == 2) cout << t << " " << x << endl;
    else if (dim == 3) cout << t << " " << x << " " << y << endl;
    else if (dim == 4) cout << t << " " << x << " " << y << " " << z << endl;
  }
};

// ローレンツアトラクタ
double p = 10.0, r = 28.0, b = 8.0/3.0;

double Fx(double t, double x, double y, double z)
{
  return p * (y - x);
}

double Fy(double t, double x, double y, double z)
{
  return -x*z + r*x - y;
}

double Fz(double t, double x, double y, double z)
{
  return x*y - b*z;
}

int main()
{
  string buf;
  double dt, xi, yi, zi, tf;
  getline(cin, buf);
  cin >> dt >> xi >> yi >> zi >> tf;
  RungeKutta RK(Fx, Fy, Fz, dt, xi, yi, zi, tf);
  RK.calc();

  return 0;
}
/*
===== input =====
dt xi yi zi tf
0.01 10 10 10 50
*/