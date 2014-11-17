#include "calculate.hpp"

#include <cmath>

#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>

void config_eval() {
	opers.insert({"+", oper_t{false, 1, false}});
	opers.insert({"-", oper_t{false, 1, false}});
	opers.insert({"*", oper_t{false, 2, false}});
	opers.insert({"/", oper_t{false, 2, false}});
	opers.insert({"%", oper_t{false, 2, false}});
	opers.insert({"^", oper_t{true, 3, false}});
	opers.insert({"+", oper_t{false, 10, true}});
	opers.insert({"-", oper_t{false, 10, true}});
	opers.insert({"!", oper_t{true, 11, true}});

	funcs.insert({"+", func_args(1, [](args_t v)
	{
		return v[0];
	})});
	funcs.insert({"+", func_args(2, [](args_t v)
	{
		return v[0] + v[1];
	})});
	funcs.insert({"-", func_args(1, [](args_t v)
	{
		return -v[0];
	})});
	funcs.insert({"-", func_args(2, [](args_t v)
	{
		return v[0] - v[1];
	})});
	funcs.insert({"*", func_args(2, [](args_t v)
	{
		return v[0] * v[1];
	})});
	funcs.insert({"/", func_args(2, [](args_t v)
	{
		return v[1] != 0 ? v[0] / v[1] : std::numeric_limits<double>::quiet_NaN();
	})});
	funcs.insert({"%", func_args(2, [](args_t v)
	{
		return fmod(v[0], v[1]);
	})});
	funcs.insert({"^", func_args(2, [](args_t v)
	{
		return pow(v[0], v[1]);
	})});
	funcs.insert({"abs", func_args(1, [](args_t v)
	{
		return abs(v[0]);
	})});
	funcs.insert({"log", func_args(1, [](args_t v)
	{
		return log10(v[0]);
	})});
	funcs.insert({"log", func_args(2, [](args_t v)
	{
		return log(v[1]) / log(v[0]);
	})});
	funcs.insert({"ln", func_args(1, [](args_t v)
	{
		return log(v[0]);
	})});
	funcs.insert({"sqrt", func_args(1, [](args_t v)
	{
		return sqrt(v[0]);
	})});
	funcs.insert({"root", func_args(2, [](args_t v)
	{
		return pow(v[1], 1.0 / v[0]);
	})});
	funcs.insert({"sin", func_args(1, [](args_t v)
	{
		return sin(v[0]);
	})});
	funcs.insert({"cos", func_args(1, [](args_t v)
	{
		return cos(v[0]);
	})});
	funcs.insert({"tan", func_args(1, [](args_t v)
	{
		return tan(v[0]);
	})});
	funcs.insert({"asin", func_args(1, [](args_t v)
	{
		return asin(v[0]);
	})});
	funcs.insert({"acos", func_args(1, [](args_t v)
	{
		return acos(v[0]);
	})});
	funcs.insert({"atan", func_args(1, [](args_t v)
	{
		return atan(v[0]);
	})});
	funcs.insert({"atan2", func_args(2, [](args_t v)
	{
		return atan2(v[0], v[1]);
	})});
	funcs.insert({"ceil", func_args(1, [](args_t v)
	{
		return ceil(v[0]);
	})});
	funcs.insert({"floor", func_args(1, [](args_t v)
	{
		return floor(v[0]);
	})});
	funcs.insert({"min", [](args_t v)
	{
		if (v.size() > 0)
			return return_t(true, *min_element(v.begin(), v.end()));
		else
			return return_t(false, 0.0);
	}});
	funcs.insert({"max", [](args_t v)
	{
		if (v.size() > 0)
			return return_t(true, *max_element(v.begin(), v.end()));
		else
			return return_t(false, 0.0);
	}});
	funcs.insert({"!", func_args(1, [](args_t v)
	{
		return tgamma(v[0] + 1);
	})});
	funcs.insert({"pi", func_constant(acos(-1.L))});
	funcs.insert({"e", func_constant(exp(1.L))});
	funcs.insert({"nan", func_constant(NAN)});
	funcs.insert({"_", func_constant(NAN)});
}

double eval( const std::string &expr, std::string *err ) {
	static bool initialized = false;
	if( !initialized ) {
		initialized = true;
		config_eval();
	}
	try
	{
		auto postfix = infix2postfix(expr);
		/*for (auto &tok : postfix)
			cout << tok.first << "/" << tok.second << " ";
		cout << std::endl;*/
		auto value = evalpostfix(postfix);
		funcs.find("_")->second = func_constant(value);
		if(err) err->clear();
		return value;
	}
	catch (parse_error &e)
	{
		std::stringstream cerr;
		cerr << std::string(e.index() + 2, ' ') << "^" << std::endl;
		cerr << e.what() << " at " << e.index() << std::endl;
		if( err ) *err = cerr.str();
	}
	catch (std::runtime_error &e)
	{
		std::stringstream cerr;
		cerr << e.what() << std::endl;
		if( err ) *err = cerr.str();
	}
	catch (std::exception &e)
	{
		std::stringstream cerr;
		cerr << "Internal error: " << e.what() << std::endl;
		if( err ) *err = cerr.str();
	}
	return std::numeric_limits<double>::quiet_NaN();
}
