#include "PmergeMe.hpp"

PmergeMe	&PmergeMe::getInstance(void)
{
	static PmergeMe	singleInstance;

	return (singleInstance);
}

void	PmergeMe::addNumber(int num)
{
	if (find(m_vector.begin(), m_vector.end(), num) != m_vector.end())
		throw std::runtime_error("Duplicate number found in input.");
	m_vector.push_back(num);
	m_list.push_back(num);
}

void	PmergeMe::printNumbers(void) const
{
	for (std::vector<int>::const_iterator it = m_vector.begin(); it != m_vector.end(); ++it)
		std::cout << *it << ' ';
	std::cout << '\n';
}

void	merge(std::vector<int>::iterator low, std::vector<int>::iterator mid, std::vector<int>::iterator high)
{
	std::vector<int>::iterator	it1 = low, it2 = mid;
	std::vector<int>		tmp;

	while (it1 < mid && it2 < high)
	{
		if (*it1 < *it2)
			tmp.push_back(*it1++);
		else
			tmp.push_back(*it2++);
	}
	for ( ; it1 < mid; ++it1)
		tmp.push_back(*it1);
	for ( ; it2 < high; ++it2)
		tmp.push_back(*it2);
	std::copy(tmp.begin(), tmp.end(), low);
}

void	binaryInsertSort(std::vector<int>::iterator low, std::vector<int>::iterator high)
{
	std::vector<int>::iterator	left, mid, right;
	int				key;

	for (std::vector<int>::iterator it = low + 1; it != high; ++it)
	{
		key = *it;
		left = low;
		right = it - 1;
		while (left <= right)
		{
			mid = left + ((right - left) >> 1);
			if (*mid > key)
			{
				if (mid == low || *(mid - 1) < key)
				{
					std::copy_backward(mid, it, it + 1);
					*mid = key;
					break ;
				}
				else
					right = mid - 1;
			}
			else if (*mid < key)
			{
				if (mid + 1 != it && *(mid + 1) > key)
				{
					std::copy_backward(mid + 1, it, it + 1);
					*(mid + 1) = key;
					break ;
				}
				else
					left = mid + 1;
			}
		}
	}
}

void	mergeSort(std::vector<int>::iterator low, std::vector<int>::iterator high)
{
	std::vector<int>::iterator	mid;
	std::ptrdiff_t			diff;

	diff = high - low;
	if (diff > 30)
	{
		mid = low + ((high - low) >> 1);
		mergeSort(low, mid);
		mergeSort(mid, high);
		merge(low, mid, high);
	}
	else if (diff > 1)
		binaryInsertSort(low, high);
}

void	merge(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& low,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& mid,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator high)
{
	std::ptrdiff_t	diff1, diff2, dist;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >	tmp;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	it1, it2;

//	std::cout << "in merge, mid is pointing to: " << *(mid->first) << std::endl;
//	std::cout << "before merge	:";
//	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = low; it != high; ++it)
//		std::cout << *(it->first) << ' ';
//	std::cout << std::endl;
	it1 = low;
	it2 = mid;
	diff1 = std::distance(low, mid);
	diff2 = std::distance(mid, high);
	dist = diff1 + diff2;
	while (diff1 > 0 && diff2 > 0)
	{
	//	std::cout << "*(it1->first) = " << *(it1->first) << ", *(it2->first) = " << *(it2->first) << std::endl;
		if (*(it1->first) < *(it2->first))
		{
		//	std::cout << "\t-----A-----\n";
			tmp.splice(tmp.end(), pairs_list, it1++);
			--diff1;
		}
		else
		{
			//std::cout << "\t-----B-----\n";
			tmp.splice(tmp.end(), pairs_list, it2++);
			--diff2;
		}
	}
	if (diff1)
		tmp.splice(tmp.end(), pairs_list, it1, it2);
	if (diff2)
		tmp.splice(tmp.end(), pairs_list, it2, high);
	pairs_list.splice(high, tmp, tmp.begin(), tmp.end());
	low = mid = high;
	std::advance(low, -dist);
	std::advance(mid, -(dist >> 1));
//	std::cout << "after merge	:";
//	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = low; it != high; ++it)
//		std::cout << *(it->first) << ' ';
//	std::cout << std::endl;
}

void	merge_sort(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& low,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& high)
{
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	mid;
	std::ptrdiff_t										dist;

//	std::cout << "in here\n";
//	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator tmp = pairs_list.begin(); tmp != pairs_list.end(); ++tmp)
//		std::cout << *(tmp->first) << ' ';
//	std::cout << "\n\n";
	dist = std::distance(low, high);
	if (dist > 1)
	{
		mid = low;
		std::advance(mid, dist >> 1);
//		std::cout << "low is pointing to: " << *(low->first) << std::endl;
//		std::cout << "mid is pointing to: " << *(mid->first) << std::endl;
		merge_sort(pairs_list, low, mid);
		merge_sort(pairs_list, mid, high);
		merge(pairs_list, low, mid, high);
	}
}

