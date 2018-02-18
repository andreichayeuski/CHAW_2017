#include <iostream>

extern "C"
{
	int __stdcall strl(char* source)
	{
		int result = 0;
		while (source[result] != '\0')
		{
			result++;
		}
		return result;
	}

	int __stdcall power(int source, int value)
	{
		int result = 1;
		for (int i = 0; i < value; i++)
		{
			result *= source;
		}
		return result;
	}

	int __stdcall outc(char* value)
	{
		setlocale(0, "");
		std::cout << value << std::endl;
		return 0;
	}

	int __stdcall outd(int value)
	{
		std::cout << value << std::endl;
		return 0;
	}
}