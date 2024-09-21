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

template<typename T>
int	getDigitsNbr(T num)
{
	int	digits = 0;

	if (num <= 0) { num *= -1; digits++; }
	while (num > 0)	{ num /= 10; digits++; }

	return (digits);
}

double	timediff_ms(const struct timespec& start, const struct timespec& end)
{
	return (end.tv_sec - start.tv_sec) * 1e6 + (double)(end.tv_nsec - start.tv_nsec) / 1e3;
}

void	PmergeMe::printNumbers(void) const
{
	int	digitsNbr = getDigitsNbr(*std::max_element(m_vector.begin(), m_vector.end()));

	for (std::vector<int>::const_iterator it = m_vector.begin(); it != m_vector.end(); ++it)
		std::cout << std::setw(digitsNbr) << *it << ' ';
	std::cout << '\n';
	/*for (std::list<int>::const_iterator it = m_list.begin(); it != m_list.end(); ++it)
		std::cout << std::setw(digitsNbr) << *it << ' ';*/
	std::cout << '\n';
}

void	PmergeMe::printTimeTaken(void) const
{
	std::cout << "Time to process a range of " << std::setw(3) << m_vector.size() << " elements with std::vector<int> : ";
	std::cout << std::fixed << vtime_taken[0] + vtime_taken[1] << " us" << std::endl;

	std::cout << "Time to process a range of " << std::setw(3) << m_list.size() << " elements with std::list<int>   : ";
	std::cout << std::fixed << ltime_taken[0] + ltime_taken[1] << " us" << std::endl;
}

void	PmergeMe::printDetailedTime(void) const
{
	std::cout << "For std::vector<int>" << std::endl;
	std::cout << "Time taken for data management: " << vtime_taken[0] << " us" << std::endl;
	std::cout << "Time taken for sorting part:    " << vtime_taken[1] << " us" << std::endl;
	std::cout << "For std::list<int>" << std::endl;
	std::cout << "Time taken for data management: " << ltime_taken[0] << " us" << std::endl;
	std::cout << "Time taken for sorting part:    " << ltime_taken[1] << " us" << std::endl;
}

void	merge(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& low,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& mid,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator high)
{
	std::ptrdiff_t										diff1, diff2, dist;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >		tmp;
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
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator& it, std::list<int>::iterator end, int key)
{
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	low = pairs_list.begin(), mid, high = it;
	std::list<int>::iterator								ptr;
	std::ptrdiff_t										diff1, diff2, dist;

	ptr = it->second;
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

void	insertion_sort(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& pairs_list, std::list<int>& last_node, long double *ltime_taken,
		const std::vector<int>& insert_elems)
{
	struct timespec										tp[2];
	
	clock_gettime(CLOCK_REALTIME, &tp[0]);
	for (std::vector<int>::const_iterator tmp = insert_elems.begin(); tmp != insert_elems.end(); ++tmp)
	{
		for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = pairs_list.begin(); it != pairs_list.end(); ++it)
		{
			if (it->second != last_node.end() && *(it->second) == *tmp)
			{
				binary_search_insert(pairs_list, it, last_node.end(), *tmp);
				break ;
			}
		}
	}

	if (!last_node.empty())
	{
		pairs_list.back().second = last_node.begin();
		binary_search_insert(pairs_list, --pairs_list.end(), last_node.end(), last_node.front());
	}										clock_gettime(CLOCK_REALTIME, &tp[1]); ltime_taken[1] += timediff_ms(tp[0], tp[1]);
}

