import heapq
import math
import sys


def D2NC(D: int, N: int, link=False) -> list:
    """
    10進数表記の数字をN進数表記に変換する。
    (関数名は"Decimal to N_ary Converter"の略。)

    Parameters
    ----------
    D : 10進数表記で与えられた数字。
    N : 変換したい底。
    link : Trueにすると結果を文字列結合して返す。

    Returns
    -------
    res : 変換結果。デフォルトではリスト化して返す。
    """
    if N < 2 or int(N) != N:
        print("Nは2以上の整数を入力して下さい")
        sys.exit()
    alp = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    res = []
    while D > 0:
        if D % N > 9:
            tmp = alp[D % N - 10]
            res.insert(0, str(tmp))
        else:
            res.insert(0, str(D % N))
        D //= N
    if not res:
        res.append('0')
    if link:
        res = ''.join(res)
    return res


def N2DC(V: str, N: int) -> int:
    """
    N進数表記の数字を10進数表記に変換する。11進数以上の場合の英文字は小文字大文字どちらでもいい。

    Parameters
    ----------
    V : N進数表記で与えられた数字。整数型を渡すことも可能。
    N : Vの底。

    Returns
    -------
    res : 変換結果を返す。
    """
    if type(V) == int:
        V = str(V)
    alp = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    exp = len(V) - 1
    res = 0
    for i in V:
        i = i.upper()
        if i not in alp[:N-10]:
            res += int(i) * (N**exp)
        else:
            res += (alp[:N-10].index(i)+10) * (N**exp)
        exp -= 1
    return res


def M2NC(V: str, M: int, N: int) -> str:
    """
    M進数表記の数字をN進数表記に変換する。。

    Parameters
    ----------
    V : N進数表記で与えられた数字。
    M : Vの底。
    N : 変換したい底。

    Returns
    -------
    変換結果を返す。
    """
    if N < 2 or int(N) != N:
        print("Nは2以上の整数を入力して下さい")
        sys.exit()
    if M == N:
        return V
    if M == 10:
        return D2NC(int(V), N, True)
    if N == 10:
        return str(N2DC(V, M))
    return D2NC(N2DC(V, M), N, True)


def prime_factorize(N: int, list=False, draw=False) -> list:
    """
    素因数分解をする。

    Parameters
    ----------
    N : 素因数分解したい数。
    list : Trueにすれば計算結果の各要素をリストにした配列を返す。
    draw : Trueにすれば計算結果を"a^b x c^d x ..."の形式にして返す。

    Returns
    -------
    res : 計算結果。(素因数, 冪指数)の形のタプルをリスト化して返す。
    """
    res = []
    pn = 2
    while N != 1:
        exp = 0
        while N % pn == 0:
            N /= pn
            exp += 1
        if exp:
            if list:
                res.append([pn, exp])
            else:
                res.append((pn, exp))
        pn += 1
    if draw:
        tmp = []
        for i in res:
            a, b = i
            tmp.append('{}^{}'.format(a, b))
        res = ' x '.join(tmp)
    return res


def p_f_f(N: int) -> list:
    """
    N!を素因数分解する。

    Parameters
    ----------
    N : 階乗を素因数分解したい数。

    Returns
    -------
    pn : 計算結果。[素因数, 冪指数]の形のリストをリスト化して返す。
    """
    pn = []
    for i in range(2, N+1):
        for j in prime_factorize(i, list=True):
            flg = 0
            if pn:
                idx = 0
                for k in pn:
                    if k[0] == j[0]:
                        pn[idx][1] += j[1]
                        flg = 1
                        break
                    idx += 1
            if not flg:
                pn.append(j)
    return pn


def gcd(a: int, b: int) -> int:
    """
    ユークリッドの互除法を用いて二数の最大公約数を求める。
    (関数名は"greatest common divisor"の略。)

    Parameters
    ----------
    a, b : 最大公約数を求めたい数。

    Returns
    -------
    b : 最大公約数
    """
    while a % b:
        r = a % b
        a = b
        b = r
    return b


def lcm(a: int, b: int) -> int:
    """
    gcd関数を用いて二数の最小公倍数を求める。
    (関数名は"least common multiple"の略。)

    Parameters
    ----------
    a, b : 最小公倍数を求めたい数。

    Returns
    -------
    最小公倍数を返す。
    """
    return int(a * b / gcd(a, b))


