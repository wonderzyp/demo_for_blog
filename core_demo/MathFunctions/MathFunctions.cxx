
#include "MathFunctions.h"

#include <unistd.h>
#include <iostream>
#include <cmath>

#ifdef USE_MYMATH
#  include "mysqrt.h"
#endif

namespace mathfunctions {
double sqrt(double x)
{
  std::cout<<"Enter here"<<std::endl;
  int *p = nullptr;
  *p = 855;


// which square root function should we use?
#ifdef USE_MYMATH
  return detail::mysqrt(x);
#else
  return std::sqrt(x);
#endif
}
}