void	merge_insert_sort(std::list<int>& list, long double *ltime_taken, const std::vector<int>& jacobsthalSequence)
{
	struct timespec										tp[2];
	std::list<int>										last_node, tmp_list;
	std::vector<int>									insert_elems;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >		pairs_list;
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	it1, it2;

											clock_gettime(CLOCK_REALTIME, &tp[0]);
	if (list.size() % 2 != 0)
		last_node.splice(last_node.begin(), list, --list.end());

	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
		pairs_list.push_back(std::make_pair(it, ++it));

	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator tmp = pairs_list.begin(); tmp != pairs_list.end(); ++tmp)
		if (*(tmp->first) < *(tmp->second))
			std::iter_swap(tmp->first, tmp->second);
											clock_gettime(CLOCK_REALTIME, &tp[1]); ltime_taken[0] = timediff_ms(tp[0], tp[1]);

	it1 = pairs_list.begin();
	it2 = pairs_list.end();								clock_gettime(CLOCK_REALTIME, &tp[0]);

	merge_sort(pairs_list, it1, it2);						clock_gettime(CLOCK_REALTIME, &tp[1]); ltime_taken[1] = timediff_ms(tp[0], tp[1]);
	
											clock_gettime(CLOCK_REALTIME, &tp[0]);
	pairs_list.push_front(std::make_pair(pairs_list.front().second, last_node.end()));
	(++pairs_list.begin())->second = last_node.end();
	int	idx;
	for (std::vector<int>::const_iterator tmp = jacobsthalSequence.begin(); tmp != jacobsthalSequence.end(); ++tmp)
	{
		idx = 0;
		for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = pairs_list.begin(); it != pairs_list.end(); ++it)
		{
			if (it->second != last_node.end())
			{
				if (idx == *tmp - 2)
				{
					insert_elems.push_back(*(it->second));
					break ;
				}
				++idx;
			}
		}
	}
											clock_gettime(CLOCK_REALTIME, &tp[1]); ltime_taken[0] += timediff_ms(tp[0], tp[1]);

	insertion_sort(pairs_list, last_node, ltime_taken, insert_elems);
											clock_gettime(CLOCK_REALTIME, &tp[0]);
	while (pairs_list.size())
	{
		tmp_list.splice(tmp_list.end(), list, pairs_list.front().first);
		pairs_list.pop_front();
	}
	list.swap(tmp_list);								clock_gettime(CLOCK_REALTIME, &tp[1]); ltime_taken[0] += timediff_ms(tp[0], tp[1]);
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

void	binary_search_insert(std::vector<std::pair<int, float> >& p_vector, std::vector<std::pair<int, float> >::iterator& it, int key)
{
	std::vector<std::pair<int, float> >::iterator	low = p_vector.begin(), mid, high = it;
	std::ptrdiff_t					dist = std::distance(p_vector.begin(), it);

	it->second = std::numeric_limits<float>::max();
	while (low <= high)
	{
		mid = low + ((high - low) >> 1);
		if (mid->first > key)
		{
			if (mid == p_vector.begin())
			{
				p_vector.insert(p_vector.begin(), std::make_pair(key, std::numeric_limits<float>::max()));
				it = p_vector.begin() + dist;
				break ;
			}
			else if ((--mid)->first < key)
			{
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
				p_vector.push_back(std::make_pair(key, std::numeric_limits<float>::max()));
				it = p_vector.begin() + dist;
				break ;
			}
			else if (mid->first > key)
			{
				p_vector.insert(mid, std::make_pair(key, std::numeric_limits<float>::max()));
				it = p_vector.begin() + dist;
				break ;
			}
			else
				low = mid;
		}
	}
}

void	insertion_sort(std::vector<std::pair<int, float> >& p_vector, float last_elem, long double *vtime_taken, const std::vector<int>& insert_elems)
{
	struct timespec					tp[2];
											clock_gettime(CLOCK_REALTIME, &tp[0]);
	for (std::vector<int>::const_iterator tmp = insert_elems.begin(); tmp != insert_elems.end(); ++tmp)
	{
		for (std::vector<std::pair<int, float> >::iterator it = p_vector.begin(); it != p_vector.end(); ++it)
		{
			if (it->second == *tmp)
			{
				binary_search_insert(p_vector, it, *tmp);
				break ;
			}
		}
	}
	
	if (last_elem != std::numeric_limits<float>::max())
	{
		p_vector.back().second = last_elem;
		binary_search_insert(p_vector, --p_vector.end(), last_elem);
	}										clock_gettime(CLOCK_REALTIME, &tp[1]); vtime_taken[1] += timediff_ms(tp[0], tp[1]);
}

