#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <fstream>
# include <sstream>
# include <string>
# include <cstring>
# include <iostream>
# include <iomanip>
# include <limits>
# include <map>

class	BitcoinExchange
{
	public:
		BitcoinExchange(const char *);
		~BitcoinExchange(void);

		bool	validateDate(const std::string &);
		bool	extractValue(const std::string &, double &);
		std::map<std::string, double>::const_iterator	findDatabseEntry(const std::string &) const;

	private:
		std::ifstream			csv_databse;
		std::stringstream		ss;
		std::map<std::string, double>	map;
		int				date_format[3];

		BitcoinExchange(void);
		BitcoinExchange(const BitcoinExchange &);
		BitcoinExchange	&operator=(const BitcoinExchange &);
};

void	ft_error(const char *msg);

#endif
