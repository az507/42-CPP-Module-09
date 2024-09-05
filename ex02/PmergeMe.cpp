#include "PmergeMe.hpp"

PmergeMe	&PmergeMe::getInstance(void)
{
	static PmergeMe	singleInstance;

	return (singleInstance);
}

void	PmergeMe::addNumber(int num)
{
	if (std::find(m_vector.begin(), m_vector.end(), num) != m_vector.end())
		throw std::runtime_error("Duplicate number found in input.");
	m_vector.push_back(num);
	m_list.push_back(num);
}

void	PmergeMe::printNumbers(void) const
{
	for (std::vector<int>::const_iterator it = m_vector.begin(); it != m_vector.end(); ++it)
		std::cout << *it << ' ';
	std::cout << '\n';
	for (std::list<int>::const_iterator it = m_list.begin(); it != m_list.end(); ++it)
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

	it1 = low;
	it2 = mid;
	diff1 = std::distance(low, mid);
	diff2 = std::distance(mid, high);
	dist = diff1 + diff2;
	while (diff1 > 0 && diff2 > 0)
	{
		if (*(it1->first) < *(it2->first))
		{
			tmp.splice(tmp.end(), pairs_list, it1++);
			--diff1;
		}
		else
		{
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
}

void	merge_sort(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& low,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& high)
{
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	mid;
	std::ptrdiff_t										dist;

	dist = std::distance(low, high);
	if (dist > 1)
	{
		mid = low;
		std::advance(mid, dist >> 1);
		merge_sort(pairs_list, low, mid);
		merge_sort(pairs_list, mid, high);
		merge(pairs_list, low, mid, high);
	}
}

void	binary_search_insert(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& it, std::list<int>::iterator end)
{
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	low = pairs_list.begin(), mid, high = it;
	std::list<int>::iterator								ptr;
	std::ptrdiff_t										diff1, diff2, dist;
	int											key;

	ptr = it->second;
	key = *ptr;
	it->second = end;
	while (1)
	{
		diff1 = std::distance(low, it); 
		diff2 = std::distance(high, it);
		if (diff1 < diff2)
			break ;
		dist = diff1 - diff2;
		mid = low;
		std::advance(mid, dist >> 1);
		if (*(mid->first) > key)
		{
			// duplicated code here 
			if (mid == pairs_list.begin())
			{
				++it;
				pairs_list.insert(mid, std::make_pair(ptr, end));
				break ;
			}
			else if (*((--mid)->first) < key)
			{
				++it;
				pairs_list.insert(++mid, std::make_pair(ptr, end));
				break ;
			}
			else
				high = mid;
		}
		else
		{
			if (++mid == pairs_list.end())
			{
				// maybe can use push_back here?
				pairs_list.insert(mid, std::make_pair(ptr, end));
				it = mid;
				break ;
			}
			else if(*(mid->first) > key)
			{
				++it;
				pairs_list.insert(mid, std::make_pair(ptr, end));
				break ;
			}
			else
				low = mid;
		}
	}
}

void	insertion_sort(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list, std::list<int>& last_node)
{
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	it = pairs_list.begin();

	while (it != pairs_list.end())
	{
		if (it->second != last_node.end())
		{
			binary_search_insert(pairs_list, it, last_node.end()); //binary search insert a node
			continue ;
		}
		++it;
	}

	if (!last_node.empty())
	{
		pairs_list.back().second = last_node.begin();
		binary_search_insert(pairs_list, --pairs_list.end(), last_node.end());
	}
}

void	merge_insert_sort(std::list<int>& list)
{
	std::list<int>										last_node, tmp_list;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >		pairs_list;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	it1, it2;

	if (list.size() % 2 != 0)
		last_node.splice(last_node.begin(), list, --list.end());

	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
		pairs_list.push_back(std::make_pair(it, ++it));

	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator tmp = pairs_list.begin(); tmp != pairs_list.end(); ++tmp)
		if (*(tmp->first) < *(tmp->second))
			std::iter_swap(tmp->first, tmp->second);

	it1 = pairs_list.begin();
	it2 = pairs_list.end();
	merge_sort(pairs_list, it1, it2);

	pairs_list.push_front(std::make_pair(pairs_list.front().second, last_node.end()));
	(++pairs_list.begin())->second = last_node.end();

	insertion_sort(pairs_list, last_node);

	while (pairs_list.size())
	{
		tmp_list.splice(tmp_list.end(), list, pairs_list.front().first);
		pairs_list.pop_front();
	}
	list.swap(tmp_list);
}

void	merge(std::vector<std::pair<int, float> >::iterator low, std::vector<std::pair<int, float> >::iterator mid, std::vector<std::pair<int, float> >::iterator high)
{
	std::vector<std::pair<int, float> >::iterator	it1 = low, it2 = mid;
	std::vector<std::pair<int, float> >		tmp;

	while (it1 < mid && it2 < high)
	{
		if (it1->first < it2->first)
			tmp.push_back(*it1++);
		else
			tmp.push_back(*it2++);
	}
	if (it1 < mid)
		tmp.insert(tmp.end(), it1, mid);
	if (it2 < high)
		tmp.insert(tmp.end(), it2, high);
	std::copy(tmp.begin(), tmp.end(), low);
}

void	merge_sort(std::vector<std::pair<int, float> >::iterator low, std::vector<std::pair<int, float> >::iterator high)
{
	std::vector<std::pair<int, float> >::iterator	mid;

	if (high - low > 1)
	{
		mid = low + ((high - low) >> 1);
		merge_sort(low, mid);
		merge_sort(mid, high);
		merge(low, mid, high);
	}
}

void	binary_search_insert(std::vector<std::pair<int, float> >& p_vector, std::vector<std::pair<int, float> >::iterator& it)
{
	std::vector<std::pair<int, float> >::iterator	low = p_vector.begin(), mid, high = it - 1;
	std::ptrdiff_t					dist = std::distance(p_vector.begin(), it);
	int						key = it->second;

	it->second = std::numeric_limits<float>::max();
	while (low <= high)
	{
		////std::cout << "low - high: " << low - high << std::endl;
		mid = low + ((high - low) >> 1);
		if (mid->first > key)
		{
			if (mid == p_vector.begin())
			{
				//std::cout << "Option A" << std::endl;
				//std::copy_backward(mid, it, it + 1);
				p_vector.insert(p_vector.begin(), std::make_pair(key, std::numeric_limits<float>::max()));
				it = p_vector.begin() + dist;
				break ;
			}
			else if ((--mid)->first < key)
			{
				//std::cout << "Option B" << std::endl;
				//std::copy_backward(mid, it, it + 1);
				p_vector.insert(mid + 1, std::make_pair(key, std::numeric_limits<float>::max()));
				it = p_vector.begin() + dist;
				break ;
			}
			else
				high = mid;
		}
		else
		{
			if (++mid == p_vector.end())
			{
				//std::cout << "Option C" << std::endl;
				p_vector.push_back(std::make_pair(key, std::numeric_limits<float>::max()));
				it = p_vector.begin() + dist;
				break ;
			}
			else if (mid->first > key)
			{
				//std::cout << "Option D" << std::endl;
				//std::copy_backward(mid, it, it + 1);
				p_vector.insert(mid, std::make_pair(key, std::numeric_limits<float>::max()));
				it = p_vector.begin() + dist;
				break ;
			}
			else
				low = mid;
		}
	}
}

void	insertion_sort(std::vector<std::pair<int, float> >& p_vector, float last_elem)
{
	std::vector<std::pair<int, float> >::iterator	it = p_vector.begin() + 1; 
	
	while (it != p_vector.end())
	{
//		//std::cout << "v insert before, it->first: " << it->first << " ,";
//		if (it->second != std::numeric_limits<float>::max())
//			//std::cout << it->second << std::endl;
//		else
//			//std::cout << "Not found" << std::endl;
		if (it->second != std::numeric_limits<float>::max())
		{
			binary_search_insert(p_vector, it);
			//std::cout << "After binary search insert" << std::endl;
			for (std::vector<std::pair<int, float> >::const_iterator it = p_vector.begin(); it != p_vector.end(); ++it)
			{
				//std::cout << it->first << ", ";
				//if (it->second == std::numeric_limits<float>::max())
					//std::cout << "Not found" << std::endl;
				//else
					//std::cout << it->second << std::endl;
			}
			//std::cout << "\n";
		}

//		//std::cout << "v insert after, it->first: " << it->first << ", ";
//		if (it->second != std::numeric_limits<float>::max())
//			//std::cout << it->second << std::endl;
//		else
//			//std::cout << "Not found" << std::endl;
		++it;
	}
	
	if (last_elem != std::numeric_limits<float>::max())
	{
		p_vector.back().second = last_elem;
		binary_search_insert(p_vector, --p_vector.end());
	}
}

void	merge_insert_sort(std::vector<int>& vector)
{
	size_t					j, v_size = vector.size() >> 1;
	float					last_elem = std::numeric_limits<float>::max();
	std::vector<std::pair<int, float> >	pairs_vector(v_size);

	for (size_t i = 0; i < v_size; ++i)
	{
		j = i << 1;
		if (vector[j] > vector[j + 1])
			pairs_vector[i] = std::make_pair(vector[j], vector[j + 1]);
		else
			pairs_vector[i] = std::make_pair(vector[j + 1], vector[j]);
	}
	if (vector.size() % 2 != 0)
		last_elem = vector.back();
//	if (vector.size() % 2 != 0)
//		pairs_vector.push_back(std::make_pair(std::numeric_limits<float>::max(), vector.back()));
	vector.clear();

	//std::cout << "initial vector: " << std::endl;
	for (std::vector<std::pair<int, float> >::const_iterator it = pairs_vector.begin(); it != pairs_vector.end(); ++it)
	{
		//std::cout << it->first << ", ";
		//if (it->second == std::numeric_limits<float>::max())
			//std::cout << "Not found" << std::endl;
		//else
			//std::cout << it->second << std::endl;
	}

	merge_sort(pairs_vector.begin(), pairs_vector.end());

	pairs_vector.insert(pairs_vector.begin(), std::make_pair(pairs_vector.front().second, std::numeric_limits<float>::max()));
	(pairs_vector.begin() + 1)->second = std::numeric_limits<float>::max();

	//std::cout << "vector after merge sort: " << std::endl;
	for (std::vector<std::pair<int, float> >::const_iterator it = pairs_vector.begin(); it != pairs_vector.end(); ++it)
	{
		//std::cout << it->first << ", ";
		//if (it->second == std::numeric_limits<float>::max())
			//std::cout << "Not found" << std::endl;
		//else
			//std::cout << it->second << std::endl;
	}

	insertion_sort(pairs_vector, last_elem);

	//std::cout << "vector after insertion sort: " << std::endl;
	for (std::vector<std::pair<int, float> >::const_iterator it = pairs_vector.begin(); it != pairs_vector.end(); ++it)
	{
		//std::cout << it->first << ", ";
		//if (it->second == std::numeric_limits<float>::max())
			//std::cout << "Not found" << std::endl;
		//else
			//std::cout << it->second << std::endl;
	}

	for (std::vector<std::pair<int, float> >::iterator it = pairs_vector.begin(); it != pairs_vector.end(); ++it)
		vector.push_back(static_cast<int>(it->first));
}

void	PmergeMe::sort(void)
{
	clock_t	start_time, end_time;

	if (m_vector.size() > 1)
	{
		start_time = clock();
		//mergeSort(m_vector.begin(), m_vector.end());
		merge_insert_sort(m_vector);
		end_time = clock();

		time_taken[0] = static_cast<long double>((end_time - start_time) / (CLOCKS_PER_SEC * 1e-6));
	}

	if (m_list.size() > 1)
	{
		start_time = clock();
		merge_insert_sort(m_list);
		//mergeSort(m_list, m_list.begin(), m_list.end());
		end_time = clock();

		time_taken[1] = static_cast<long double>((end_time - start_time) / (CLOCKS_PER_SEC * 1e-6));
	}

	if (std::adjacent_find(m_vector.begin(), m_vector.end(), std::greater<int>()) == m_vector.end())
		std::cout << "vector is sorted" << std::endl;
	else
		throw std::logic_error("unsorted vector");
	if (std::adjacent_find(m_list.begin(), m_list.end(), std::greater<int>()) == m_list.end())
		std::cout << "list is sorted" << std::endl;
	else
		throw std::logic_error("unsorted list");
	//std::cout << std::fixed << ">>> " << time_taken[1] << " us" << std::endl;
}

PmergeMe::PmergeMe(void) {}
PmergeMe::~PmergeMe(void) {}
PmergeMe::PmergeMe(const PmergeMe &) {}
PmergeMe	&PmergeMe::operator=(const PmergeMe &) { return *this; }
