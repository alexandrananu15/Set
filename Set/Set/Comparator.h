#pragma once

template<class T>
class Comparator
{
public:
	int operator()(T, T) const;
};

template<class T>
int Comparator<T>::operator()(T val1, T val2) const							//functioneaza similar functiei cmp
{
	if (val1 > val2)
		return 1;
	else if (val1 < val2)
		return -1;
	return 0;
}


template<>
int Comparator<int>::operator()(int val1, int val2) const					//operator
{
	if (!((val1 ^ val2) & 1))			//au aceeasi paritate
		return 0;
	else if (val1 > val2)
		return 1;
	return -1;
}
