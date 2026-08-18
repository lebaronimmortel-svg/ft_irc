#include "../includes/Header.hpp"

int main(int argc, char** argv)
{
	(void) argv;

	if (argc != 3)
	{
		std::cerr << "Error : too few arguments" << std::endl;
		return (-1);
	}
	
	return (0);
}