def calc(a: int, b: int, p: int) -> int:
    """
    二分累乗法を用いてaのb乗をpで割った余りを求める。
    組み込み関数のpow関数を使ったほうがいい。

    Parameters
    ----------
    a : 正整数
    b : 冪
    p : mod

    Returns
    -------
    aのb乗をpで割った余りを返す。
    """
    if b == 0:
        return 1
    elif b % 2:
        return (a * calc(a, b-1, p)) % p
    else:
        d = calc(a, b/2, p)
        return d * d % p


def factorial_1(N: int, mod: int) -> int:
    """
    再帰関数を用いて階乗計算を行い、任意の数で割った余りを求める。再帰回数が多すぎる場合はエラーが出る。

    Parameters
    ----------
    N : 階乗したい数。
    mod : 割る数。

    Returns
    -------
    余りを返す。
    """
    if N == 1:
        return 1
    return (N * factorial_1(N-1, mod)) % mod


def factorial_2(N: int, mod: int) -> int:
    """
    for文を用いて階乗計算を行い、任意の数で割った余りを求める。再帰関数型よりも求められる数の範囲が広い。実行速度も若干速い？

    Parameters
    ----------
    N : 階乗したい数。
    mod : 割る数。

    Returns
    -------
    余りを返す。
    """
    res = 1
    for i in range(1, N+1):
        res *= i
        res %= mod
    return res


def nCr_mod(n: int, r: int, mod: int) -> int:
    """
    factorial_2関数を用いて階乗計算を行い、あるmodにおいてフェルマーの小定理が成り立つとき逆元を求められることから、nCrをmodで割った余りを求める。

    Parameters
    ----------
    n : 総数。
    r : 選ぶ数。

    Returns
    -------
    余りを返す。
    """
    if n == r:
        return 1
    if r > n:
        return 0
    return (factorial_2(n, mod) * pow((factorial_2(n-r, mod) * factorial_2(r, mod)), mod-2, mod)) % mod


def nCr(n: int, r: int):
    """
    math.factorial関数を用いて階乗計算を行い、nCrを求める。
    modを求めたいときでもこの関数を使ったほうが速かったりする？

    Parameters
    ----------
    n : 総数。
    r : 選ぶ数。

    Returns
    -------
    nCr
    """
    if n == r:
        return 1
    if r > n:
        return 0
    return math.factorial(n) // (math.factorial(n-r) * math.factorial(r))