void	merge_insert_sort(std::vector<int>& vector, long double *vtime_taken, const std::vector<int>& jacobsthalSequence)
{
	struct timespec				tp[2];
	size_t					j, v_size = vector.size() >> 1;
	float					last_elem = std::numeric_limits<float>::max();
	std::vector<int>			insert_elems;
	std::vector<std::pair<int, float> >	pairs_vector(v_size);
											clock_gettime(CLOCK_REALTIME, &tp[0]);
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
	vector.clear();									clock_gettime(CLOCK_REALTIME, &tp[1]); vtime_taken[0] = timediff_ms(tp[0], tp[1]);
											clock_gettime(CLOCK_REALTIME, &tp[0]);
	merge_sort(pairs_vector.begin(), pairs_vector.end());
											clock_gettime(CLOCK_REALTIME, &tp[1]); vtime_taken[1] = timediff_ms(tp[0], tp[1]);
											clock_gettime(CLOCK_REALTIME, &tp[0]);
	pairs_vector.insert(pairs_vector.begin(), std::make_pair(pairs_vector.front().second, std::numeric_limits<float>::max()));
	(pairs_vector.begin() + 1)->second = std::numeric_limits<float>::max();

	int	idx;
	for (std::vector<int>::const_iterator tmp = jacobsthalSequence.begin(); tmp != jacobsthalSequence.end(); ++tmp)
	{
		idx = 0;
		for (std::vector<std::pair<int, float> >::iterator it = pairs_vector.begin(); it != pairs_vector.end(); ++it)
		{
			if (it->second != std::numeric_limits<float>::max())
			{
				if (idx == *tmp - 2)
				{
					insert_elems.push_back(it->second);
					break ;
				}
				++idx;
			}
		}
	}
//	std::cout << "printing pend elems: ";
//	for (std::vector<int>::const_iterator it = insert_elems.begin(); it != insert_elems.end(); ++it)
//		std::cout << *it << ' ';
//	std::cout << std::endl;
											clock_gettime(CLOCK_REALTIME, &tp[1]); vtime_taken[0] += timediff_ms(tp[0], tp[1]);
	insertion_sort(pairs_vector, last_elem, vtime_taken, insert_elems);
											clock_gettime(CLOCK_REALTIME, &tp[0]);
	for (std::vector<std::pair<int, float> >::iterator it = pairs_vector.begin(); it != pairs_vector.end(); ++it)
		vector.push_back(static_cast<int>(it->first));
											clock_gettime(CLOCK_REALTIME, &tp[1]); vtime_taken[0] += timediff_ms(tp[0], tp[1]);
}

int	getJacobsthalNumber(int i)
{
	if (i <= 1)
		return i;
	return getJacobsthalNumber(i - 1) + 2 * getJacobsthalNumber(i - 2);
}

void	PmergeMe::generateJacobsthalSequence(void)
{
	int	max = m_vector.size() >> 1, n, prev = 1;

	for (int i = 3; i < std::numeric_limits<int>::max(); ++i)
	{
		n = getJacobsthalNumber(i);
		if (n <= max) {
			for (int x = n; x > prev; --x)
				jacobsthalSequence.push_back(x);
		} else {
			for (int x = max; x > prev; --x)
				jacobsthalSequence.push_back(x);
			break ;
		}
		prev = n;
	}
}

int	mergeInsertSort(std::vector<int>& vec, int level);

void	PmergeMe::sort(void)
{
//	size_t	old_vsize = m_vector.size();//, old_lsize = m_list.size();
//
//	generateJacobsthalSequence();
//	if (old_vsize > 1)
//		merge_insert_sort(m_vector, vtime_taken, jacobsthalSequence);
//	if (old_lsize > 1)
//		merge_insert_sort(m_list, ltime_taken, jacobsthalSequence);
//	jacobsthalSequence.clear();
	mergeInsertSort(this->m_vector, 1);
	std::cout << "size after sorting: " << m_vector.size() << std::endl;
//	if (old_vsize != m_vector.size())//|| old_lsize != m_list.size())
//		throw std::logic_error("missing elements in container after sorting");
}

void	compareAndSwap(std::vector<int>& vec, int n)
{
	std::vector<int>::iterator	it1 = vec.begin(), it2;

	while (it1 != vec.end() && vec.end() - it1 >= (n << 1))
	{
		it2 = it1 + n;
	//	std::cout << "before: *it1: " << *it1 << ", *it2: " << *it2 << std::endl;
		if (*it1 < *it2)
		{
			std::swap_ranges(it1, it2, it2);
		//	std::cout << "after: *it1: " << *it1 << ", *it2: " << *it2 << std::endl;
		}
		it1 += n << 1;
	}
}

