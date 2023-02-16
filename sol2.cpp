// Created by Nikolay Budin

#ifdef DEBUG
#  define _GLIBCXX_DEBUG
#endif
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#define ff first
#define ss second
#define szof(x) ((int)x.size())
#ifndef LOCAL
#  define cerr __get_ce
#endif

using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef unsigned long long ull;

using namespace __gnu_pbds;
template <typename T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <typename K, typename V> using ordered_map = tree<K, V, less<K>, rb_tree_tag, tree_order_statistics_node_update>;

int const INF = (int)1e9 + 1e3;
ll const INFL = (ll)1e18 + 1e6;
mt19937 tw(chrono::high_resolution_clock::now().time_since_epoch().count());
uniform_int_distribution<ll> ll_distr;
ll rnd(ll a, ll b) { return ll_distr(tw) % (b - a + 1) + a; }
uniform_real_distribution<double> d_distr;
double rnd() {
	return d_distr(tw);
}

struct demon {
	int sc, tr, sr;
	vector<int> pref;
	demon(int _sc, int _tr, int _sr) : sc(_sc), tr(_tr), sr(_sr) {
		pref.push_back(0);
	}

	int get_sc(int t) {
		if (t >= szof(pref)) {
			return pref.back();
		}
		return  pref[t];
	}

	double get_sort_val() {
		return (sr - sc) / (double) tr;
	}
};

bool is_eq(demon const& d1, demon const& d2) {
	return d1.sc == d2.sc && d1.tr == d2.tr && d1.sr == d2.sr && d1.pref.back() == d2.pref.back();
}

bool is_strictly_better(demon const& d1, demon const& d2) {
	return d1.sc <= d2.sc && d1.tr <= d2.tr && d1.sr >= d2.sr && d1.pref.back() >= d2.pref.back() && !is_eq(d1, d2);
}

string file_name;

