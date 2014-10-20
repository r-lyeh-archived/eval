#pragma once
#include <string>

void config_eval();
double eval( const std::string &expr, std::string *err = 0 );
