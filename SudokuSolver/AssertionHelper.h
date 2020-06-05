#pragma once

#define NULLPTR_CHECK(val) \
	if ((val) == nullptr) \
		throw invalid_argument("parameter is nullptr");

#define OUTOFRANGE_CHECK(val, min, max) \
	if ((val) >= (max)) \
		throw out_of_range("parameter is out of range (too big)"); \
	if ((val) < (min)) \
		throw out_of_range("parameter is out of range (too small)");

#define VALUE_IS_CHECK(val, is) \
	if ((val) != (is)) \
		throw out_of_range("parameter is out of range (not the same)");