def xor_1(n: int) -> int:
    """
    0~nまでの排他的論理和(XOR)を計算する。

    Parameters
    ----------
    n : XORを求めたい数字。

    Returns
    -------
    res : 計算結果。10進数表記で返す。

    <note>
    # 00000   0桁目は01が周期的に現れてその周期は2
    # 00001   1桁目は0011が周期的に現れてその周期は4
    # 00010   2桁目は00001111が周期的に現れてその周期は8
    # 00011   --> i桁目の周期は2^(i+1)
    # 00100   --> 1周期中に現れる1の数は"(周期)/2"
    # 00101   --> 0~nまでのうちi桁目に現れる周期の数は"(n+1)//(i桁目の周期)"
    # 00110   --> 0~nまでのうちi桁目に現れる1の数は
    # 00111       (i桁目の周期の数)*(i桁目の1周期中の1の数)
    # 01000       + (i桁目の全部分から周期が完全に繰り返された部分を引いた数)
    # 01001       - (その部分にある0の個数)
    # 01010       例えば、1桁目を見て
    # 01011                    00110011001
    # 01100       の場合は、2周期繰り返された後001が残る。1周期中の1の数は2個で
    # 01101       2周期分の合計は4。さらに残りの部分は001の3個で、そのうち0の数
    # 01110       は2個なので、残りの部分にある1の個数は3-2=1。したがって1の総数は
    # 01111       4+1=5。1周期中に含まれる0の個数は"(周期)/2"で求まるので、
    # 10000       残りの部分の数からこれを引けば、残りの部分にある1の個数が求まる。
    # 10001       マイナスになる場合は、0を出力すればいい。(max関数を使った。)
    """
    if n == 0:
        return 0
    else:
        digit = 1
        tmp = 1
        while True:  # nが2進数で何桁あるかを求める。
            if tmp <= n < 2*tmp:  # 2^(x-1) <= n < 2^x ならnは2進数でx桁
                break
            tmp *= 2
            digit += 1

        res = 0
        period = 1
        for i in range(digit):  # 0桁目から順に1の数を数える。
            period *= 2  # i桁目の000…0111…1の周期は2^(i+1)
            count_1 = (n+1)//period * period//2
            count_1 += max(0, (n+1) % period - period//2)

            if count_1 % 2:  # 1の数が奇数なら足す。偶数の場合XORは0。
                res += 2**i

    return res


def xor_2(n: int) -> int:
    """
    0~nまでの排他的論理和(XOR)を計算する。
    nを偶数としたとき、nとn+1のXORが1になることを利用して、より高速に計算できる。

    Parameters
    ----------
    n : 排他的論理和を求めたい数字。

    Returns
    -------
    res : 計算結果。10進数表記で返す。
    """
    if n % 2:
        count_1 = (n+1)//2
        if count_1 % 2:
            a = 1
        else:
            a = 0
        return a
    else:
        count_1 = n//2
        if count_1 % 2:
            a = 1
        else:
            a = 0
        return a ^ n


def drawing_map(map):
    '''
    マップを描画する。
    map : マップ(二次元配列も可)
    '''
    if isinstance(map[0], (list, tuple)):
        map = [''.join(i) for i in map]
    for i in map:
        print(i)


def clear_maze_breadth(maze, start, goal, wall='#', symbol_s='S', symbol_g='G', path='*', drawing=False):
    '''
    幅優先探索を使い迷路のゴールまでの最短距離を求める。迷路が解けない場合は-1を返す。また二個目の要素として処理回数を返す。
    マップの情報をディクショナリにしない分、clear_maze_1に比べて効率が良くなった。引数として地形情報のみをリスト化したマップを渡せばよい。

    Parameters
    ----------
    maze : 迷路のマップ(二次元配列も可)
    start : 入力される迷路のスタート地点の記号(str)もしくは座標(tuple or list)
    goal : 入力される迷路のゴール地点の記号(str)もしくは座標(tuple or list)
    wall : 迷路の壁を表す記号
    symbol_s : 出力した迷路のスタート位置を表す記号
    symbol_g : 出力した迷路のゴール位置を表す記号
    path : 出力した迷路の通った道のりを表す記号
    drawing: Trueにすれば道のりを描画

    Returns
    -------
    ans : 最短経路数。解けない場合は-1を返す。
    ct : 処理回数
    '''
    ct = 0

    # 二次元配列の場合は文字列に変換
    if isinstance(maze[0], (list, tuple)):
        maze = [''.join(i) for i in maze]

    WALL = wall

    H = len(maze)  # 迷路の高さを取得
    if ' ' in maze[0]:  # 's . # g'のように文字列中に空白がある場合は削除
        for i in range(H):
            maze[i] = maze[i].replace(' ', '')
    W = len(maze[0])  # 迷路の幅を取得

    # 入力された迷路にスタートとゴールが示されていて、その記号が引数として与えられた場合スタート・ゴール位置の座標を取得
    if type(start) == str:
        for i in range(H):
            for j in range(W):
                if maze[i][j] == start:
                    s_y, s_x = i, j
                if maze[i][j] == goal:
                    g_y, g_x = i, j
    else:
        s_y, s_x = start
        g_y, g_x = goal

    # 探索されたらその時点までの経路数に更新して、探索済みかどうかを判定する。
    distance = [[False]*W for i in range(H)]

    if drawing is True:  # 通った道筋を記録
        root = [[False]*W for i in range(H)]

    queue = []
    queue.append((s_y, s_x))
    distance[s_y][s_x] = 0

    flg = False  # ゴールにたどり着いたらTrueに更新する。
    while queue:
        y, x = queue.pop(0)

        for i in range(4):
            ct += 1
            yy, xx = y+[0, 1, 0, -1][i], x+[1, 0, -1, 0][i]
            if 0 <= yy < H and 0 <= xx < W and maze[yy][xx] != WALL and distance[yy][xx] is False:
                distance[yy][xx] = distance[y][x] + 1
                queue.append((yy, xx))
                if drawing is True:
                    root[yy][xx] = (y, x)
                if yy == g_y and xx == g_x:
                    flg = True
                    break
        if flg:
            break

    if flg is False:
        ans = -1
    else:
        ans = distance[g_y][g_x]
        if drawing is True:  # rootに記録された座標をゴールから逆に辿る
            maze = [list(i) for i in maze]
            maze[s_y][s_x] = symbol_s
            maze[g_y][g_x] = symbol_g
            y, x = root[g_y][g_x]
            for i in range(ans-1):
                maze[y][x] = path
                y, x = root[y][x]
            drawing_map(maze)

    return ans, ct


def clear_maze_depth(maze, start, goal, wall='#', symbol_s='S', symbol_g='G', path='*', drawing=False):
    '''
    深さ優先探索を使い迷路を解きゴールまでの距離を求める。迷路が解けない場合は-1を返す。幅優先探索とは違い最短経路ではないことがある。また二個目の要素として処理回数を返す。

    Parameters
    ----------
    maze : 迷路のマップ(二次元配列も可)
    start : 入力される迷路のスタート地点の記号(str)もしくは座標(tuple or list)
    goal : 入力される迷路のゴール地点の記号(str)もしくは座標(tuple or list)
    wall : 迷路の壁を表す記号
    symbol_s : 出力した迷路のスタート位置を表す記号
    symbol_g : 出力した迷路のゴール位置を表す記号
    path : 出力した迷路の通った道のりを表す記号
    drawing: Trueにすれば道のりを描画

    Returns
    -------
    ans : 経路数(最短は保証しない)。解けない場合は-1を返す。
    ct : 処理回数
    '''
    ct = 0

    # 二次元配列の場合は文字列に変換
    if isinstance(maze[0], (list, tuple)):
        maze = [''.join(i) for i in maze]

    WALL = wall

    H = len(maze)  # 迷路の高さを取得
    if ' ' in maze[0]:  # 's . # g'のように文字列中に空白がある場合は削除
        for i in range(H):
            maze[i] = maze[i].replace(' ', '')
    W = len(maze[0])  # 迷路の幅を取得

    # 入力された迷路にスタートとゴールが示されていて、その記号が引数として与えられた場合スタート・ゴール位置の座標を取得
    if type(start) == str:
        for i in range(H):
            for j in range(W):
                if maze[i][j] == start:
                    s_y, s_x = i, j
                if maze[i][j] == goal:
                    g_y, g_x = i, j
    else:
        s_y, s_x = start
        g_y, g_x = goal

    # 探索されたらその時点までの経路数に更新して、探索済みかどうかを判定する。
    distance = [[False]*W for i in range(H)]

    if drawing is True:  # 通った道筋を記録
        root = [[False]*W for i in range(H)]

    stack = []
    stack.append((s_y, s_x))
    distance[s_y][s_x] = 0

    flg = False  # ゴールにたどり着いたらTrueに更新する。
    while stack:
        y, x = stack.pop(-1)

        for i in range(4):
            ct += 1
            yy, xx = y+[0, 1, 0, -1][i], x+[1, 0, -1, 0][i]
            if 0 <= yy < H and 0 <= xx < W and maze[yy][xx] != WALL and distance[yy][xx] is False:
                distance[yy][xx] = distance[y][x] + 1
                stack.append((yy, xx))
                if drawing is True:
                    root[yy][xx] = (y, x)
                if yy == g_y and xx == g_x:
                    flg = True
                    break
        if flg:
            break

    if flg is False:
        ans = -1
    else:
        ans = distance[g_y][g_x]
        if drawing is True:  # rootに記録された座標をゴールから逆に辿る
            maze = [list(i) for i in maze]
            maze[s_y][s_x] = symbol_s
            maze[g_y][g_x] = symbol_g
            y, x = root[g_y][g_x]
            for i in range(ans-1):
                maze[y][x] = path
                y, x = root[y][x]
            drawing_map(maze)

    return ans, ct


def bellman_ford(n: int, s: int, Graph):
    """
    ベルマンフォード法。任意の始点から各点への最短距離を求める。
    引数のグラフは 0-indexed の隣接リストで与える必要がある。
    負の重みが存在していても正しく動作する。
    すべての辺について重みを更新することを１ループとすると、一回のループで最低一つの頂点の最短距離が定まるので、ループ回数はn-1回。n回以上の更新がある場合は、負の閉路が存在することを意味する。

    Parameters
    ----------
    n : グラフの頂点数
    s : 始点
    Graph : グラフ。隣接リスト型。

    Returns
    -------
    始点から各点(indexに対応)までの最短距離をリスト化して返す。負の閉路が存在する場合は、-1も返す。
    """
    INF = float('inf')
    dis = [INF] * n
    dis[s] = 0
    for i in range(n):
        for j in range(n):
            for edge in Graph[j]:
                to, weight = edge
                if dis[j] != INF and dis[to] > dis[j] + weight:
                    dis[to] = dis[j] + weight
                    if i == n - 1:
                        return dis, -1
    return dis
    """
    入力例
    --------
    4 4
    0 1 2
    0 2 3
    0 3 100
    2 3 4
    --------
    n, m = map(int, input().split())
    graph = [[] for _ in range(n)]
    for _ in range(m):
        frm, to, weight = map(int, input().split())
        graph[frm].append((to, weight))

    出力例
    ------------
    [0, 2, 3, 7]
    ------------
    print(bellman_ford(n, 0, graph))
    """


def dijkstra(n: int, s: int, Graph, Root=False):
    """
    ダイクストラ法。任意の始点から各点への最短距離を求める。
    引数のグラフは 0-indexed の隣接リストで与える必要がある。
    プライオリティキューを使い、コストの小さい点から探索していく。
    ベルマンフォード法よりも効率がいいが、負の重みが存在する場合は使えない。
    heapqモジュールをインポートする必要がある。
    RootをTrueにすれば経路を記録した配列を得られるので、それをもとに道筋を書き起こせる。

    Parameters
    ----------
    n : グラフの頂点数
    s : 始点
    Graph : グラフ。隣接リスト型。
    Root : Trueにすれば各点の一つ前の点を記録する。

    Returns
    -------
    始点から各点(indexに対応)までの最短距離をリスト化して返す。RootがTrueの場合は経路を記録したリストも返す。
    """
    INF = float('inf')
    dis = [INF] * n
    dis[s] = 0
    hq = []
    heapq.heappush(hq, (0, s))   # (cost, now) 始点からのコストと今見ている点
    if Root is True:
        root = [-1] * n  # 最終的に-1の点は更新されていないことを示す
        root[s] = s

    while hq:
        c, n = heapq.heappop(hq)
        for i in Graph[n]:
            to, weight = i
            if dis[to] > c + weight:
                dis[to] = c + weight
                heapq.heappush(hq, (dis[to], to))
                if Root is True:
                    root[to] = n

    if Root is True:
        return dis, root
    else:
        return dis
    """
    入力例
    --------
    4 4
    0 1 2
    0 2 3
    0 3 100
    2 3 4
    --------
    n, m = map(int, input().split())
    graph = [[] for _ in range(n)]
    for _ in range(m):
        frm, to, weight = map(int, input().split())
        graph[frm].append((to, weight))

    出力例
    ------------
    ([0, 2, 3, 7], [0, 0, 0, 2])
    ------------
    print(bellman_ford(n, 0, graph, Root=True))
    """


def warshall_floyd(n: int, Graph):
    """
    ワ―シャルフロイド法。全点対間の最短距離を求める。
    引数のグラフは 0-indexed の隣接行列で与える必要がある。
    Graph[i][j] : 点iから点jへの辺の重み。辺が存在しない所はINFで初期化しておく。i=jのときは0。
    負の閉路が存在する場合は返り値のGraph[i][i]が負になる。

    Parameters
    ----------
    n : グラフの頂点数
    Graph : グラフ。隣接行列型。

    Returns
    -------
    Graph[i][j] : 点iから点jまでの最短距離
    隣接行列として与えられたグラフを上のように書き換えて返す。
    """
    for i in range(n):
        for j in range(n):
            for k in range(n):
                Graph[j][k] = min(Graph[j][k], Graph[j][i] + Graph[i][k])
    return Graph
    """
    入力例
    ---------
    4 6
    0 1 10
    0 3 100
    1 3 1000
    2 1 1
    2 3 10000
    3 0 5
    ---------
    n, m = map(int, input().split())
    INF = float('inf')
    graph = [[INF]*n for _ in range(n)]
    for i in range(n):
        graph[i][i] = 0
    for _ in range(m):
        frm, to, weight = map(int, input().split())
        graph[frm][to] = weight

    出力例
    -------------------
    0から1へのコスト : 10
    0から3へのコスト : 100
    1から0へのコスト : 1005
    1から3へのコスト : 1000
    2から0へのコスト : 1006
    2から1へのコスト : 1
    2から3へのコスト : 1001
    3から0へのコスト : 5
    3から1へのコスト : 15
    -------------------
    dis = warshall_floyd(n, graph)
    for i in range(n):
        for j in range(n):
            if dis[i][j] != INF and i != j:
                print("{}から{}へのコスト : {}".format(i, j, dis[i][j]))
    """


def topological_sort(G):
    """
    グラフをトポロジカルソートする。与えられたグラフがトポロジカルソートできない(DAGでない)場合は-1を返す。
    プライオリティキューを用いて、頂点番号の小さいものを優先的にソートする。
    heapqモジュールをインポートする必要がある。

    Parameters
    ----------
    G : ソートしたいグラフ。0-indexedの隣接リスト型。

    Returns
    -------
    右向きにソートしたグラフの頂点のリスト。グラフがDAGでない場合は-1を返す。
    """
    V = len(G)      # グラフの頂点数を取得
    indeg = [0]*V   # 各頂点の入次数を記録。indeg[i]は頂点iの入次数を表す。
    for u in G:
        for v in u:
            indeg[v] += 1
    res = []        # ソート結果を記録
    hq = [i for i in range(V) if indeg[i] == 0]  # 入次数0の頂点を格納
    while hq:  # 入次数0の頂点を除いたグラフの中で入次数0の点を探し、入次数0の点がなくなるまで繰り返す
        u = heapq.heappop(hq)   # 頂点番号の小さいものを優先
        res.append(u)
        for v in G[u]:
            indeg[v] -= 1       # uから辺が伸びていた先の頂点の入次数を1減らす
            if indeg[v] == 0:   # 入次数が0の点があればキューに入れる
                heapq.heappush(hq, v)
    if len(res) == V:
        return res
    else:
        return -1
    """
    ------
    V A
    u1 v1
    u2 v2
    .  .
    .  .
    .  .
    uA vA
    ------
    0-indexedのグラフ。
    V : 頂点数
    A : 辺数
    u : 始点
    v : 終点
    ==================
    <input>
    6 10
    0 1
    2 0
    2 4
    2 5
    3 0
    4 0
    4 1
    5 1
    5 3
    5 4
    ==================
    <output>
    [2, 5, 3, 4, 0, 1]
    ==================
    V, A = map(int, input().split())
    graph = [[] for _ in range(V)]
    for i in range(A):
        u, v = map(int, input().split())
        graph[u].append(v)
    print(topological_sort(graph))
    """


class cumulative_sum():
    """
    一次元累積和を計算して、指定された範囲内の総和を求める。
    """
    def __init__(self, a):
        """
        Parameters
        ----------
        a      : 累積和を求めたい数列
        self.N : aの長さ
        self.s : 累積和計算用テーブル
        """
        self.N = len(a)
        self.s = [0]*(self.N+1)
        for i in range(self.N):
            self.s[i+1] = self.s[i] + a[i]

    def calc_sum(self, l, r):
        """
        Parameters
        ----------
        l, r : 総和を求めたい範囲。1-indexedで[l, r]の範囲を示す。

        Returns
        -------
        [l, r]内の総和
        """
        return self.s[r] - self.s[l-1]
    """
    -------------
    N
    a1 a2 ... aN
    Q
    l1 r1
    l2 r2
    .  .
    .  .
    .  .
    lQ rQ
    -------------
    1-indexedの数列を想定している。
    l, r : [l, r]の範囲を表す。
    Q個のクエリで指定された範囲内の総和を求める。
    ===========
    <input>
    5
    1 3 2 5 4
    3
    2 4
    1 5
    1 3
    ===========
    <output>
    10
    15
    6
    ===========
    N = int(input())
    A = [int(i) for i in input().split()]
    Q = int(input())
    query = [0]*Q
    for i in range(Q):
        query[i] = map(int, input().split())
    cs = cumulative_sum(A)
    for i in query:
        l, r = i
        print(cs.calc_sum(l, r))
    """


class cumulative_sum_2d():
    """
    二次元累積和を計算して、指定された長方形領域内の総和を求める。
    """

    def __init__(self, a):
        """
        Parameters
        ----------
        a      : 累積和を求めたい二次元配列
        self.H : aの高さ
        self.W : aの幅
        self.s : 累積和計算用テーブル。self.s[y][x]は、1-indexedで[1, y]×[1, x]
                 領域(原点を含む高さy、幅xの長方形領域)内の総和を表す。
        """
        self.H = len(a)
        self.W = len(a[0])
        self.s = [[0]*(self.W+1) for _ in range(self.H+1)]
        for y in range(self.H):
            for x in range(self.W):
                self.s[y+1][x+1] = self.s[y][x+1] + \
                    self.s[y+1][x] - self.s[y][x] + a[y][x]

    def calc_sum(self, yh, yt, xh, xt):
        """
        Parameters
        ----------
        yh, yt : 総和を求めたい長方形領域の縦の範囲。1-indexedで[yh, yt]
                 の範囲を示す。
        xh, xt : 総和を求めたい長方形領域の横の範囲。1-indexedで[xh, xt]
                 の範囲を示す。

        Returns
        -------
        [yh, yt]×[xh, xt]領域内の総和
        """
        return self.s[yt][xt] - self.s[yt][xh-1] - self.s[yh-1][xt] + self.s[yh-1][xh-1]
    """
    ----------------
    H W
    a11 a12 ... a1W
     .   .  ...  .
     .   .  ...  .
     .   .  ...  .
    aH1 aH2 ... aHW
    Q
    yh1 yt1 xh1 xt1
     .   .   .   .
     .   .   .   .
     .   .   .   .
    yhQ ytQ xhQ xtQ
    ----------------
    1-indexedの二次元領域を想定している。
    yh yt xh xt : [yh, yt]×[xh, xt]の長方形領域を表す。
    Q個のクエリで指定された領域内の総和を求める。
    ===========
    <input>
    4 5
    1 8 7 3 2
    9 1 3 4 6
    3 5 8 1 4
    2 7 3 2 5
    3
    2 3 3 5
    1 2 2 3
    1 4 1 5
    ===========
    <output>
    26
    19
    84
    ===========
    H, W = map(int, input().split())
    A = [0]*H
    for y in range(H):
        A[y] = tuple(map(int, input().split()))
    Q = int(input())
    query = [0]*Q
    for i in range(Q):
        query[i] = map(int, input().split())
    cs2 = cumulative_sum_2d(A)
    for i in query:
        yh, yt, xh, xt = i
        print(cs2.calc_sum(yh, yt, xh, xt))
    """


class fibonacci():
    """
    メモ化再帰を用いて、フィボナッチ数列の第n項目までを計算する。
    a[1] = 1, a[2] = 1, a[n] = a[n-1] + a[n-2]
    """
    def __init__(self, n: int):
        self.memo = [0]*(n+1)

    def calc(self, m: int):
        if m == 1:
            return 1
        if m == 2:
            return 1
        if self.memo[m] != 0:
            return self.memo[m]
        self.memo[m] = self.calc(m-1) + self.calc(m-2)
        return self.memo[m]
    """
    サンプル
    ----------------------------
    n = int(input())
    fibonacci = fibonacci(n)
    for i in range(1, n+1):
        print(fibonacci.calc(i))
    """


class UnionFind():
    def __init__(self, n):
        # 親要素の番号を格納。根の場合は-(その集合のsize)。
        # ノード番号とインデックスを対応させるためにn+1にした。0は無視。
        self.parent = [-1 for _ in range(n+1)]

    # 根ノードを検索
    def find(self, x):
        # 根ならその番号を返す
        if self.parent[x] < 0:
            return x
        # 根でないなら親の要素で再検索
        else:
            # 走査していく過程で親を書き換える
            self.parent[x] = self.find(self.parent[x])
            return self.parent[x]

    # 同じ集合に属するかチェック(根が同じか)
    def same_check(self, x, y):
        return self.find(x) == self.find(y)

    # 自分のいる集合のsizeを返す
    def size(self, x):
        y = self.find(x)  # xの根ノードを検索
        return -self.parent[y]  # 根ノードの番号には-(その集合のsize)があるので-1をかけて返す

    # 併合
    def union(self, x, y):
        # x, yの根ノードを検索
        x_root = self.find(x)
        y_root = self.find(y)

        # 同じ集合に属している場合(根が同じ場合)はそのまま
        if x_root == y_root:
            return False
        # 違う集合の場合(根が違う場合)、x_rootのsizeの方が大きいとしてx_rootにy_rootをつなげる
        else:
            # 大きい方に小さい方をつなげたいので、大小が違うなら入れ替える
            if -self.parent[x_root] < -self.parent[y_root]:
                x_root, y_root = y_root, x_root

            # x_rootのsizeを増やす
            self.parent[x_root] += self.parent[y_root]
            # y_rootの親をx_rootに変更する
            self.parent[y_root] = x_root
            return True
