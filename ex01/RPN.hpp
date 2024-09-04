#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <stdexcept>

class	RPN
{
	public:
		static RPN	*getInstance(void);

		void	push_to_stack(int);
		void	perform_op(char);
		int	peek_top_stack(void) const;

	private:
		RPN(void);
		~RPN(void);
		RPN(const RPN &);
		RPN	&operator=(const RPN &);

		std::stack<int>		m_stack;
};

#endif
