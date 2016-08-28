// RandomUtilities.h

// tom bailey   7 mar 03
// Random Variate generation.

// tom bailey   0900  21 feb 2012
// Revisions to match current version of rand().

// tom bailey   1420   23 mar 2012
// Revision of the definition of randomInt.

//Matt Grant 11 Feb 2016
//Added methods to generate random sorted and unsorted vector and 2D vector

//Matt Grant 27 Mar 2016
//Added method to generate random pairs and collections of pairs
//Changed vect methods to template functions


#ifndef _RANDOM_UTILITIES_H_
#define _RANDOM_UTILITIES_H_

#include <cmath>
#include <ctime>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;


// Seed the random-number generator with current time so that
// the numbers will be different every time we run.
// WARNING:  Only do this once per run!
// The static variable called enforces the one call per run 
// rule.
void randomizeSeed()
{
	static bool called = false;
	if (!called)
	{
		srand(unsigned long(time(NULL)));
		called = true;
	}
}


// Return a random variate distributed uniformly between
// zero and one.  All doubles in this range are 
// approximately equally likely.
// 0 <= r < 1
double randUniform()
{
	double r = double(rand()) / (RAND_MAX + 1.0);
	r = (double(rand()) + r) / (RAND_MAX + 1.0);
	r = (double(rand()) + r) / (RAND_MAX + 1.0);
	r = (double(rand()) + r) / (RAND_MAX + 1.0);
	return  r;
};


// Return a random variate distributed uniformly on the
// range minReal upto, not including, maxReal.
// minReal is assumed < maxReal.
double randReal(double minReal, double maxReal)
{
	return minReal
	+ (maxReal - minReal) * randUniform();
}


// Return a random variate distributed uniformly on the
// range minInt upto, not including, maxInt.
// minInt is assumed < maxInt.
int randInt(int minInt, int maxInt)
{
	return int(minInt
	+ (maxInt - minInt) * randUniform());
}


// Return a random variate from the exponential
// distribution with mean one.
double randExpon()
{
	double unif = randUniform();
	double expon = -log(unif);
	return expon;
}


// Return a random variate from the triangle distribution 
// with the minimum, most likely, and maximum values given
// by the three parameters.
double randTriangle(double minimum, double mode, double maximum)
{
	double larger(randUniform());
	double other(randUniform());
	if (larger < other)
	{
		larger = other;
	}

	if (randUniform() * (maximum - minimum) < (mode - minimum))
	{
		return minimum + larger * (mode - minimum);
	}
	else
	{
		return maximum + larger * (mode - maximum);
	}
}


// Return a random variate from the Erlang ( sum of k 
// exponential samples ) distribution.  The mean of the 
// distribution is k.
double randErlang(short k)
{
	double sum = 0.0;
	for (short i = 0; i < k; i++)
	{
		sum += randExpon();
	}
	return sum / double(k);
}

//Creates a vector of size containing random T between min and max
template<class T>
vector<T> randVect(unsigned int size,
	const T min, const T max, T(*randFunc)(T, T)) {
	vector<T> toReturn;

	while (toReturn.size() < size) {
		toReturn.push_back(randFunc(min, max));
	}
	return toReturn;
}

//Overload
//Creates a vector of size containing random T without min/max parameters
//Permits use of random functions for which min and max either
//aren't used or are undefined.
template<class T>
vector<T> randVect(unsigned int size, T(*randFunc)()) {
	vector<T> toReturn;

	while (toReturn.size() < size) {
		toReturn.push_back(randFunc());
	}
	return toReturn;
}


//Calls randVect to create a randomized vector of T and sorts using <algorithm> sort
//Lets <algorithm> sort handle comparing
template<class T>
vector<T> randSortVect(unsigned int size,
	const T min, const T max, T(*randFunc)(T, T)) {
	vector<T> toReturn = randVect(size, min, max, randFunc);

	sort(toReturn.begin(), toReturn.end());

	return toReturn;

}

//Calls randVect to create a randomized vector of T and sorts using <algorithm> sort
//Uses no parameters for min/max
//Permits use of random functions for which min and max either
//aren't used or are undefined.
//Lets <algorithm> sort handle comparing
template<class T>
vector<T> randSortVect(unsigned int size, T(*randFunc)()) {
	vector<T> toReturn = randVect(size, randFunc);

	sort(toReturn.begin(), toReturn.end());

	return toReturn;

}

//Calls randVect to create a randomized vector of T and sorts using <algorithm> sort
//Lets client code specify a comparator
template<class T>
vector<T> randSortVect(unsigned int size,
	const T min, const T max, T(*randFunc)(T, T), bool(*comp)(T, T)) {
	vector<T> toReturn = randVect(size, min, max, randFunc);

	sort(toReturn.begin(), toReturn.end(), comp);

	return toReturn;

}

//Calls randVect to create a randomized vector of T and sorts using <algorithm> sort
//Uses no parameters for min/max
//Permits use of random functions for which min and max either
//aren't used or are undefined.
//Lets client code handle comparing
template<class T>
vector<T> randSortVect(unsigned int size, T(*randFunc)(), bool(*comp)(T, T)) {
	vector<T> toReturn = randVect(size, randFunc);

	sort(toReturn.begin(), toReturn.end(), comp);

	return toReturn;

}

//Generates pair of random T between min and max using paramater random 
//function
template<class T>
pair<T, T> randPair(const T min, const T max, T(*randFunc)(T, T)) {
	return (pair<T, T>(
		randFunc(min, max),
		randFunc(min, max)));

}

//Generates pair of random T using function that has no parameters
//Permits use of random functions for which min and max either
//aren't used or are undefined.
template<class T>
pair<T, T> randPair(T(*randFunc)()) {
	return(pair<T, T>(
		randFunc(),
		randFunc()));
}

//Generates vector of pairs of random T using randPair
template<class T>
vector< pair<T, T> > randPairVect(unsigned int size,
	T min, T max, T(*randFunc)(T, T)) {
	vector< pair<T, T> > toReturn;

	while (toReturn.size() < size) {
		toReturn.push_back(randPair(min, max, randFunc));
	}

	return toReturn;
}

//Generates vector of pairs of random T with function that has no parameters
//Permits use of random functions for which min and max either
//aren't used or are undefined.
template<class T>
vector< pair<T, T> >randPairVect(unsigned int size, T(*randFunc)()) {
	vector< pair<T, T> > toReturn;

	while (toReturn.size() < size) {
		toReturn.push_back(randPair(randFunc));
	}

	return toReturn;
}
#endif