#include "PmergeMe.hpp"

PmergeMe	&PmergeMe::getInstance(void)
{
	static PmergeMe	singleInstance;

	return (singleInstance);
}

double	timediff_ms(const struct timespec& start, const struct timespec& end)
{
	return (end.tv_sec - start.tv_sec) * 1e6 + (double)(end.tv_nsec - start.tv_nsec) / 1e3;
}

void	PmergeMe::addNumber(int num)
{
	if (std::find(m_vector.begin(), m_vector.end(), num) != m_vector.end())
		throw std::runtime_error("Duplicate number found in input.");
								clock_gettime(CLOCK_REALTIME, &this->time[0]);
	m_vector.push_back(num);				clock_gettime(CLOCK_REALTIME, &this->time[1]);
								time_taken[0] += timediff_ms(time[0], time[1]);

								clock_gettime(CLOCK_REALTIME, &this->time[0]);
	m_list.push_back(num);					clock_gettime(CLOCK_REALTIME, &this->time[1]);
								time_taken[1] += timediff_ms(time[0], time[1]);
}

template<typename T>
int	getDigitsNbr(T num)
{
	int	digits = 0;

	if (num <= 0) {
		num *= -1; digits++;
	}
	while (num > 0)	{
		num /= 10;
		digits++;
	}
	return digits;
}

void	PmergeMe::printNumbers(void) const
{
	int	digitsNbr = getDigitsNbr(*std::max_element(m_vector.begin(), m_vector.end()));

//	for (std::vector<int>::const_iterator it = m_vector.begin(); it != m_vector.end(); ++it)
//		std::cout << std::setw(digitsNbr) << *it << ' ';
//	std::cout << '\n';
	for (std::list<int>::const_iterator it = m_list.begin(); it != m_list.end(); ++it)
		std::cout << std::setw(digitsNbr) << *it << ' ';
	std::cout << '\n';
}

void	PmergeMe::printTimeTaken(void) const
{
	std::cout << "Time to process a range of " << std::setw(3) << m_vector.size() << " elements with std::vector<int> : ";
	std::cout << std::fixed << time_taken[0] << " us" << std::endl;

	std::cout << "Time to process a range of " << std::setw(3) << m_list.size() << " elements with std::list<int>   : ";
	std::cout << std::fixed << time_taken[1] << " us" << std::endl;
}

/*
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

void	PmergeMe::generateJacobsthalSequence(void)
{
	int	max = m_vector.size() >> 1, n, prev = 1;

	for (int i = 3; i < std::numeric_limits<int>::max(); ++i)
	{
		n = getJacobsthalNumber(i);
		if (n <= max) {
			for (int x = n; x > prev; --x) {
				jacobsthalSequence.push_back(x);
			}
		} else {
			for (int x = max; x > prev; --x) {
				jacobsthalSequence.push_back(x);
			}
			break ;
		}
		prev = n;
	}
}
*/

int	getJacobsthalNumber(int i)
{
	if (i <= 1)
		return i;
	return getJacobsthalNumber(i - 1) + 2 * getJacobsthalNumber(i - 2);
}

void	mergeInsertSort(std::vector<int>& vec, int level);
void	mergeInsertSort(std::list<int>& list, int level);
void	rearrangeVectorElems(std::vector<int>& vec, const std::vector<int>& ref);

void	PmergeMe::sort(void)
{
	size_t	old_vsize = m_vector.size(), old_lsize = m_list.size();

//	generateJacobsthalSequence();
//	if (old_vsize > 1)
//		merge_insert_sort(m_vector, vtime_taken, jacobsthalSequence);
//	if (old_lsize > 1)
//		merge_insert_sort(m_list, ltime_taken, jacobsthalSequence);
//	jacobsthalSequence.clear();
	if (this->m_vector.size() > 1) {
										clock_gettime(CLOCK_REALTIME, &this->time[0]);
		mergeInsertSort(this->m_vector, 1);				clock_gettime(CLOCK_REALTIME, &this->time[1]);
										this->time_taken[0] += timediff_ms(time[0], time[1]);
										clock_gettime(CLOCK_REALTIME, &this->time[0]);
		mergeInsertSort(this->m_list, 1);				clock_gettime(CLOCK_REALTIME, &this->time[1]);
										this->time_taken[1] += timediff_ms(time[0], time[1]);
	}
	if (old_vsize != m_vector.size() || old_lsize != m_list.size()) {
		throw std::logic_error("missing elements in container after sorting");
	}
}

