// A simple test and demonstration of the registry interface defined in
// regristy.h
//
// Author: Philip Salvaggio

#include "base_classes.h"
#include <iostream>

using namespace std;

// No parameter constructor classes.
Base0::Base0() {}
Base0::~Base0() {}

// 1-parameter constructor classes.
Base1::Base1(Base0* printer) : printer_(printer) {}
Base1::~Base1() {}

// 2-parameter constructor classes.
Base2::Base2(const Base1* printer, int id) : printer_(printer), id_(id) {}
Base2::~Base2() {}