std::vector<int>	createJacobSeq(int size)
{
	std::vector<int>	jacob;
	int			n, prev = 1;

	for (int i = 1; ; ++i)
	{
		n = getJacobsthalNumber(i);
		if (n >= size) {
			for (int x = size; x > prev; --x)
				jacob.push_back(x);
			break ;
		} else
			for (int x = n; x > prev; --x)
				jacob.push_back(x);
		prev = n;
	}
	return (jacob);
}

// low is actually upper bound, high is 1 elem past lower bound
void	binarySearchInsert(std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >& p_vec,
		std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator low,
		std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator high,
		std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator it,
		std::vector<int>::iterator dfl_value)
{
	std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator	mid;
	std::vector<int>::iterator									v1, v2;
	int												key;

	assert(it->second != dfl_value);
	v1 = it->first;
	v2 = it->second;
	key = *(it->second);
	//it->second = dfl_value;
	while (low <= high)
	{
		mid = low + ((high - low) >> 1);
		if (*(mid->first) > key)
		{
			if (*mid == p_vec.back()) {
				p_vec.push_back(std::make_pair(it->second, dfl_value));
				break ;
			} else if ((++mid)->first == dfl_value || *(mid->first) < key) {
				p_vec.insert(mid, std::make_pair(it->second, dfl_value));
				break ;
			} else
				low = mid;
		}
		else if (*(mid->first) < key)
		{
			if (mid == p_vec.begin()) {
				p_vec.insert(p_vec.begin(), std::make_pair(it->second, dfl_value));
				break ;
			} else if ((--mid)->first == dfl_value || *(mid->first) > key) {
				p_vec.insert(++mid, std::make_pair(it->second, dfl_value));
				break ;
			} else
				high = mid;
		}
	}
	std::find(p_vec.begin(), p_vec.end(), std::make_pair(v1, v2))->second = dfl_value;
	//it->second = dfl_value;
}

int	mergeInsertSort(std::vector<int>& vec, int level)
{
	//std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator > >::iterator	leftover;
	std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator > >		p_vec;
	std::vector<int>::iterator									leftover;
	std::vector<int>										jacob, pend, tmp_vec;
	std::ptrdiff_t											dist = 0;
	int												length, count;

	std::cout << "level: " << level << std::endl;
	//if (level >= (int)vec.size() || (int)vec.size() - level < level)
	if (level >= (int)vec.size() || (vec.size() / level) <= 1) {
		std::cout << "returning" << std::endl;
		return (1);
	}
	//std::cout << "remainder: " << vec.size() % level << std::endl;
	compareAndSwap(vec, level);
	length = mergeInsertSort(vec, level << 1);
	leftover = vec.end();
	std::cout << "vec.size(): " << vec.size() << std::endl;
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
		std::cout << *it << ' ';
	std::cout << std::endl;
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it += (level << 1))
	{
		std::cout << "vec.end() - it: " << vec.end() - it << ", level << 1: " << (level << 1) << std::endl;
		if (vec.end() - it < (level << 1)) {
		//	p_vec.push_back(std::make_pair(it, --vec.end()));
		//	leftover = it + level + 1;
			leftover = it;
			dist = vec.end() - it;
			std::cout << "leftover: " << *leftover << std::endl;
			break ;
		}
		//std::cout << "pushed, *it: " << *it << std::endl;
		p_vec.push_back(std::make_pair(it, it + level));
//		if (vec.end() - it < (level << 1))
//		{
//			//std::cout << "breaking, vec.end() - it: " << vec.end() - it << std::endl;
//			//leftover = it;
//			//p_vec.push_back(std::make_pair(it + level + 1, --vec.end()));
//			break ;
//		}
	}