void	compareAndSwap(std::vector<int>& vec, int n)
{
	std::vector<int>::iterator	it1 = vec.begin(), it2;
	std::ptrdiff_t			dist;

//	(void)dist;
	// only swap within a pair, if there are odd number of elems, the last "half-pair" doesnt need to be compared and swapped
	while (it1 != vec.end())// && vec.end() - it1 >= (n << 1))
	{
		if ((dist = vec.end() - it1) < (n << 1)) {
			break ;
		}
		it2 = it1 + n;
		if (*it1 < *it2) {
			std::swap_ranges(it1, it2, it2);
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
			for (int x = size; x > prev; --x) {
				jacob.push_back(x);
			}
			break ;
		} else {
			for (int x = n; x > prev; --x) {
				jacob.push_back(x);
			}
		}
		prev = n;
	}
	return (jacob);
}

// low is actually upper bound, high is 1 elem past lower bound
void	binarySearchInsert(std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >& p_vec,
		std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator low,
		std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator high,
		std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator it,
		std::vector<int>::iterator dfl_value) {

	std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator	mid;
	std::vector<int>::iterator									v1, v2;
	int												key;

	assert(it->second != dfl_value);
	v1 = it->first;
	v2 = it->second;
	key = *(it->second);
	//it->second = dfl_value;
	while (low <= high) {
		mid = low + ((high - low) >> 1);
		if (*(mid->first) > key) {
			if (*mid == p_vec.back()) {
				p_vec.push_back(std::make_pair(it->second, dfl_value));
				break ;
			} else if ((++mid)->first == dfl_value || *(mid->first) < key) {
				p_vec.insert(mid, std::make_pair(it->second, dfl_value));
				break ;
			} else {
				low = mid;
			}
		} else if (*(mid->first) < key) {
			if (mid == p_vec.begin()) {
				p_vec.insert(p_vec.begin(), std::make_pair(it->second, dfl_value));
				break ;
			} else if ((--mid)->first == dfl_value || *(mid->first) > key) {
				p_vec.insert(++mid, std::make_pair(it->second, dfl_value));
				break ;
			} else {
				high = mid;
			}
		} else {
			break ;
		}
	}
	std::find(p_vec.begin(), p_vec.end(), std::make_pair(v1, v2))->second = dfl_value;
	//it->second = dfl_value;
}

