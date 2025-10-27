# orflib library examples

#%%
import orflib as orf
import numpy as np
import os

ver = orf.version()
print(f'orflib version: {ver}')
pid = os.getpid()
print(f'pid: {pid}')

# Greet
print('----------------')
name = "World"
print(orf.sayHello(name))

# Outer product
print('----------------')
x = [1, 2, 3]
y = [4, 5]
op = orf.outerProd(x, y)
print(f'x: {x}\ny: {y}')
print(f'outerProd:\n{op}')

# Matrix
print('----------------')
m = np.array([[1, 2, 3], [4, 5, 6]])
em = orf.echoMatrix(m)
print(f'orig matrix:\n{m}')
print(f'echo matrix:\n{em}')

#PPolyEval
print('----------------')
xbpt = np.arange(1, 6)
yval =  np.arange(-10, 15, 5)
pord = 1
xval = np.arange(0.5, 6.0, 0.5)
pval = orf.ppolyEval(xbpt, yval, pord, xval, 0)
pder = orf.ppolyEval(xbpt, yval, pord, xval, 1)
pint = orf.ppolyIntegral(xbpt, yval, pord, xval[0], xval)
print('Piecewise polynomial')
print(f'bkpts={xbpt}')
print(f'yvals={yval}')
print(f'pord={pord}')
print(f'xval={xval}')
print(f'pval={pval}')
print(f'pder={pder}')
print(f'pint={pint}')

#Root bracketing and secant method search
print('----------------')
pcoeffs = [0, 2, 1]
lolim = -4
uplim = 3
nsubs = 10

brkts = orf.polyBracket(pcoeffs, lolim, uplim, nsubs)
print(f"Roots of polynomial: {pcoeffs}")
print(f"Root brackets:\n {brkts}")

root = orf.polySecant(pcoeffs, brkts[0, 0], brkts[0, 1], 1e-12)
print(f"root: {root:.4f}")

#ToContCmpd, FromContComp
print('----------------')
anfreq = 1
inrate = 0.10
ccrate = orf.toContCmpd(inrate, anfreq)
outrate = orf.fromContCmpd(ccrate, anfreq)
print('To and from continuous compounding')
print(f'InRate={inrate:.4f}, CCRate={ccrate:.4f}, PerRate={outrate:.4f}')

#%%
# function group 1
print('=================')
print('Analytic prices')

fwdpx = orf.fwdPrice(spot = 100, timetoexp = 1.0, intrate = 0.02, divyield = 0.04)
print(f'FwdPrice={fwdpx:.4f}')

digi = orf.digiBS(payofftype = 1, spot = 100, timetoexp = 1.0, strike = 100,
                 intrate = 0.04, divyield = 0.02, volatility = 0.2)[0]
print('Digital call using Black-Scholes analytic solution')
print(f'Price={digi:.4f}')

euro = orf.euroBS(payofftype = 1, spot = 100, timetoexp = 1.0, strike = 100,
                intrate = 0.04, divyield = 0.02, volatility = 0.4)[0]
print('European call using Black-Scholes analytic solution')
print(f'Price={euro:.4f}')

#%%
# function group 2
print('================')
print('Market objects')

#yccreate
yc = orf.ycCreate(ycname = 'USD', 
                 tmats =  [1/12,  1/4,  1/2,   3/4,    1,     2,    3,     4,    5,      10],
                 vals = [0.01,   0.02, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.0575, 0.065],
                 valtype = 0)
print(f'Created yield curve: {yc}')

#discount, spotrate, fwdrate
df = orf.discount(ycname = yc, tmat = 2)
spotrate = orf.spotRate(ycname = yc, tmat = 2)
fwdrate = orf.fwdRate(ycname = yc, tmat1 = 1, tmat2 = 2)
print(f'DF={df:.4f}, SpotRate={spotrate:.4f} FwdRate={fwdrate:.4f}')

print('Market list')
print(orf.mktList())

#%%
# function group 3
print('=================')
print('European option using Black-Scholes Monte Carlo')

#eurobsmc
mcpars0 = {'URNGTYPE': 'MT19937', 'PATHGENTYPE': 'EULER'}
npaths0 = 1000000
euromc0 = orf.euroBSMC(payofftype = 1, strike = 100, timetoexp = 1.0, spot = 100,
                       discountcrv =  yc, divyield = 0.02, volatility = 0.4,
                       mcparams = mcpars0, npaths = npaths0)
print(f'URNGTYPE={mcpars0["URNGTYPE"]} PATHGENTYPE={mcpars0["PATHGENTYPE"]} NPATHS={npaths0}')
print(f'Price={euromc0['Mean']:0.4f}  StdErr={euromc0['StdErr']:0.4f}')