#pragma once

#include<vector>
#include<utility>
using namespace std;

#include "Helper.h"

const double pi = 3.1415926;

// Return the index of the winner
int basicCompare(const vector<int> &S, const double Delta, const double delta, const vector<int> &true_ranking, int &complexity) {
	if (Delta >= 0.5) {
		return multiwiseCompare(S, true_ranking, complexity);
	}

	double N = log(1.0 / delta) * 0.5 / (Delta * Delta);
	vector<int> count(S.size(), 0);
	for (int t = 0; t < N; ++t) {
		++count[noisyMultiwiseCompare(S, true_ranking, Delta, complexity)];
	}
	return max_element(count.begin(), count.end()) - count.begin();
}

vector<int> basicKSelect(const vector<int> &S, const int m, const int k, const double Delta, const double delta, const vector<int> &true_ranking, int &complexity) {
	if (S.size() <= k) {
		return S;
	}

	vector<int> ans;
	if (S.size() <= m) {
		vector<int> R = S;
		for (int i = 0; i < k; ++i) {
			int w = basicCompare(R, Delta, delta / k, true_ranking, complexity);
			ans.push_back(R[w]);
			R.erase(R.begin() + w);
		}
		return ans;
	}

	double delta_0 = delta / S.size(), delta_1 = delta_0 / S.size() * S.size();
	if (m > 2) {
		delta_0 = delta / 3.0;
		delta_1 = delta_0 / (6.0 * k + 5.0 * (1.0 + ((double)m / (m - 2)) * log(2 * (m - 1) * (m - 2) / delta_0)));
	}

	vector<bool> removed(S.size(), false);
	vector<int> V;
	while (V.size() < m - 1) {
		int id = rand() % S.size();
		if (!removed[id]) {
			V.push_back(S[id]);
			removed[id] = true;
		}
	}

	vector<int> R, Rp, A, D, E;
	for (int i = 0; i < S.size(); ++i) {
		if (!removed[i]) {
			R.push_back(S[i]);
		}
	}

	int v = -1;
	for (int t = 1; t < m; ++t) {
		int idv = basicCompare(V, Delta, delta_1, true_ranking, complexity), idr = 0;
		v = V[idv];
		E.push_back(v);
		V.erase(V.begin() + idv);

		while (idr < R.size()) {
			E.push_back(R[idr]);
			++idr;
			while (E.size() == m || (idr == R.size() && E.size() > 1)) {
				int w = basicCompare(E, Delta, delta_1, true_ranking, complexity);
				if (E[w] == v) {
					Rp.insert(Rp.end(), E.begin() + 1, E.end());
					E.resize(1);
				}
				else {
					A.push_back(E[w]);
					E.erase(E.begin() + w);
				}
			}
		}

		if (D.size() + A.size() > k) {
			A = basicKSelect(A, 2, k - D.size(), Delta, delta - delta_0, true_ranking, complexity);
			D.insert(D.end(), A.begin(), A.end());
			return D;
		}
		else if (D.size() + A.size() <= k) {
			D.insert(D.end(), A.begin(), A.end());
			if (D.size() == k) {
				return D;
			}
			D.insert(D.end(), v);
			if (D.size() == k) {
				return D;
			}
		}
		A.clear();
		E.clear();
		R.clear();
		R.swap(Rp);
	}

	A = basicKSelect(R, 2, k - D.size(), Delta, delta - delta_0, true_ranking, complexity);
	D.insert(D.end(), A.begin(), A.end());
	return D;
}


vector<int> multiwiseTournamentKSelection(const vector<int> &S, const int m, const int k, const double Delta, const double delta, const vector<int> &true_ranking, int &complexity) {
	if (S.size() <= m * k) {
		return basicKSelect(S, m, k, Delta, delta, true_ranking, complexity);
	}

	int t = 0;
	vector<int> R = S, Rp, A, T;
	while (R.size() > k) {
		++t;
		double delta_t = 9.0 * delta / (pi * pi * t * t);
		int idr = 0;
		while (idr < R.size()) {
			while (A.size() < k * m && idr < R.size()) {
				A.push_back(R[idr]);
				++idr;
			}
			T = basicKSelect(A, m, k, Delta, delta_t / k, true_ranking, complexity);
			Rp.insert(Rp.end(), T.begin(), T.end());
			A.clear();
		}
		R.clear();
		R.swap(Rp);
	}

	return R;
}


vector<int> multiwiseQuickSelect(const vector<int> &S, const int m, const int k, const vector<int> &true_ranking, int &complexity) {
	return basicKSelect(S, m, k, 1.0, 0.0, true_ranking, complexity);
}

vector<int> multiwiseQuickSort(const vector<int> &S, const int m, const vector<int> &true_ranking, int &complexity) {
	vector<int> ans;
	vector<pair<int, int>> fs;
	int n = S.size();

	if (n <= m) {
		for (int i = 0; i < n; ++i) {
			fs.push_back({ true_ranking[S[i]], S[i] });
		}
		sort(fs.begin(), fs.end(),
			[](const pair<int, int> &p1, const pair<int, int> &p2) {
			return p1.first < p2.first;
		});
		for (int i = 0; i < n; ++i) {
			ans.push_back(fs[i].second);
		}
		if (n > 1) {
			complexity += 1;
		}
		return ans;
	}

	// Find pivots
	vector<bool> processed(n, false);
	vector<int> V;
	int h = m / 2;
	while (V.size() < h) {
		int v = rand() % n;
		if (!processed[v]) {
			V.push_back(S[v]);
			processed[v] = true;
		}
	}
	for (int i = 0; i < h; ++i) {
		fs.push_back({ true_ranking[V[i]], V[i] });
	}
	sort(fs.begin(), fs.end(),
		[](const pair<int, int> &p1, const pair<int, int> &p2) {
		return p1.first < p2.first;
	});
	for (int i = 0; i < h; ++i) {
		V[i] = fs[i].second;
	}
	if (h > 1) {
		complexity += 1;
	}

	// Split items
	vector<vector<int>> A(h + 1);
	for (int i = 0; i < n; ++i) {
		if (!processed[i]) {
			for (int v = 0; v < h; ++v) {
				if (true_ranking[S[i]] < true_ranking[V[v]]) {
					A[v].push_back(S[i]);
					processed[i] = true;
					break;
				}
			}
			if (!processed[i]) {
				A[h].push_back(i);
			}
		}
	}
	complexity += ceil((double)(n - h) / (double)(m - h));

	// Recursive calls
	for (int i = 0; i <= h; ++i) {
		A[i] = multiwiseQuickSort(A[i], m, true_ranking, complexity);
	}

	// conquer
	for (int i = 0; i < h; ++i) {
		ans.insert(ans.end(), A[i].begin(), A[i].end());
		ans.push_back(V[i]);
	}
	ans.insert(ans.end(), A[h].begin(), A[h].end());
	return ans;
}

// We do not run Multiwise Merge Sort but only computes the complexity. which is the same as a real one
int calculateMultiwiseMergeSortComplexity(const int n, const int m) {
	if (n == 1) {
		return 0;
	}

	if (n <= m) {
		return n - 1;
	}

	int ans = 0, remaining = n;
	for (int i = 0; i < m; ++i) {
		int part_size = remaining / (m - i);
		remaining -= part_size;
		ans += calculateMultiwiseMergeSortComplexity(part_size, m);
	}

	ans += n - 1; // for merging all the items

	return ans;
}