#include "BitcoinExchange.hpp"

template<typename T>
T	str_to_nbr(const std::string &s, std::stringstream &ss)
{
	T	num;

	ss.clear();
	ss.str(s);
	if (!(ss >> num) || ss.peek() != EOF)
		throw std::runtime_error("converting string to number.");
	return (num);
}

BitcoinExchange::BitcoinExchange(const char *s1) : csv_databse(s1)
{
	double			exchange_rate;
	std::string		buffer, date;
	std::string::size_type	offset1, offset2;

	if (!csv_databse)
		throw std::invalid_argument("could not open file.");

	csv_databse.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (getline(csv_databse, buffer))
	{
		memset(date_format, 0, sizeof(int) * 3);
		offset1 = buffer.find_first_of(',');
		offset2 = buffer.find_last_of(',');
		if (offset1 == std::string::npos || offset1 != offset2)
			throw std::runtime_error("invalid nbr of commas in csv databse.");

		exchange_rate = str_to_nbr<double>(buffer.c_str() + offset1 + 1, this->ss);
		if (exchange_rate < 0.0)
			throw std::runtime_error("negative exchange exchange_rate in csv_databse.");

		date = buffer.substr(0, offset1);
		offset1 = date.find_first_of('-');
		offset2 = date.find_last_of('-');
		if (offset1 != 4 || offset2 != 7)
			throw std::runtime_error("invalid date format in csv_databse.");

		this->date_format[0] = str_to_nbr<int>(date.substr(0, offset1), this->ss);
		this->date_format[1] = str_to_nbr<int>(date.substr(offset1 + 1, 2), this->ss);
		this->date_format[2] = str_to_nbr<int>(date.substr(offset2 + 1, 2), this->ss);
		if (date_format[0] < 1 || date_format[0] > 2024
			|| date_format[1] < 1 || date_format[1] > 12
			|| date_format[2] < 1 || date_format[2] > 31)
			throw std::runtime_error("invalid date in csv_databse");

		this->map[date] = exchange_rate;
	}

	if (this->map.empty())
		throw std::runtime_error("csv_databse is empty");
	memset(date_format, 0, sizeof(int) * 3);
}

BitcoinExchange::~BitcoinExchange(void) {}

bool	BitcoinExchange::validateDate(const std::string &date)
{
	std::string::size_type	pos1, pos2;

	pos1 = date.find('-');
	pos2 = date.find('-', pos1 + 1);
	if (pos1 != 4 || pos2 != 7)
		return (ft_error("Invalid '-' position in date"), false);

	this->date_format[0] = str_to_nbr<int>(date.substr(0, pos1), this->ss);
	this->date_format[1] = str_to_nbr<int>(date.substr(pos1 + 1, 2), this->ss);
	this->date_format[2] = str_to_nbr<int>(date.substr(pos2 + 1, 2), this->ss);
	return (date_format[0] < 1 || date_format[0] > 2024
		|| date_format[1] < 1 || date_format[1] > 12
		|| date_format[2] < 1 || date_format[2] > 31)
		? ft_error(std::string("bad input => " + date).c_str()), false : true;
}

bool	BitcoinExchange::extractValue(const std::string &value, double &input_price)
{
	input_price = str_to_nbr<double>(value, this->ss);
	return (input_price < 0.0)
		? ft_error("not a positive number."), false
		: (input_price > 1000.0)
		? ft_error("too large a number."), false : true;
}

std::map<std::string, double>::const_iterator	BitcoinExchange::findDatabseEntry(const std::string &date) const
{
	std::pair<std::map<std::string, double>::const_iterator,
		std::map<std::string, double>::const_iterator>	ret;

	ret = this->map.equal_range(date);
	if (ret.first->first != date && ret.first != map.begin())
		--ret.first;
	return (ret.first);
}

BitcoinExchange::BitcoinExchange(void) {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &) {}
BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &) { return *this; }
