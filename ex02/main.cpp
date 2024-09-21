#include "PmergeMe.hpp"
#include <sstream>

template<typename T, typename U>
T	str_to_nbr(const U *s, std::stringstream &ss)
{
	T	num;

	ss.clear();
	ss.str(s);
	if (!(ss >> num) || ss.peek() != std::char_traits<char>::eof())
		throw std::invalid_argument("converting str to nbr");
	return (num);
}

int	main(int argc, char *argv[])
{
	std::stringstream	ss;
	PmergeMe		&instance = PmergeMe::getInstance();
	int			num;

	if (argc == 1)
		return (0);
	try {
		while (*++argv)
		{
			num = str_to_nbr<int, char>(*argv, ss);
			if (num < 0)
				return (std::cerr << "Error\n", 1);
			instance.addNumber(num);
		}
	}
	catch (const std::exception &e) {
		return (std::cerr << "Error: " << e.what() << '\n', 1);
	}

	std::cout << "Before:	";
	instance.printNumbers();

	instance.sort();

	std::cout << "After:	";
	instance.printNumbers();
	instance.printTimeTaken();
	//instance.printDetailedTime();

	instance.checkSorted();
}