void	binary_search_insert(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& it, std::list<int>::iterator end)
{
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	low = pairs_list.begin(), mid, high = pairs_list.end();
	std::ptrdiff_t										diff1, diff2, dist;
	int											key, cmp_value;

	key = *(it->second);
	it->second = end;
	//while (mid != it)
	while (1)
	{
		// The moment one of the iterators cross over it, this while loop should break i think
//		diff1 = std::distance(low, pairs_list.end());
//		diff2 = std::distance(high, pairs_list.end())
		diff1 = std::distance(low, it); 
		if (high == pairs_list.end())
			diff2 = std::numeric_limits<std::ptrdiff_t>::max();
		else
			diff2 = std::distance(high, it);
		if (diff1 < diff2)
			break ;
		dist = diff1 - diff2;
		mid = low;
		std::advance(mid, dist >> 1);
		cmp_value = *(mid->first);
		if (cmp_value > key)
		{
			//if (mid == pairs_list.begin() || (--std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator(mid))->first < key)
			if (mid == pairs_list.begin() || *((--mid)->first) < key)
			{
				pairs_list.splice(++mid, pairs_list, it++);
				--it;
				break ;
			}
			else
				high = mid;
		}
		else
		{
			if (++mid != it && *(mid->first) > key)
			{
				pairs_list.splice(mid, pairs_list, it++);
				--it;
				break ;
			}
			else
				low = mid;
		}
		
	}
	std::cout << "dist from low to end: " << std::distance(low, pairs_list.end()) << std::endl;
	std::cout << "dist from high to end: " << std::distance(high, pairs_list.end()) << std::endl;
}

void	insertion_sort(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list, std::list<int>& last_node)
{
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = pairs_list.begin(); it != pairs_list.end(); ++it)
	{
		std::cout << "At start: " << *(it->first) << ", " << *(it->second) << std::endl;
	}

	pairs_list.push_front(std::make_pair(pairs_list.front().second, last_node.end()));
	(++pairs_list.begin())->second = last_node.end();

	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = pairs_list.begin(); it != pairs_list.end(); ++it)
	{
		std::cout << "after removing start: " << *(it->first) << ", " << *(it->second) << std::endl;
	}
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = pairs_list.begin(); it != pairs_list.end(); ++it)
	{
		std::cout << "Before insert: " << *(it->first) << std::endl;
		if (it->second != last_node.end())
		{
			std::cout << "\tGOING TO BIN INSERT" << std::endl;
			binary_search_insert(pairs_list, it, last_node.end()); //binary search insert a node
		}
		std::cout << "After insert: " << *(it->first) << std::endl;
	}

	if (!last_node.empty())
	{
		pairs_list.push_back(std::make_pair(last_node.begin(), last_node.end()));
		binary_search_insert(pairs_list, --pairs_list.end(), last_node.end()); //binary search insert a node
	}

	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = pairs_list.begin(); it != pairs_list.end(); ++it)
	{
		std::cout << *(it->first) << ", ";
		if (it->second != last_node.end())
			std::cout << *(it->second) << std::endl;
		else
			std::cout << "not found" << std::endl;
	}
}

void	merge_insert_sort(std::list<int> list)
{
	std::cout << '\n';
	std::list<int>										last_node, sorted_list, unsorted_list;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >		pairs_list;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	it1, it2;

	if (list.size() % 2 != 0)
	{
		last_node.splice(last_node.begin(), list, --list.end());
	}

	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
	{
		pairs_list.push_back(std::make_pair(it, ++it));
	}
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator tmp = pairs_list.begin(); tmp != pairs_list.end(); ++tmp)
	{
		if (*(tmp->first) < *(tmp->second))
			std::iter_swap(tmp->first, tmp->second);
	}

	it1 = pairs_list.begin();
	it2 = pairs_list.end();
	merge_sort(pairs_list, it1, it2);
	insertion_sort(pairs_list, last_node);

	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator tmp = pairs_list.begin(); tmp != pairs_list.end(); ++tmp)
	{
		std::cout << "first: " << *(tmp->first) << std::endl;
		std::cout << "second: ";
		if (tmp->second != last_node.end())
			std::cout << *(tmp->second) << std::endl;
		else
			std::cout << "not found" << std::endl;
		std::cout << '\n';
	}
	std::cout << std::endl;
}

void	PmergeMe::sort(void)
{
	clock_t	start_time, end_time;

//	if (m_vector.size() > 1)
//	{
//		start_time = clock();
//		mergeSort(m_vector.begin(), m_vector.end());
//		end_time = clock();
//
//		time_taken[0] = static_cast<long double>((end_time - start_time) / (CLOCKS_PER_SEC * 1e-6));
//	}

	if (m_list.size() > 1)
	{
		start_time = clock();
		merge_insert_sort(m_list);
		//mergeSort(m_list, m_list.begin(), m_list.end());
		end_time = clock();

		time_taken[1] = static_cast<long double>((end_time - start_time) / (CLOCKS_PER_SEC * 1e-6));
	}
	std::cout << std::fixed << ">>> " << time_taken[0] << '\n';
}

PmergeMe::PmergeMe(void) {}
PmergeMe::~PmergeMe(void) {}
PmergeMe::PmergeMe(const PmergeMe &) {}
PmergeMe	&PmergeMe::operator=(const PmergeMe &) { return *this; }
