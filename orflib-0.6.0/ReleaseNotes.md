ORFLIB Release Notes
====================

VERSION 0.6.0
-------------

### Additions

1. New folder `orflib/math/random`  
	It will contain files for random number generation.
  
2. New file `orflib/math/random/normalrng.hpp`  
	It defines the NormalRng class template that generates random normal deviates.
  
3. New file `orflib/math/random/rng.hpp`  
	It contains typedefs for common combinations of distributions and underlying generators.  
	This file is to be included by any other file that requires random samples.
  
4. New folder `orflib/methods/montecarlo`  
	It will contain files specific to MC path generation.
  
5. New files `orflib/methods/montecarlo/pathgenerator.hpp` and `eulerpathgenerator.hpp`  
	The first file defines the abstract base PathGenerator and the second file defines a class template 
	that generates paths using the Euler method.
  
6. New file `orflib/methods/montecarlo/mcparams.hpp`  
	It defines a struct to be used for collecting MC parameters.

7. New folder `orflib/products`  
	It will contain files specific to product payoffs.

8. New files `orflib/products/product.hpp` and `orflib/products/europeancallput.hpp`  
	The abstact product base class and the European option product class.

9. New file `orflib/math/stats/statisticscalculator.hpp`  
	It defines the base class template for classes that compute statistics of samples.
  
10. New file `orflib/math/stats/meanvarcalculator.hpp`  
	It defines the mean-variance calculator.

11. New files  `orflib/pricers/bsmcpricer.hpp` and `bsmcpricer.cpp`  
	They define the Black-Scholes Monte Carlo pricer class.
    
12. New Python file `pyqflib/pyfunctions3.cpp`  
	It implements the Python callable C++ function pyOrfEuroBSMC.

13. New Python callable function orf.euroBSMC.

14. Added solutions to homeworks 1-5.


VERSION 0.5.0
-------------

### Additions

1. New utility functions toContCmpd and fromContCpd in file `orflib/utils.hpp`  
   They convert an interest rate to/from  continuous compounding from/to compounding with a given annual frequency.

2. New folder `orflib/math/optim`  
  It contains root finding and optimization routines.
	
3. New file `orflib/math/optim/roots.hpp`  
  It contains the root finding routines zbrak and rtsec from NR C++ 3ed.

4. New file `orflib/math/optim/polyfunc.hpp`  
  It defines a class that models a polynomial.

5. New Python callable functions orf.polyBracket, orf.polySecant, orf.toContCmpd, orf.fromContCmpd


VERSION 0.4.0
-------------

### Additions

1. New file `orflib/sptr.hpp`
	Type definions for shared pointers.

2. New file `orflib/sptrmap.hpp`
	It defines the SPtrMap class template.

3. New folder `orflib/market` with two new files `yieldcurve.hpp` and `yieldcurve.cpp`.  
	They define and implement the YieldCurve class.

4. Two new files `orflib/market/market.hpp` and `market.cpp`  
	They define and implement the Market singleton class.

5. New file `pyorflib/pyfunctions2.cpp` and additions to `pyorflib/module.cpp`
    and `pyorflib/qflib/__init__.py`. 
    They implement and register the following Python callable functions  
	orf.mktList,  orf.mktClear, orf.ycCreate, orf.discount, orf.fwdDiscount, orf.spotRate, orf.fwdRate  


VERSION 0.3.0
-------------

### Additions

1. New file `orflib/math/matrix.hpp`.   
  It defines aliases for the Vector and Matrix classes to be used in numerical work.

2. New folder `orflib/math/interpol` with two new files `piecewisepolynomial.hpp` and `piecewisepolynomial.cpp`.  
	They implement the class PiecewisePolynomial.

3. New file `pyorflib/pyutils.hpp`.  
	It implements the utility functions asVector(), asMatrix(), and asNumpy().

4. In file `pyorflib/pyfunctions0.hpp` added functions:  
    pyOrfEchoMatrix(), pyOrfPPolyEval(), pyOrfPPolyIntegral().

5. In file `pyorflib/orflib/__init__.py` added Python callable functions:   
	orf.echoMatrix(), orf.ppolyEval(), orf.ppolyIntegral()

### Modifications

1. Orflib now has dependency on the armadillo C++ matrix library. 



VERSION 0.2.0
-------------

### Additions

1. New file `orflib/exception.hpp`.  
	Definition of orf::Exception class and the ORF_ASSERT macro.

2. New folder `orflib/math/stats` to contain statistics related source files.

3. New files `orflib/math/stats/errorfunction.hpp` and `errorfunction.cpp`.  
	Definition of class ErrorFunction.

4. New files `orflib/math/stats/univariatedistribution.hpp` and `normaldistribution.hpp`.  
	Definition of `UnivariateDistribution` and `NormalDistribution` classes.

5. New folder `orflib/math/pricers` to contain pricing related source files.

6. New files `orflib/math/pricers/simplepricers.hpp` and `simplepricers.cpp`.  
	Definition of the functions fwdPrice(), digitalOptionBS() and europeanOptionBS().

7. In file `pyorflib/pyfunctions0.hpp` added functions:  
    pyOrfErf(), pyOrfInvErf(), pyOrfNormalCdf(), pyOrfNormalInvCdf()
 
8. New file `pyorflib/pyfunctions1.hpp`.  
	Added definition of the following functions:  
	pyOrfFwdPrice(), pyOrfDigiBS() and pyOrfEuroBS().

9. In file `pyorflib/orflib/__init__.py` added Python callable functions:   
	orf.normalCdf(), orf.normalInvCdf(), orf.fwdPrice(), orf.digiBS(), orf.euroBS().


### Modifications

1. Removed unnecessary file `orflib/empty.cpp`


VERSION 0.1.0
-------------

### Additions

1. Top level CMakeLists.txt file with projet-wide settings

2. New folder `orflib` with CMakeLists.txt and start-up code for the core library.

3. New folder `pyorflib` with CMakeLists.txt and start-up code for the Python interface.

4. This release notes file.

5. `.gitignore` file for managing which files to keep under revision control.
