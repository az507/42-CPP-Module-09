#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <list>
# include <vector>
# include <iostream>
# include <iomanip>
# include <algorithm>
# include <utility>
# include <functional>
# include <ctime>

class	PmergeMe
{
	public:
		static PmergeMe	&getInstance(void);
		void		addNumber(int);
		void		printNumbers(void) const;
		void		sort(void);

	private:
		PmergeMe(void);
		~PmergeMe(void);
		PmergeMe(const PmergeMe &);
		PmergeMe	&operator=(const PmergeMe &);

		std::vector<int>	m_vector;
		std::list<int>		m_list;
		long double		time_taken[2];
};

#endif
