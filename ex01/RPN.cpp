#include "RPN.hpp"

RPN	*RPN::getInstance(void)
{
	static RPN	singleInstance;

	return (&singleInstance);
}

void	RPN::push_to_stack(int num)
{
	m_stack.push(num);
}

void	RPN::perform_op(char op_type)
{
	int	num1;
	int	num2;

	if (m_stack.size() < 2)
		throw std::runtime_error("Not enough numbers to perform op.");
	num1 = m_stack.top();
	m_stack.pop();
	num2 = m_stack.top();
	m_stack.pop();
	switch (op_type)
	{
		case '+':
			num2 += num1;
			break ;
		case '-':
			num2 -= num1;
			break ;
		case '/':
			num2 /= num1;
			break ;
		case '*':
			num2 *= num1;
			break ;
		default:
			throw std::invalid_argument("unknown op type.");
	}
	m_stack.push(num2);
}

int	RPN::peek_top_stack(void) const
{
	return m_stack.top();
}

RPN::RPN(void) {}
RPN::~RPN(void) {}
RPN::RPN(const RPN &) {}
RPN	&RPN::operator=(const RPN &) { return *this; }