//	if (leftover != vec.end())
//		std::cout << "leftover: " << *leftover << std::endl;

	std::cout << "Before appending b1 to a1" << std::endl;
	for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::const_iterator it = p_vec.begin(); it != p_vec.end(); ++it)
		std::cout << "a: " << *(it->first) << ", b: " << ((it->second == vec.end()) ? std::numeric_limits<int>::min() : *(it->second)) << std::endl;
	//insert b1 into back of main chain
	p_vec.push_back(std::make_pair(p_vec.back().second, vec.end()));
	p_vec.at(p_vec.size() - 2).second = vec.end();

	int	x = 0;
	for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::const_iterator it = p_vec.begin(); it != p_vec.end(); ++it) {
		if (it->second != vec.end()) {
			++x;
		}
	}
	std::cout << "\tx:	" << x << std::endl;

	jacob = createJacobSeq(x + 1);
	std::cout << "jacob of length " << length << ": ";
	for (std::vector<int>::const_iterator it = jacob.begin(); it != jacob.end(); ++it)
		std::cout << *it << ' ';
	std::cout << std::endl;

	std::cout << "After appending b1 to a1" << std::endl;
	for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::const_iterator it = p_vec.begin(); it != p_vec.end(); ++it)
		std::cout << "a: " << *(it->first) << ", b: " << ((it->second == vec.end()) ? std::numeric_limits<int>::min() : *(it->second)) << std::endl;

	if (!jacob.empty()) {
		//place elems to be inserted into pend vector, which is determined by jacob seq
		for (std::vector<int>::const_iterator it = jacob.begin(); it != jacob.end(); ++it) {
			count = 1;
			for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::reverse_iterator y = p_vec.rbegin(); y != p_vec.rend(); ++y) {
				if (y->second != vec.end() && ++count == *it) {
					pend.push_back(*(y->second));
					break ;
				}
			}
		}
		std::cout << "before primary binary search insert" << std::endl;
		for (std::vector<int>::const_iterator it = pend.begin(); it != pend.end(); ++it) {
			for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator tmp = p_vec.begin(); tmp != p_vec.end(); ++tmp) {
				if (tmp->second != vec.end() && *(tmp->second) == *it) {
					//place elems to be inserted into pend vector, which is determined by jacob seq
					binarySearchInsert(p_vec, tmp + 1, p_vec.end(), tmp, vec.end());
					break ;
				}
			}
			if (it + 1 == pend.end())
				std::cout << "break cond met" << std::endl;
		}
	}
	std::cout << "after primary binary search insert" << std::endl;
	for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::const_iterator it = p_vec.begin(); it != p_vec.end(); ++it)
		std::cout << "a: " << *(it->first) << ", b: " << ((it->second == vec.end()) ? std::numeric_limits<int>::min() : *(it->second)) << std::endl;
	// remainder should be 1 group by itself, so only need 1 insertion for 1 group (remainder amt) of elems (which shld be less than length)

	if (leftover != vec.end() && level != 1) {
		std::cout << "inserting leftover elems" << std::endl;
		p_vec.push_back(std::make_pair(vec.end(), leftover));
		binarySearchInsert(p_vec, p_vec.begin(), --p_vec.end(), --p_vec.end(), vec.end());
		p_vec.pop_back();
	} else if (leftover != vec.end() && level == 1) {
		for (std::vector<int>::iterator it = leftover; it != vec.end(); ++it) {
			p_vec.push_back(std::make_pair(vec.end(), it));
			binarySearchInsert(p_vec, p_vec.begin(), --p_vec.end(), --p_vec.end(), vec.end());
		}
	}
	std::cout << "after leftover binary search insert" << std::endl;
	for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::const_iterator it = p_vec.begin(); it != p_vec.end(); ++it)
	{
		//std::cout << "level during insert to tmp: " << level << std::endl;
		if (vec.end() - it->first >= (level << 1)) {
			tmp_vec.insert(tmp_vec.end(), it->first, it->first + level);
			std::cout << "Option A" << std::endl;
		} else {
			tmp_vec.insert(tmp_vec.end(), it->first, vec.end());
			std::cout << "Option B, vec.end() - it->first: " << vec.end() - it->first << ", level: " << level << std::endl;
		}
	}
//	if (leftover != vec.end() && level != 1)
//		tmp_vec.insert(tmp_vec.end(), leftover, leftover + dist);
	std::cout << "done\n" << std::endl;
	vec.swap(tmp_vec);
	if (level == 1)
		std::reverse(vec.begin(), vec.end());
	return (length << 1);
}

void	PmergeMe::checkSorted(void) const
{
	if (std::adjacent_find(m_vector.begin(), m_vector.end(), std::greater<int>()) != m_vector.end())
		throw std::logic_error("unsorted vector");
//	if (std::adjacent_find(m_list.begin(), m_list.end(), std::greater<int>()) != m_list.end())
//		throw std::logic_error("unsorted list");
}

PmergeMe::PmergeMe(void) {}
PmergeMe::~PmergeMe(void) {}
PmergeMe::PmergeMe(const PmergeMe &) {}
PmergeMe	&PmergeMe::operator=(const PmergeMe &) { return *this; }
