# -*- coding: utf-8 -*-
# 迷路をランダムに生成してBFSとDFSで解く

import time
import functions
import random

# 幅と奥行きの設定 (どちらも奇数(>=17)である必要がある)
height = 43
width = 37

maze = [[1 if 0 < i < width-1 else 0 for i in range(width)] if 0 < j < height-1 else [
    0 for i in range(width)] for j in range(height)]  # 通路:0 壁:1


def mazeprint(maze):  # 数字で表現された面を●や◯に変換する関数
    c_dict = {0: ".",  1: "#"}
    c_maze = [[c_dict[maze[j][i]]
               for i in range(width)] for j in range(height)]
    c_maze[s_y][s_x] = ' '
    c_maze[g_y][g_x] = ' '
    functions.drawing_map(c_maze)
    print()
    return c_maze


# 開始点を定める
wl = [[2*i+2, 2*j+2] for i in range(int((width-1)/2)-1)
      for j in range(int((height-1)/2)-1)]  # 未探索リスト
stt = wl.pop(random.randint(0, len(wl)-1))  # ランダムに開始点を定める
ps = [stt]  # 探索済みリスト

print("start Point: ", stt)
maze[stt[1]][stt[0]] = 0  # 壁(1)を通路(0)に書き換える
# mazeprint(maze)  # 迷路出力


# 探索する
dr = [[-1, 0], [1, 0], [0, -1], [0, 1]]  # 探索方向のリスト

while(len(wl) > 0):
    random.shuffle(dr)  # 探索方向順をランダムに
    for i in range(4):  # このfor文内ではある座標sttの周辺４方向に対する探索のみ行う
        nxtx = stt[0]+dr[i][0]*2  # 探索候補のx座標
        nxty = stt[1]+dr[i][1]*2  # 探索候補のy座標
        if maze[nxty][nxtx] == 1:  # 2マス先が通路でないとき
            wl.remove([nxtx, nxty])  # 未探索リストから削除
            ps.append([nxtx, nxty])  # 探索済みリストに追加
            maze[nxty][nxtx] = 0  # 2マス先を通路に変更
            maze[nxty-dr[i][1]][nxtx-dr[i][0]] = 0  # 1マス先を通路に変更
            break
            if i == 3:
                ps.remove([nxtx, nxty])
        stt = ps[random.randint(0, len(ps)-1)]

while True:  # スタート位置とゴール位置をランダムに設定
    s_y, s_x = random.randint(2, height-3), random.randint(2, width-3)
    g_y, g_x = random.randint(2, height-3), random.randint(2, width-3)
    if s_y == g_y and s_x == g_x:
        pass
    else:
        if abs(s_x-g_x) > width/1.5:  # 二点がある程度離れるように設定
            if abs(s_y-g_y) > width/2.5:
                break
            else:
                pass
        else:
            pass

# s_y, s_x = 2, 2
# g_y, g_x = height-3, width-3

# 完成した迷路を出力
maze = mazeprint(maze)

t_1 = time.time()

ans_b = functions.clear_maze_breadth(maze, (s_y, s_x), (g_y, g_x), wall="#",
                                     symbol_s=' ', symbol_g=' ', path='o', drawing=True)

t_2 = time.time()

time_b = t_2 - t_1

print()

t_3 = time.time()

ans_d = functions.clear_maze_depth(maze, (s_y, s_x), (g_y, g_x), wall="#",
                                   symbol_s=' ', symbol_g=' ', path='o', drawing=True)

t_4 = time.time()

time_d = t_4 - t_3

print('breadth : {}'.format(ans_b), end=' / ')
print('time = {}'.format(time_b))
print('depth : {}'.format(ans_d), end=' / ')
print('time = {}'.format(time_d))
