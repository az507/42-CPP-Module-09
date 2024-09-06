#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <list>
# include <vector>
# include <iostream>
# include <iomanip>
# include <algorithm>
# include <utility>
# include <functional>
# include <limits>
# include <ctime>
# include <cmath>

class	PmergeMe
{
	public:
		static PmergeMe	&getInstance(void);
		void		addNumber(int);
		void		printNumbers(void) const;
		void		printTimeTaken(void) const;
		void		printDetailedTime(void) const;
		void		sort(void);
		void		checkSorted(void) const;

	private:
		PmergeMe(void);
		~PmergeMe(void);
		PmergeMe(const PmergeMe &);
		PmergeMe	&operator=(const PmergeMe &);

		std::vector<int>	m_vector;
		std::list<int>		m_list;
		long double		vtime_taken[2]; // [0]: data management, [1]: sorting part
		long double		ltime_taken[2];
};

#endif
