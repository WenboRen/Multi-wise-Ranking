#pragma once

#include <stdio.h>
#include <tchar.h>

#include<vector>
#include<string>
#include<set>
#include<algorithm>
#include<utility>
#include<iostream>
using namespace std;

#include "Algorithms.h"
#include "Helper.h"

int main()
{
	srand(12345678);

	//string task = "Top-K";
	string task = "Full-Ranking";

	//string alg = "MultiwiseQuickSelect";
	//string alg = "BasicKSelect";
	//string alg = "MultiwiseTournamentKSelection";
	string alg = "MultiwiseQuickSort";
	//string alg = "MultiwiseMergeSort";

	int n = 100000;
	int m = 50;
	int k = 500; // only works for k-selection

	double Delta = 1.0;
	double delta = 0.01;

	int repetition = 100;

	int complexity = 0;
	double ave_complexity = 0.0;
	double accuracy = 0.0;

	//vector<int> Ms = { 2, 3, 4, 5, 6, 8, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 150, 160, 180, 200, 300, 400, 500, 600, 700, 800, 900, 920, 940, 960, 970, 980, 990, 1000 };
	vector<int> Ms; for (int i = 1000; i <= 100000; i += 1000) Ms.push_back(i);
	//vector<int> Ns = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000 };
	//vector<int> Ks = { 1, 2, 3, 4, 5, 6, 8, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 150, 160, 180, 200, 250, 300,350, 400, 450, 500 };

	cout << "n = " << 100000 << ", m = " << "x" << ", k = " << "---" << ", Delta = " << Delta << ", delta = " << delta << ", repetition = " << repetition << endl;
	cout << "x = " << endl;
	for (int m : Ms) cout << m << " ";
	//for (int k : Ks) cout << k << " ";
	//for (int n : Ns) cout << n << " ";

	cout << endl << endl;;

	vector<int> S, true_ranking;
	for (int i = 0; i < n; ++i) {
		S.push_back(i);
		true_ranking.push_back(i);
	}

	for (int t = 0; t < repetition; ++t) {
		//for (int n : Ns){
		for (int m : Ms) {
		//for (int k : Ks) {
			random_reorder(true_ranking);
			complexity = 0;
			vector<int> ans;
			vector<int> Sp(S.begin(), S.begin() + n);

			if (alg == "MultiwiseQuickSelect") {
				ans = multiwiseQuickSelect(S, m, k, true_ranking, complexity);
			}
			else if (alg == "BasicKSelect") {
				ans = basicKSelect(S, m, k, Delta, delta, true_ranking, complexity);
			}
			else if (alg == "MultiwiseTournamentKSelection") {
				ans = multiwiseTournamentKSelection(S, m, k, Delta, delta, true_ranking, complexity);
			}
			else if (alg == "MultiwiseQuickSort") {
				ans = multiwiseQuickSort(Sp, m, true_ranking, complexity);
			}
			else if (alg == "MultiwiseMergeSort") {
				ans = Sp;
				complexity = calculateMultiwiseMergeSortComplexity(n, m);
			}

			bool correct = true;
			if (task == "Top-K") {
				for (int i = 0; i < k; ++i) {
					correct = correct && true_ranking[ans[i]] < k;
				}
				set<int> check_duplication(ans.begin(), ans.end());
				if (check_duplication.size() < k) {
					correct = false;
				}
			}
			else if (task == "Full-Ranking") {
				for (int i = 0; i < n; ++i) {
					correct = correct && (true_ranking[ans[i]] == i);
				}
			}

			if (correct) {
				accuracy += 1.0;
			}

			cout << complexity << " ";
		}
		cout << endl;
	}

	system("pause");

	return 0;
}

