Eval <a href="https://travis-ci.org/r-lyeh/eval"><img src="https://api.travis-ci.org/r-lyeh/eval.svg?branch=master" align="right" /></a>
====

Very simple expression evaluator using shunting-yard algorithm and RPN (C++11).

### API
```c++
// public api: evaluate string
double eval( const std::string &expr, std::string *err = 0 );
```

### Showcase
```c++
#include "eval.hpp"
int main() {
    double var = eval("sin(11+12-13*(14)/15)"); // var == -0.991703
}
```

### Changelog
- v1.0.2 (2016/05/19): Fix out-of-range iterator access
- v1.0.1 (2016/03/26): Add simple support for assignment and variables
- v1.0.0 (2016/02/20): Extra math stuff; Header-only now; Initial SemVer adherence;
