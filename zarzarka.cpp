//~ while (clock()<=69*CLOCKS_PER_SEC)
//~ #pragma comment(linker, "/stack:200000000")
#pragma GCC optimize("O3")
//~ #pragma GCC target ("avx2")
//~ #pragma GCC optimize("Ofast")
//~ #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
//~ #pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

template <typename T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define eni(x) sim > typename \
  enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge { c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair < b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
    *this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "

#define shandom_ruffle random_shuffle

using ll=long long;
using pii=pair<int,int>;
using pll=pair<ll,ll>;
using vi=vector<int>;
using vll=vector<ll>;
const int nax=1000*1007;
int pokonane=0;
#include "inout.h"

int main(int argc, char *argv[])
{
	store_input_name(argv[1]);
	store_old_output_name(argv[2]);
	store_new_output_name(argv[3]);
	wczytaj_in();
	output out=wczytaj_out();
	vector<int> bads = get_bads(), are(in.n);
	// output out;
	// for (int i = 0; i < in.n; i++) {
	// 	if (!bads[i]) {
	// 		out.push_back(i);
	// 	}
	// }
	for (auto x : out) {
		are[x] = 1;
	}
	for (int i = 0; i < in.n; i++) {
		if (!are[i]) {
			out.push_back(i);
		}
	}

	long long jest=score(out);
	output best=out;
	long long best_res=jest;
	for (long long h=0; 1; h++)
	{
		//int ile=rand()%2+1;
		int ile=1;
		vector<pii> s;
		for (int j=0; j<ile; j++)
		{
			int x=rand()%out.size();
			int y=rand()%out.size();
			x=rand()%in.turns;
			if (rand()&1)
				y=rand()%in.turns;
			if (x > y) {
				swap(x, y);
			}
			while (y < (int)out.size() - 1 && bads[out[y]] == 1 && bads[out[x]] == 0) {
				y++;
			}
			s.push_back({x, y});
		}
		for (pii i : s)
			swap(out[i.first], out[i.second]);
		long long nowe=score(out);
		if (nowe>=jest || !(rand()%1000000))
		{
			jest=nowe;
		}
		else
		{
			reverse(s.begin(), s.end());
			for (pii i : s)
				swap(out[i.first], out[i.second]);
		}
		if (jest>best_res)
		{
			best_res=jest;
			best=out;
			debug() << "mam " << jest;
		}
		same_file_every_minute(best);
		if (!(h%100000))
		{
			debug() <<  h << " " << jest;
		}
	}
	wypisz(out);
	return 0;
}
