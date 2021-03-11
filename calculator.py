# -*- coding: utf-8 -*-

import math


def prime_factorize(n: int):
  res = {}
  if (n == 0 or n == 1):
    print(n, 'は素因数分解できません')
    exit(0)
  s = 2
  while(n != 1):
    flg = True
    while(n % s == 0 and n > 1):
      n //= s
      if flg:
        res[s] = 1
        flg = False
      else:
        res[s] += 1
    if s == 2:
      s += 1
    else:
      s += 2
  return res


def prime_check(n: int):
  if (n < 2):
    print(n, 'は素数ではありません')
  s = 2
  while(s**2 <= n):
    if (n % s == 0):
      print(n, 'は素数ではありません')
      print('最小素因数は', s)
      exit(0)
    if s == 2:
      s += 1
    else:
      s += 2
  print(n, 'は素数です')
  exit(0)


S = input()

if S[-1] == 'f':
  res = prime_factorize(int(S[:-1]))
  ct = 1
  for key, value in res.items():
    print(key, end='')
    if value > 1:
      print('^', end='')
      print(value, end='')
    if ct < len(res):
      print('*', end='')
    ct += 1
  print()
  exit(0)
elif S[-1] == 'p':
  prime_check(int(S[:-1]))

tmp = []
flg_n = True
flg_op = False
tmp_n = ''
tmp_op = ''
number = '1234567890'
for s in S:
  if s in number:
    if flg_n:
      tmp_n += s
    else:
      if flg_op:
        tmp.append(tmp_op)
        flg_op = False
      tmp_n = s
      flg_n = True
  elif s == '!':
    flg_n = False
    tmp.append(tmp_n)
    tmp += s
  else:
    if flg_op:
      tmp_op += s
    else:
      if flg_n:
        tmp.append(tmp_n)
        flg_n = False
      tmp_op = s
      flg_op = True
if flg_n:
  tmp.append(tmp_n)
elif flg_op:
  tmp.append(tmp_op)

# print(tmp)

length = len(tmp)
index = 0
while index < length:
  if tmp[index] == '^':
    tmp[index] = '**'
  elif tmp[index] == '!':
    tmp[index-1] = str(math.factorial(int(tmp[index-1])))
    tmp.pop(index)
    index -= 1
    length -= 1
  index += 1

# print(tmp)
# print(''.join(tmp))
print(eval(''.join(tmp)))
