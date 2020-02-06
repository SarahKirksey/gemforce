#ifndef _GEM_SORT_H
#define _GEM_SORT_H

// black magic to have overloaded function pointers correctly resolved as template params in the sorting functions
// from https://stackoverflow.com/a/36794145
#define AS_LAMBDA(func) [&](auto&&... args) -> decltype(func(std::forward<decltype(args)>(args)...)) { return func(std::forward<decltype(args)>(args)...); }

constexpr int switch_size = 20;

template<class gem, class comparer>
inline void ins_sort(gem* gems, int len, comparer gem_less)
{
	int i,j;
	gem element;
	for (i=1; i<len; i++) {
		element=gems[i];
		for (j=i; j>0 && gem_less(element, gems[j-1]); j--) {
			gems[j]=gems[j-1];
		}
		gems[j]=element;
	}
}

template<class gem, class comparer>
void quick_sort(gem* gems, int len, comparer gem_less)
{
	if (len > switch_size)  {
		gem pivot = gems[len/2];
		gem* beg = gems;
		gem* end = gems+len-1;
		while (beg <= end) {
			while (gem_less(*beg, pivot)) {
				beg++;
			}
			while (gem_less(pivot,*end)) {
				end--;
			}
			if (beg <= end) {
				gem temp = *beg;
				*beg = *end;
				*end = temp;
				beg++;
				end--;
			}
		}
		if (end - gems + 1 < gems - beg + len) {		// sort smaller first
			quick_sort(gems, end - gems + 1, gem_less);
			quick_sort(beg, gems - beg + len, gem_less);
		}
		else {
			quick_sort(beg, gems - beg + len, gem_less);
			quick_sort(gems, end - gems + 1, gem_less);
		}
	}
}

template<class gem, class comparer>
void gem_sort(gem* gems, int len, comparer gem_less)
{
	quick_sort(gems, len, gem_less);	// partially sort
	ins_sort(gems, len, gem_less);		// finish the nearly sorted array
}

//-------------------
// Comparison methods
//-------------------

template<class gem>
inline bool gem_1D_less(const gem& gem1, const gem& gem2)
{
	return get_first(gem1) < get_first(gem2);
}

template<unsigned int ACC = 0, class gem>
inline bool gem_2D_less(const gem& gem1, const gem& gem2)
{
	if constexpr (ACC == 0) {
		if (get_first(gem1) != get_first(gem2))
			return get_first(gem1) < get_first(gem2);
	}
	else {
		if ((int)(get_first(gem1) * ACC) != (int)(get_first(gem2) * ACC))
			return get_first(gem1) < get_first(gem2);
	}
	return get_second(gem1) < get_second(gem2);
}

#endif // _GEM_SORT_H
