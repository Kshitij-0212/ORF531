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

#%%
# function group 1
print('=================')
print('Analytic prices')

fwdpx = orf.fwdPrice(spot = 100, timetoexp = 1.0, intrate = 0.02, divyield = 0.04)
print(f'FwdPrice={fwdpx:.4f}')

digi = orf.digiBS(payofftype = 1, spot = 100, timetoexp = 1.0, strike = 100,
                 intrate = 0.02, divyield = 0.04, volatility = 0.2)
print('Digital call using Black-Scholes analytic solution')
print(f'Price={digi:.4f}')

euro = orf.euroBS(payofftype = 1, spot = 100, timetoexp = 1.0, strike = 100,
                intrate = 0.02, divyield = 0.04, volatility = 0.4)
print('European call using Black-Scholes analytic solution')
print(f'Price={euro:.4f}')
