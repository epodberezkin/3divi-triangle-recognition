#pragma once
#include <string>
#include <vector>
#include <algorithm>

class InputParser
{
public:
	InputParser(int &, char **);
	~InputParser();
	const std::string & getCmdOption(const std::string &) const;
	bool cmdOptionExists(const std::string &) const;
private:
	std::vector <std::string> tokens;
};