// level is used for determining the range of vec iterators that we r grouping into a single object (one half of the pair)
void	mergeInsertSort(std::vector<int>& vec, int level) {

	std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator > >		p_vec;
	std::vector<int>										jacob, pend, tmp_vec;
	static int											remainder = 0;

	// recursion endpt/base cond, where we cant even form a pair (2 groups of elems) to be sorted
	if (level >= (int)vec.size() || (vec.size() / level) <= 1) {
		remainder = vec.size() % (level >> 1);
		return ;
	}
	compareAndSwap(vec, level);
	mergeInsertSort(vec, level << 1);
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end() - remainder; it += (level << 1)) {
		if (vec.end() - remainder - it < (level << 1)) {
			p_vec.push_back(std::make_pair(it, vec.end()));
			break ;
		}
		p_vec.push_back(std::make_pair(it, it + level));
	}
	std::pair<std::vector<int>::iterator, std::vector<int>::iterator>	tmp_pair;
	bool									odd = false;

	// this below will usually happen if nbr of elems, after trimming away remainders, is not a power of 2 || odd number of pairs
	if (p_vec.back().second == vec.end()) {
		tmp_pair = std::make_pair(p_vec.back().second, p_vec.back().first);
		p_vec.pop_back();
		odd = true;
	}
	//insert b1 into back of main chain
	p_vec.push_back(std::make_pair(p_vec.back().second, vec.end()));
	p_vec.at(p_vec.size() - 2).second = vec.end();

	// figure out how long my jacob seq shld be, depends on number of elems on pend chain to be inserted back into main chain
	int	x = 0;
	for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::const_iterator it = p_vec.begin(); it != p_vec.end(); ++it) {
		if (it->second != vec.end()) {
			++x;
		}
	}
	jacob = createJacobSeq(x + 1);
	if (!jacob.empty()) {
		int	count;
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
		for (std::vector<int>::const_iterator it = pend.begin(); it != pend.end(); ++it) {
			for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::iterator tmp = p_vec.begin(); tmp != p_vec.end(); ++tmp) {
				if (tmp->second != vec.end() && *(tmp->second) == *it) {
					//place elems to be inserted into pend vector, which is determined by jacob seq
					binarySearchInsert(p_vec, tmp + 1, p_vec.end(), tmp, vec.end());
					break ;
				}
			}
		}
	}
	// if there are odd number of pairs, last pair shld be handled in this if block below e.g. 3 pairs left when sorting 15 numbers
	if (odd) {
		p_vec.push_back(tmp_pair);
		binarySearchInsert(p_vec, p_vec.begin(), --p_vec.end(), --p_vec.end(), vec.end());
		p_vec.pop_back();
	}
	if (level == 1 && remainder) {
		for (std::vector<int>::iterator it = vec.end() - remainder; it != vec.end(); ++it) {
			p_vec.push_back(std::make_pair(vec.end(), it));
			binarySearchInsert(p_vec, p_vec.begin(), --p_vec.end(), --p_vec.end(), vec.end());
			p_vec.pop_back();
		}
	}
	for (std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator> >::const_iterator it = p_vec.begin(); it != p_vec.end(); ++it) {
		if (vec.end() - it->first >= level) {
			tmp_vec.insert(tmp_vec.end(), it->first, it->first + level);
		} else {
			tmp_vec.insert(tmp_vec.end(), it->first, vec.end());
		}
	}
	rearrangeVectorElems(vec, tmp_vec);
	if (level == 1) {
		std::reverse(vec.begin(), vec.end());
	}
}

void	rearrangeVectorElems(std::vector<int>& vec, const std::vector<int>& ref) {

	int	idx = 0;
	for (std::vector<int>::const_iterator it = ref.begin(); it != ref.end(); ++it) {
		//for (std::vector<int>::iterator tmp = vec.begin(); tmp != vec.end(); ++tmp) {
		for (std::vector<int>::iterator tmp = vec.begin(); tmp != vec.begin() + ref.size(); ++tmp) {
			if (*it == *tmp) {
				std::copy_backward(vec.begin() + idx, tmp, tmp + 1);
				*(vec.begin() + idx) = *it;
				break ;
			}
		}
		++idx;
	}
}

/*
void	compareAndSwap(std::vector<int>& vec, int n)
{
	std::vector<int>::iterator	it1 = vec.begin(), it2;
	std::ptrdiff_t			dist;

	(void)dist;
	while (it1 != vec.end() && vec.end() - it1 >= (n << 1))
	{
		it2 = it1 + n;
		if (*it1 < *it2)
			std::swap_ranges(it1, it2, it2);
		it1 += n << 1;
	}
}
*/

void	compareAndSwap(std::list<int>& list, int n) {

	std::list<int>::iterator	it1, it2;
	std::ptrdiff_t	dist;

	it1 = list.begin();
	while (it1 != list.end()) {//&& std::distance(it1, list.end()) >= (n << 1)) {
		if ((dist = std::distance(it1, list.end()) < (n << 1))) {
			break ;
		}
		it2 = it1;
		std::advance(it2, n);
		if (*it1 < *it2) {
			std::swap_ranges(it1, it2, it2);
		}
		std::advance(it1, n << 1);
	}
}

