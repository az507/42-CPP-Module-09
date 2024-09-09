#include "BitcoinExchange.hpp"

void	ft_error(const char *msg)
{
	std::cerr << "Error: " << msg << '\n';
}

int	main(int argc, char *argv[])
{
	if (argc == 1)
		return 0;
	try {
		BitcoinExchange		btc("data.csv");
		std::ifstream		input_databse(argv[1]);
		std::string::size_type	offset;
		std::string		buffer, date;
		double			input_value;
		long double		total_value;

		if (!input_databse)
			throw std::invalid_argument("could not open file.");

		input_databse.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		while (getline(input_databse, buffer))
		{
			offset = buffer.find('|');
			date = buffer.substr(0, offset - 1);
			try {
				if (!btc.validateDate(date))
					continue ;
				else if (offset == std::string::npos)
					ft_error("no pipe symbol seperator.");
				else if (!btc.extractValue(buffer.substr(offset + 1), input_value))
					continue ;
				else {
					//std::cout << "rate: " << btc.findDatabseEntry(date)->second << std::endl;
					total_value = btc.findDatabseEntry(date)->second * input_value;
					//std::cout << std::fixed << std::setprecision(2);
					std::cout << date << " => " << input_value << " = " << total_value << '\n';
				}
			}
			catch (const std::exception &e) {
				ft_error(e.what());
			}
		}
	}
	catch (const std::exception &e) {
		ft_error(e.what());
		return (1);
	}
}
