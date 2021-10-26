#pragma once

#include<vector>
using namespace std;

// Return the index but not the value;
int noisyMultiwiseCompare(const vector<int> &S, const vector<int> &true_ranking, const double Delta, int &complexity) {
	int best = 0, best_ranking = true_ranking[S[0]];
	for (int i = 1; i < S.size(); ++i) {
		if (true_ranking[S[i]] < best_ranking) {
			best = i;
			best_ranking = true_ranking[S[i]];
		}
	}
	if (S.size() > 1) {
		++complexity;
	}
	if (rand() < RAND_MAX * (0.5 + Delta) - 1.0 / S.size()) {
		return best;
	}
	return rand() % S.size();
}

int multiwiseCompare(const vector<int> &S, const vector<int> &true_ranking, int &complexity) {
	return noisyMultiwiseCompare(S, true_ranking, 1.0, complexity);
}

void random_reorder(vector<int> &S) {
	for (int i = 0; i < S.size(); ++i) {
		swap(S[i], S[i + rand() % (S.size() - i)]);
	}
}