// low is actually upper bound, high is 1 elem past lower bound
void	binarySearchInsert(std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >& p_list,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator low,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator high,
		std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it,
		std::list<int>::iterator dfl_value) {

	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	mid;
	std::list<int>::iterator								p, v1;
	std::ptrdiff_t										diff1, diff2, dist;
	int											key;

	assert(it->second != dfl_value);
	v1 = it->first;
	p = it->second;
	key = *p;
	while (1) {
		diff1 = std::distance(low, p_list.end());
		diff2 = std::distance(high, p_list.end());
		//std::cout << "diff1 = " << diff1 << ", diff2 = " << diff2 << std::endl;
		if (diff1 < diff2) {
			break ;
		}
		mid = low;
		dist = diff1 - diff2;
		std::advance(mid, dist >> 1);
		if (*(mid->first) > key) {
			if (*mid == p_list.back()) {
				p_list.push_back(std::make_pair(p, dfl_value));
				break ;
			} else if ((++mid)->first == dfl_value || *(mid->first) < key) {
				p_list.insert(mid, std::make_pair(p, dfl_value));
				break ;
			} else {
				low = mid;
			}
		} else {
			if (*mid == p_list.front()) {
				p_list.push_front(std::make_pair(p, dfl_value));
				break ;
			} else if ((--mid)->first == dfl_value || *(mid->first) > key) {
				p_list.insert(++mid, std::make_pair(p, dfl_value));
				break ;
			} else {
				high = mid;
			}
		}
	}
	it->second = dfl_value;
	//std::find(p_list.begin(), p_list.end(), std::make_pair(v1, p))->second = dfl_value;
}

void	rearrangeListElems(std::list<int>& list, const std::list<int>& ref);

