# orflib library examples

#%%
import orflib as orf
import numpy as np
import os

ver = orf.version()
print(f'orflib version: {ver}')
pid = os.getpid()
print(f'pid: {pid}')

name = "World"
print(orf.sayHello(name))

x = [1, 2, 3]
y = [4, 5]
op = orf.outerProd(x, y)
print(f'x: {x}\ny: {y}')
print(f'outerProd:\n{op}')
