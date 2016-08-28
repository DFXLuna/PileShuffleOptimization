// PileShuffleOptimization.cpp : Defines the entry point for the console application.
// Find optimal number of piles for pile shuffling to maximize probability of 
// Two lands per pile
// Uses Hypergeometric distribution h(x; p, n, k) = [kCx][p-kCn-x]/[pCn]
// p: number of items in population
// k: Number of items in population that are successes
// n: Number of items in sample(size of piles)
// x: Number of items in sample(pile) that are success

#include "RandomUtilities.h"
#include <iostream>
using namespace std;

const int P = 100;
const int K = 40;
const int N = 3;
const int X = 2;
const int TRIALS = 10000;

float aCb(int, int);
float hyper(int, int, int, int);

int main()
{
	//for RandomUtilities
	randomizeSeed();

	float avgTrials = 0;
	for (int i = 0; i < TRIALS; ++i) {

		int n = N;
		int k = K;
		//Set population equal to non sucesses for management of population
		int p = P - k;
		int piles = 0;
		float dist = 0;
		float avgH = 0;
		int sub = 0;

		while (p > -1) {
			dist = hyper(X, p + k, n, k);
			//cout << dist << endl;
			avgH += dist;
			if (k < n) {
				sub = randInt(0, k);
			}
			else {
				sub = randInt(0, n);
			}
			k -= sub;
			p -= (sub + n - sub);
			++piles;
		}
		avgH /= piles;
		avgTrials += avgH;
	}
	avgTrials /= TRIALS;
	cout << "Average Hypergeometric Distribution over " 
	<< TRIALS <<  " trials: " << avgTrials << endl;
}

float aCb(int n, int k) {
	if (k > n) return 0;
	if (k * 2 > n) k = n - k;
	if (k == 0) return 1;

	float result = n;
	for (int i = 2; i <= k; ++i) {
		result *= (n - i + 1);
		result /= i;
	}
	return result;
}

float hyper(int x, int p, int n, int k) {
	if (n > p) return 0;
	if (n < x) return 0;
	if (k == p) return 1;
	else {
		return ((aCb(k, x) * aCb((p - k), (n - x))) / aCb(P, n));
	}
}