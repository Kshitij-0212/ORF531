/** 
@file  pyfunctions0.hpp
@brief Implementation of Python callable functions
*/
#include "pycpp.hpp"
#include <orflib/defines.hpp>
#include <string>

static 
PyObject*  pyOrfVersion(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  return asPyScalar(std::string(ORF_VERSION_STRING));
PY_END;
}

static 
PyObject* pyOrfSayHello(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyArg1))
    return NULL;
  std::string name = asString(pyArg1);        // read in the name
  std::string greet = "Hello " + name + "!";  // compose the greeting  
  return asPyScalar(greet);
PY_END;
}

static 
PyObject* pyOrfOuterProd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  PyObject* pyArg2(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyArg1, &pyArg2))
    return NULL;

  std::vector<double> vec1 = asDblVec(pyArg1);
  std::vector<double> vec2 = asDblVec(pyArg2);
  size_t nrow = vec1.size();
  size_t ncol = vec2.size();
  
  // allocate and compute the answer
  std::vector<std::vector<double>> outprod(nrow);
  for (size_t i = 0; i < nrow; ++i) {
	  outprod[i].resize(ncol);
	  for (size_t j = 0; j < ncol; ++j) {
		  outprod[i][j] = vec1[i] * vec2[j];
	  };
  };
	
  return asPyArray(outprod);

PY_END;
}

static 
PyObject* pyOrfPolyProd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  PyObject* pyArg2(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyArg1, &pyArg2))
    return NULL;

  std::vector<double> vec1 = asDblVec(pyArg1);
  std::vector<double> vec2 = asDblVec(pyArg2);
  size_t nrow = vec1.size();
  size_t ncol = vec2.size();
  
  if (vec1.empty() || vec2.empty()){
    throw std::invalid_argument("Inavlid Argument:: Empty Input");
  }

  // allocate and compute the answer
  std::vector<double> polyprod(nrow+ncol-1);
  for (size_t i=0; i<nrow+ncol-1;++i){
    polyprod[i]=0;
  }
  for (size_t i = 0; i < nrow; ++i) {
	  for (size_t j = 0; j < ncol; ++j) {
		  polyprod[i+j] += vec1[i] * vec2[j];
	  };
  }; 

	size_t i=nrow+ncol-2;
  while(i>0 and polyprod[i]==0){
    printf("%d",i);
    polyprod.pop_back();
    --i;
  }
  
  return asPyArray(polyprod);

PY_END;
}
