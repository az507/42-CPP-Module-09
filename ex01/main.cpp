#include "RPN.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <cstring>

template<typename T>
T	str_to_nbr(const std::string &s, std::stringstream &ss)
{
	T	num;

	ss.clear();
	ss.str(s);
	if (!(ss >> num))
		throw std::invalid_argument("invalid str to nbr.");
	return (num);
}

int	main(int argc, char *argv[])
{
	RPN			*rpn;
	const char		*operations = "+-/*";
	const char		*ptr;
	std::stringstream	ss;
	std::string		buffer;

	if (argc != 2)
		return (std::cerr << "Usage: " << argv[0] << " \"math expr\"\n", 1);

	rpn = RPN::getInstance();
	try {
		while (*argv[1])
		{
			if (isdigit(*argv[1]))
				rpn->push_to_stack(str_to_nbr<int>(std::string(argv[1], 1), ss));
			else if ((ptr = strchr(operations, *argv[1])))
				rpn->perform_op(*ptr);
			else if (!isspace(*argv[1]))
				return (std::cerr << "Error\n", 1);
			argv[1]++;
		}
		std::cout << rpn->peek_top_stack() << '\n';
	}
	catch (const std::exception &e) {
		return (std::cerr << "Error: " << e.what() << '\n', 1);
	}
}