void	mergeInsertSort(std::list<int>& list, int level) {

	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >	p_list;
	static int									remainder = 0;

	if (level >= (int)list.size() || list.size() / level <= 1) {
		remainder = list.size() % (level >> 1);
		return ;
	}
	std::cout << "level = " << level << ", before compareAndSwap:	";
	for (std::list<int>::const_iterator it = list.begin(); it != list.end(); ++it) {
		std::cout << *it << ' ';
	} std::cout << std::endl;
	compareAndSwap(list, level);
	std::cout << "after compareAndSwap:			";
	for (std::list<int>::const_iterator it = list.begin(); it != list.end(); ++it) {
		std::cout << *it << ' ';
	} std::cout << '\n' << std::endl;
	mergeInsertSort(list, level << 1);
	std::list<int>::iterator	it2;
	std::ptrdiff_t			dist;
	for (std::list<int>::iterator it1 = list.begin(); it1 != list.end(); std::advance(it1, level << 1)) {
		if ((dist = std::distance(it1, list.end()) - remainder) < level << 1) {
			if (dist) {
				p_list.push_back(std::make_pair(it1, list.end()));
			}
			break ;
		}
		std::advance(it2 = it1, level);
		p_list.push_back(std::make_pair(it1, it2));
	}
	std::cout << "main pairs list, level = " << level << std::endl;
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator it = p_list.begin(); it != p_list.end(); ++it) {
		std::cout << '(' << *(it->first) << ", " << ((it->second == list.end()) ? -1 : *(it->second)) << ')' << std::endl;
	}
	std::pair<std::list<int>::iterator, std::list<int>::iterator>	tmp_pair;
	bool								odd = false;
	if (p_list.back().second == list.end()) {
		tmp_pair = std::make_pair(list.end(), p_list.back().first);
		p_list.pop_back();
		odd = true;
	}
	std::cout << "removing odd nbr of elems, before transferring b1 to a1" << std::endl;
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator it = p_list.begin(); it != p_list.end(); ++it) {
		std::cout << '(' << *(it->first) << ", " << ((it->second == list.end()) ? -1 : *(it->second)) << ')' << std::endl;
	}
	p_list.push_back(std::make_pair(p_list.back().second, list.end()));
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator	z;
	std::advance(z = p_list.end(), -2);
	z->second = list.end();
	std::cout << "after transferring b1 to a1, before binary search insert" << std::endl;
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator it = p_list.begin(); it != p_list.end(); ++it) {
		std::cout << '(' << *(it->first) << ", " << ((it->second == list.end()) ? -1 : *(it->second)) << ')' << std::endl;
	}
	int	x = 0;
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = p_list.begin(); it != p_list.end(); ++it) {
		if (it->second != list.end()) {
			++x;
		}
	}
	std::vector<int> jacob = createJacobSeq(x + 1), pend;
	if (!jacob.empty()) {
		int	count;
		/*generate pend chain based on jacob seq*/
		for (std::vector<int>::const_iterator it = jacob.begin(); it != jacob.end(); ++it) {
			count = 1;
			for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::reverse_iterator y = p_list.rbegin(); y != p_list.rend(); ++y) {
				if (y->second != list.end() && ++count == *it) {
					pend.push_back(*(y->second));
					break ;
				}
			}
		}
		for (std::vector<int>::const_iterator tmp = pend.begin(); tmp != pend.end(); ++tmp) {
			for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator it = p_list.begin(); it != p_list.end(); ++it) {
				if (it->second != list.end() && *(it->second) == *tmp) {
					binarySearchInsert(p_list, ++std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator(it),
						p_list.end(), it, list.end());
					break ;
				}
			}
		}
	}
	std::cout << "after main binary search insert" << std::endl;
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator it = p_list.begin(); it != p_list.end(); ++it) {
		std::cout << '(' << *(it->first) << ", " << ((it->second == list.end()) ? -1 : *(it->second)) << ')' << std::endl;
	}
	std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::iterator last_it;
	if (odd) {
		p_list.push_back(tmp_pair);
		binarySearchInsert(p_list, p_list.begin(), --p_list.end(), --p_list.end(), list.end());
		p_list.pop_back();
		std::cout << "after inserting last elem for odd nbr of pairs" << std::endl;
		for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator it = p_list.begin(); it != p_list.end(); ++it) {
			std::cout << '(' << *(it->first) << ", " << ((it->second == list.end()) ? -1 : *(it->second)) << ')' << std::endl;
		}
	}
	if (level == 1 && remainder) {
		std::list<int>::iterator	it;
		for (std::advance(it = list.end(), -remainder); it != list.end(); ++it) {
			p_list.push_back(std::make_pair(list.end(), it));
			binarySearchInsert(p_list, p_list.begin(), --p_list.end(), --p_list.end(), list.end());
			p_list.pop_back();
		}
		std::cout << "after binary search insert for remainder elems" << std::endl;
		for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator it = p_list.begin(); it != p_list.end(); ++it) {
			std::cout << '(' << *(it->first) << ", " << ((it->second == list.end()) ? -1 : *(it->second)) << ')' << std::endl;
		}
	}
	std::cout << std::endl;
	std::list<int>::iterator	p;
	std::list<int>			tmp_list;
	for (std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator> >::const_iterator it = p_list.begin(); it != p_list.end(); ++it) {
		assert(it->first != list.end());
		if (std::distance(it->first, list.end()) >= level) {
			std::advance(p = it->first, level);
			tmp_list.insert(tmp_list.end(), it->first, p);
		} else {
			tmp_list.insert(tmp_list.end(), it->first, list.end());
		}
	}
	rearrangeListElems(list, tmp_list);
	if (level == 1) {
		list.reverse();
	}
}
/*
	std::cout << "before rearranging: ";
	for (std::list<int>::const_iterator it = list.begin(); it != list.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;
	std::cout << "after rearranging:  ";
	for (std::list<int>::const_iterator it = list.begin(); it != list.end(); ++it) {
		std::cout << *it << ' ';
	}
*/

void	rearrangeListElems(std::list<int>& list, const std::list<int>& ref) {

	int				idx = 0;
	std::list<int>::iterator	start, p;
	for (std::list<int>::const_iterator tmp = ref.begin(); tmp != ref.end(); ++tmp) {
		for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it) {
			if (*it == *tmp) {
				std::advance(start = list.begin(), idx);
				std::copy_backward(start, it, ++std::list<int>::iterator(it));
				std::advance(p = list.begin(), idx);
				*p = *tmp;
				break ;
			}
		}
		++idx;
	}
}

void	PmergeMe::checkSorted(void) const {

	if (std::adjacent_find(m_vector.begin(), m_vector.end(), std::greater<int>()) != m_vector.end()) {
		throw std::logic_error("unsorted vector");
	}
	if (std::adjacent_find(m_list.begin(), m_list.end(), std::greater<int>()) != m_list.end()) {
		throw std::logic_error("unsorted list");
	}
}

PmergeMe::PmergeMe(void) {}
PmergeMe::~PmergeMe(void) {}
PmergeMe::PmergeMe(const PmergeMe &) {}
PmergeMe	&PmergeMe::operator=(const PmergeMe &) { return *this; }