void solve() {
	int s_init, s_max, turns, dems;
	cin >> s_init >> s_max >> turns >> dems;
	vector<demon> demons;

	vector<ll> scores;
	for (int i = 0; i < dems; ++i) {
		int sc, tr, sr, n;
		cin >> sc >> tr >> sr >> n;
		demons.push_back(demon(sc, tr, sr));
		for (int j = 0; j < n; ++j) {
			int num;
			cin >> num;
			demons[i].pref.push_back(demons[i].pref.back() + num);
		}
		scores.push_back(demons[i].pref.back());
	}

	sort(scores.rbegin(), scores.rend());

	cerr << "max score <= " << accumulate(scores.begin(), scores.begin() + min(turns, dems), 0ll) << endl;

	int calc_cnt_used;
	auto calc = [&](vector<int> const& perm) {
		int curs = s_init;
		static vector<int> recover_at(turns);
		fill(recover_at.begin(), recover_at.end(), 0);
		int cnt = 0;
		ll res = 0;
		ll sum_st = 0;
		for (int t = 0; t < turns && cnt < szof(perm); ++t) {
			// sum_st += (min(curs + recover_at[t], s_max) - curs) * (turns - t);
			sum_st += min(curs + recover_at[t], s_max) - curs;
			curs = min(curs + recover_at[t], s_max);
			// sum_st = max(sum_st, (ll) curs);
			int dind = perm[cnt];
			if (curs >= demons[dind].sc) {
				// sum_st -= demons[dind].pref.back() - demons[dind].get_sc(turns - t);
				curs -= demons[dind].sc;
				sum_st -= demons[dind].sc;
				++cnt;
				if (t + demons[dind].tr < turns) {
					recover_at[t + demons[dind].tr] += demons[dind].sr;
				}
				res += demons[dind].get_sc(turns - t);
				// sum_st += (ll) demons[dind].get_sc(turns - t) * (turns - t);
			}
		}

		calc_cnt_used = cnt;

		return pair(res, sum_st);
	};

	auto save_result = [&](vector<int> const& perm) {
		assert(file_name != "");
		ofstream out;
		out.open(file_name);
		for (int i = 0; i < dems && i < turns; ++i) {
			out << perm[i] << "\n";
		}
		out.close();
	};

	vector<int> perm;

	if (file_name != "") {
		ifstream in;
		in.open(file_name);
		perm.clear();
		int ind;
		set<int> have;
		while (in >> ind) {
			assert(!have.count(ind));
			have.insert(ind);
			perm.push_back(ind);
		}
		in.close();

		perm.resize(turns);
		have = set<int>(perm.begin(), perm.end());

		auto get_left = [&](set<int> used) {
			vector<int> ret;
			for (int i = 0; i < dems; ++i) {
				if (!used.count(i)) {
					ret.push_back(i);
				}
			}
			return ret;
		};

		vector<int> left = get_left(have);
		sort(left.begin(), left.end(), [&](int a, int b) {
			return demons[a].sc < demons[b].sc;
		});

		for (int i = 0; i < turns && i < szof(left); ++i) {
			perm.push_back(left[i]);
			have.insert(left[i]);
		}

		left = get_left(have);
		sort(left.begin(), left.end(), [&](int a, int b) {
			return demons[a].tr < demons[b].tr;
		});

		for (int i = 0; i < turns && i < szof(left); ++i) {
			perm.push_back(left[i]);
			have.insert(left[i]);
		}

		left = get_left(have);
		sort(left.begin(), left.end(), [&](int a, int b) {
			return demons[a].sr > demons[b].sr;
		});

		for (int i = 0; i < turns && i < szof(left); ++i) {
			perm.push_back(left[i]);
			have.insert(left[i]);
		}

		left = get_left(have);
		sort(left.begin(), left.end(), [&](int a, int b) {
			return demons[a].pref.back() > demons[b].pref.back();
		});

		for (int i = 0; i < turns && i < szof(left); ++i) {
			perm.push_back(left[i]);
			have.insert(left[i]);
		}

		left = get_left(have);
		sort(left.begin(), left.end(), [&](int a, int b) {
			return demons[a].sr - demons[a].sc > demons[b].sr - demons[b].sc;
		});

		for (int i = 0; i < turns && i < szof(left); ++i) {
			perm.push_back(left[i]);
			have.insert(left[i]);
		}

		left = get_left(have);
		sort(left.begin(), left.end(), [&](int a, int b) {
			return (demons[a].sr - demons[a].sc) / (double) demons[a].tr > 
				(demons[b].sr - demons[b].sc) / (double) demons[b].tr;
		});

		for (int i = 0; i < turns && i < szof(left); ++i) {
			perm.push_back(left[i]);
			have.insert(left[i]);
		}

		// assert(szof(perm) == dems);
	}

	auto best_perm = perm;
	auto best_score = calc(best_perm);
	cerr << "init with score " << best_score.ff << " " << best_score.ss << endl;
	auto last_upd = clock();
	
	// while (true) {
	// 	bool changed = false;
	// 	for (int i = 0; i < turns; ++i) {
	// 		for (int j = turns; j < dems; ++j) {
	// 			if (is_strictly_better(demons[perm[j]], demons[perm[i]])) {
	// 				swap(perm[i], perm[j]);
	// 				changed = true;
	// 			}
	// 		}
	// 	}
	// 	if (!changed) {
	// 		break;
	// 	}
	// }
	
	
	auto cur_score = calc(perm);
	bool need_save = false;
	if (best_score < cur_score) {
		best_score = cur_score;
		best_perm = perm;
		need_save = true;
	}

	cerr << szof(perm) << endl;

	ll last_upd_tt = 0;
	for (ll tt = 0; ; ++tt) {
		if (false && tt % 100000 == 0) {
			while (true) {
				bool changed = false;
				for (int i = 0; i < turns; ++i) {
					for (int j = turns; j < szof(perm); ++j) {
						if (is_strictly_better(demons[perm[j]], demons[perm[i]])) {
							swap(perm[i], perm[j]);
							changed = true;
						}
					}
				}
				if (!changed) {
					break;
				}
			}
			cur_score = calc(perm);
			cerr << "new cur score " << cur_score.ff << " " << cur_score.ss << endl;
			last_upd_tt = tt;
		}
		auto mem_perm = perm;
		int a = rnd(0, szof(perm) - 1);
		int b = rnd(0, szof(perm) - 1);
		if (rnd() < 0.5) {
			a = rnd(0, turns - 1);
			b = rnd(0, turns - 1);
		}
		if (a >= turns && b >= turns) {
			continue;
		}
		if (a > b) {
			swap(a, b);
		}
		// swap(perm[a], perm[b]);
		int val = perm[b];
		perm.erase(perm.begin() + b);
		perm.insert(perm.begin() + a, val);
		auto new_score = calc(perm);
		if (new_score >= cur_score) {
			cur_score = new_score;
			if (best_score < cur_score) {
				last_upd_tt = tt;
				best_score = cur_score;
				best_perm = perm;
				need_save = true;
			}
		} else {
			// swap(perm[a], perm[b]);
			perm = mem_perm;
		}

		if (need_save && clock() - last_upd > 5 * CLOCKS_PER_SEC) {
			cerr << "saving " << best_score.ff << " " << best_score.ss << endl;
			save_result(best_perm);
			cerr << "done" << endl;
			need_save = false;
			last_upd = clock();
		}
	}
}


int main(int argc, char* argv[]) {
#ifdef LOCAL
	auto start_time = clock();
	cerr << setprecision(3) << fixed;
#endif
	cout << setprecision(15) << fixed;
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	assert(argc >= 2);
	file_name = argv[1];

	solve();

#ifdef LOCAL
	auto end_time = clock();
	cerr << "Execution time: " << (end_time - start_time) * (int)1e3 / CLOCKS_PER_SEC << " ms\n";
#endif
}