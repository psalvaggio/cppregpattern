// Implementation file for base_classes.h
// Author: Philip Salvaggio

#include "base_classes.h"

Base0::Base0() {}
Base0::~Base0() {}

Base1::Base1(Base0* printer) : printer_(printer) {}
Base1::~Base1() {}

Base2::Base2(const Base1* printer, int id) : printer_(printer), id_(id) {}
Base2::~Base2() {}
