import orflib.pyorflib

###################
# function group 0

def version(): 
    """orflib library version.

    Returns
    -------
    str
        version in the format Major.Minor.Revision
    """
    return pyorflib.version()


def sayHello(name): 
    """Says hello.

    Parameters
    ----------
    name : str
        caller name

    Returns
    -------
    str
        greeting
    """
    return pyorflib.sayHello(name)


def outerProd(vector1, vector2):
    """Computes the outer product of two numerical vectors.

    Parameters
    ----------
    vector1 : list(double) or 1D numpy array
        first vector
    vector2: list(double) or 1D numpy array
        second vector

    Returns
    -------
    2D numpy array
        outer product as matrix
    """
    return pyorflib.outerProd(vector1, vector2)


def erf(x):
    """Error function at `x`. 
    """
    return pyorflib.erf(x)


def invErf(x):
    """Inverse error function at `x`. 
    """
    return pyorflib.invErf(x)


def normalCdf(x):
    """Cumulative standard normal distribution at `x`. 
    """
    return pyorflib.normalCdf(x)


def normalInvCdf(x):
    """Inverse of the cumulative standard normal distribution at quantile `x`.
    """
    return pyorflib.normalInvCdf(x)


def echoMatrix(mat):
    """Echoes the input matrix.

    Parameters
    ----------
    mat : 2D numpy array
        input matrix

    Returns
    -------
    2D numpy array
        the input matrix `mat`
    """
    return pyorflib.echoMatrix(mat)


def ppolyEval(bkpoints, values, polyorder, xvec, derivorder): 
    """Values and derivatives of a piecewise polynomial function at a set of points.

    Parameters
    ----------
    bkpoints : list(double) or 1D numpy array
        breakpoints of the piecewise polynomial function
    values : list(double) or 1D numpy array
        corresponding values at each breakpoint
    polyorder : {0, 1}
        polynomial order of the segments (constant or linear)
    xvec : list(double) or 1D numpy array
        points at which values or derivatives are evaluated
    derivorder : int (>= 0)
        if 0, values are computed, else derivatives of `derivorder` order  

    Returns
    -------
    1D numpy array
        values or derivatives at each point in `xvec`
    
    Notes
    -----
    1. If `polyorder`==0, the value at the left breakpoint is used for the segment.
    2. At points outside the breakpoint span constant extrapolation is used.
    """
    return pyorflib.ppolyEval(bkpoints, values, polyorder, xvec, derivorder)


def ppolyIntegral(bkpoints, values, polyorder, xstart, xvec): 
    """Integral of a piecewise polynomial function from `xstart` to each point in `xvec`.

    Parameters
    ----------
    bkpoints : list(double) or 1D numpy array
        brekpoints of the piecewise polynomial function
    values : list(double) or 1D numpy array
        corresponding values at each breakpoint
    polyorder : {0, 1}
        polynomial order of the segments (constant or linear)
    xstart : double
        starting point of each interval
    xvec : list(double) or 1D numpy array
        end points one for each interval

    Returns
    -------
    1D numpy array
        values of the integral
    
    Notes
    -----
    1. If `polyorder`==0, the value at the left breakpoint is used for the segment.
    2. At points outside the breakpoint span constant extrapolation is used.
    """
    return pyorflib.ppolyIntegral(bkpoints, values, polyorder, xstart, xvec)

###################
# function group 1

def fwdPrice(spot, timetoexp, intrate, divyield):
    """Forward price of an asset.

    Parameters
    ----------
    spot : double
        asset spot price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.

    Returns
    -------
    double
        asset forward price
     """
    return pyorflib.fwdPrice(spot, timetoexp, intrate, divyield)


def digiBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility):
    """Price of a European digital option in the Black-Scholes model. 

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    spot : double
        asset spot price
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility

    Returns
    -------
    dictionary
        price of the option
    """
    return pyorflib.digiBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility)


def euroBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility):
    """Price and Greeks of a European option in the Black-Scholes model. 

    Parameters
    ----------
    payofftype : {1, -1}
        1 for call, -1 for put
    spot : double
        asset spot price
    strike : double
        strike price
    timetoexp : double
        time to expiration in years
    intrate : double
        interest rate, p.a. and c.c.
    divyield : double    
        asset dividend yield, p.a. and c.c.
    volatility : double
        asset return volatility

    Returns
    -------
        price of the option
    """
    return pyorflib.euroBS(payofftype, spot, strike, timetoexp, intrate, divyield, volatility)
