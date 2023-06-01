#include <functional>
#include <string>

#ifndef LFD_HPP
#define LFD_HPP

struct LFD
{
	
	using Function = std::function<float(float)>;

	//generate function based on expression
	Function generateFunction(const std::string& expression);

};

#endif