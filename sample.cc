#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "eval.hpp"

void tests() {

	std::stringstream right, wrong;

	#define test1(A) [&]() { auto _A_ = (A); if( _A_ != decltype(A)(0) ) \
	    return right << "[ OK ] " __FILE__ ":" << __LINE__ << " -> " #A " -> " << _A_ << std::endl, true; else \
	    return wrong << "[FAIL] " __FILE__ ":" << __LINE__ << " -> " #A " -> " << _A_ << std::endl, false; \
	}()

	#define test3(A,op,B) [&]() { auto _A_ = (A); auto _B_ = (B); if( _A_ op _B_ ) \
	    return right << "[ OK ] " __FILE__ ":" << __LINE__ << " -> " #A " " #op " " #B " -> " << _A_ << " " #op " " << _B_ << std::endl, true; else \
	    return wrong << "[FAIL] " __FILE__ ":" << __LINE__ << " -> " #A " " #op " " #B " -> " << _A_ << " " #op " " << _B_ << std::endl, false; \
	}()

	#define testN(NaN)    test3(NaN,!=,NaN)

    /* eval() */
    double val1 = eval("5*(4+4+1)");      // ->  45
    assert( val1 == 45 );

    double val2 = eval("-5*(2*(1+3)+1)"); // -> -45
    assert( val2 == -45 );

    double val3 = eval("5*((1+3)*2+1)");  // ->  45
    assert( val3 == 45 );



    test3( eval("(2+3)*2"), ==, 10 );

    // Some simple expressions
    test3( eval("1234"), ==, 1234 );
    test3( eval("1+2*3"), ==, 7 );

    // Parenthesis
    test3( eval("5*(4+4+1)"), ==, 45 );
    test3( eval("5*(2*(1+3)+1)"), ==, 45 );
    test3( eval("5*((1+3)*2+1)"), ==, 45 );

    // Spaces
    test3( eval("5 * ((1 + 3) * 2 + 1)"), ==, 45 );
    test3( eval("5 - 2 * ( 3 )"), ==, -1 );
    test3( eval("5 - 2 * ( ( 4 )  - 1 )"), ==, -1 );

    // Sign before parenthesis
    test3( eval("-(2+1)*4"), ==, -12 );
    test3( eval("-4*(2+1)"), ==, -12 );

    // Fractional numbers
    test3( eval("1.5/5"), ==, 0.3 );
    test3( eval("1/5e10"), ==, 2e-11 );
    test3( eval("(4-3)/(4*4)"), ==, 0.0625 );
    test3( eval("1/2/2"), ==, 0.25 );
    test3( eval("0.25 * .5 * 0.5"), ==, 0.0625 );
    test3( eval(".25 / 2 * .5"), ==, 0.0625 );

    // Repeated operators
    test3( eval("1+-2"), ==, -1 );
    test3( eval("--2"), ==, 2 );
    test3( eval("2---2"), ==, 0 );
    test3( eval("2-+-2"), ==, 4 );

    // Check for parenthesis error
    testN( eval("5*((1+3)*2+1") );
    testN( eval("5*((1+3)*2)+1)") );

    // Check for repeated operators
    testN( eval("5*/2") );

    // Check for wrong positions of an operator
    testN( eval("*2") );
    testN( eval("2+") );
    testN( eval("2*") );

    // Check for divisions by zero
    testN( eval("2/0") );
    testN( eval("3+1/(5-5)+4") );
    testN( eval("2/") ); // Erroneously detected as division by zero, but that's ok for us

    // Check for invalid characters
    testN( eval("~5") );
    testN( eval("5x") );

    // Check for multiply errors
    testN( eval("3+1/0+4$") ); // Only one error will be detected (in this case, the last one)
    testN( eval("3+1/0+4") );
    testN( eval("q+1/0)") ); // ...or the first one
    testN( eval("+1/0)") );
    testN( eval("+1/0") );

    // Check for emtpy string
    testN( eval("") );

    std::cout << right.str();
    std::cout << wrong.str();
}

int main()
{
	config_eval();
	tests();

	std::string exp;
	while (std::cout << "> ", getline(std::cin, exp))
	{
		std::string err;
		auto value = eval(exp, &err);
		if( err.empty() )
			std::cout << std::setprecision(std::numeric_limits<decltype(value)>::digits10) << value << std::endl;
		else
			std::cout << err << std::endl;
	}
	return 0;
}

