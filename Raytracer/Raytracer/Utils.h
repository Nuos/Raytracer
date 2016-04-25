#ifndef _UTILS_H
#define _UTILS_H

class Utils
{
public:
	template<typename Type>
	static Type min(Type &a, Type &b)
	{
		return a < b ? a : b;
	}

	template<typename Type>
	static Type max(Type &a, Type &b)
	{
		return a > b ? a : b;
	}

	template<typename Type>
	static Type lerp(Type &a, Type &b, int &t)
	{
		return (1-t)*a + t*b;
	}
};

#endif