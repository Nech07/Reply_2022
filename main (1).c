#include <bits/stdc++.h>
int STANDARD = 1;
using namespace std;
#define F0(i,n) for (int i=0; i<n; i++)
#define FR(i,n) for (int i=n-1; i>=0; i--)
#define F1(i,n) for (int i=1; i<=n; i++)
#define CL(a,x) memset(x, a, sizeof(x));
#define SZ(x) ((int)x.size())
const int inf = 1000009;
const double pi = acos(-1.0);
typedef pair<int, int> pii;
typedef long long ll;
typedef unsigned long long ull;
const double EPS = 1e-9;
#define PR(x) cerr << #x << "=" << x << endl
template<class A, class B>
ostream& operator<<(ostream& os, const pair<A, B>& p) { os << "(" << p.first << "," << p.second << ")"; return os; }
istream& operator>>(istream& is, pii& p) { is>>p.first>>p.second; return is; }
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "["; for (int i=0;i<v.size();i++) { if (i>0) os << ","; os << v[i]; } os << "]"; return os;
}
template<class T>
ostream& operator<<(ostream& os, const set<T>& v) {
    os << "{"; int f=1; for(auto i:v) { if(f)f=0;else os << ","; os << i; } os << "}"; return os;
}

inline ll GetTSC() {
    ll lo, hi;
    asm volatile ("rdtsc": "=a"(lo), "=d"(hi));
    return lo + (hi << 32);
}
inline double GetSeconds() {
    return GetTSC() / 2.8e9;
}

const int MAX_RAND = 1 << 30;
struct Rand {
    ll x, y, z, w, o;
    Rand() {}
    Rand(ll seed) { reseed(seed); o = 0; }
    inline void reseed(ll seed) { x = 0x498b3bc5 ^ seed; y = 0; z = 0; w = 0;  F0(i, 20) mix(); }
    inline void mix() { ll t = x ^ (x << 11); x = y; y = z; z = w; w = w ^ (w >> 19) ^ t ^ (t >> 8); }
    inline ll rand() { mix(); return x & (MAX_RAND - 1); }
    inline int nextInt(int n) { return rand() % n; }
    inline int nextInt(int L, int R) { return rand()%(R - L + 1) + L; }
    inline int nextBool() { if (o < 4) o = rand(); o >>= 2; return o & 1; }
    double nextDouble() { return rand() * 1.0 / MAX_RAND; }
};
Rand my(2020);
double saveTime;
const int _SZ = 32;
double t_o[_SZ];
ll c_o[_SZ];
void Init() {
    saveTime = GetSeconds();
    F0(i, _SZ) t_o[i] = 0.0;
    F0(i, _SZ) c_o[i] = 0;
}
double Elapsed() { return GetSeconds() - saveTime; }
void Report() {
    double tmp = Elapsed();
    cerr << "-------------------------------------" << endl;
    cerr << "Elapsed time: " << tmp << " sec" << endl;
    double total = 0.0; F0(i, _SZ) { if (t_o[i] > 0) cerr << "t_o[" << i << "] = " << t_o[i] << endl; total += t_o[i]; } cerr << endl; //if (total > 0) cerr << "Total spent: " << total << endl;
    F0(i, _SZ) if (c_o[i] > 0) cerr << "c_o[" << i << "] = " << c_o[i] << endl;
    cerr << "-------------------------------------" << endl;
}
struct AutoTimer {
    int x;
    double t;
    AutoTimer(int x) : x(x) {
        t = Elapsed();
    }
    ~AutoTimer() {
        t_o[x] += Elapsed() - t;
    }
};
#define AT(i) AutoTimer a##i(i)
//#define AT(i)
vector<string> lang;
map<string, int> M;

int Id(string& s) {
    if (!M.count(s)) {
        M[s] = SZ(lang);
        lang.push_back(s);
    }
    return M[s];
}

const int N = 500001;
const int LOGN = 1 << 16;
double logs[LOGN];
const string DS="URDL";
int st, stmax, T, n;
vector<int> f[N];
int frag[N], spend[N], rectime[N], recover[N], tot[N];
vector<int> pref[N];
double pen[N];

double pw = 1.0;
double eval(int i, int time) {
    double sp = spend[i];
    sp -= 15.0 * recover[i] / (rectime[i] + 2.0);
    sp = max(sp, 1.0);
    return ((tot[i]) / pow(sp, pw)) * (1 + pen[i]);
}

int main(int argc, char* argv[]) {
    int test = -1;
    if (argc > 1) test = atoi(argv[0]);
    test = 5;
    char fin[1000], fout[1000];
    sprintf(fin, "%c.txt", test + '0');
    sprintf(fout, "%c.sol", test + '0');
    ignore = freopen(fin, "r", stdin);
    ignore = freopen(fout, "w", stdout);

    cin >> st >> stmax >> T >> n;
    PR(T); PR(n);
    set<int> S;
    int cnt=0;
    F0(i, n) {
        cin >> spend[i] >> rectime[i] >> recover[i] >> frag[i];
        pref[i].push_back(0);
        F0(j, frag[i]) {
            int x;
            cin >> x;
            tot[i] += x;
            pref[i].push_back(tot[i]);
            f[i].push_back(x);
        }
    }

    PR(accumulate(tot, tot+n, 0) * 1.0 / n);
    PR(cnt);
    PR(S);

    while (1) {
        F0(i, n) pen[i] = my.nextDouble() * 0.0;
        vector<int> used(n, 0);
        vector<int> uppers, others, greed;
        F0(i, n) if (rectime[i] == 1 && spend[i] == 3) {
            assert(recover[i] == 9);
            uppers.push_back(i);
        } else {
            //if (recover[i] > spend[i])
            //if (rectime[i] < 1000)
            //if (recover[i] > spend[i] || rectime[i] >= 1000)
            others.push_back(i);
        }
        greed = others;
        sort(uppers.begin(), uppers.end(), [&](int x, int y) {
            return tot[x] < tot[y];
        });
        sort(greed.begin(), greed.end(), [&](int x, int y) {
            return tot[x] < tot[y];
        });
        sort(others.begin(), others.end(), [&](int x, int y) {
            return eval(x, 0) < eval(y, 0);
        });
        //PR(SZ(uppers));
        int stamina = st;
        int score = 0;
        vector<int> ord;
        vector<int> gain(T + 1, 0);
        for (int time = 0; time < T; time++) {
            if (time == int(0.2*T)) {
                pw = 0.9;
                sort(others.begin(), others.end(), [&](int x, int y) {
                    return eval(x, time) < eval(y, time);
                });
            }

            int i = -1;
            if (stamina + gain[time] > stmax) cerr << "Waste: " << stamina + gain[time] - stmax << endl;
            stamina = min(stamina + gain[time], stmax);

            while (used[others.back()]) others.pop_back();
            if (time < T - 99) {
                int bv = -100000;
                int bx = -1;
                int cnt = 0;
                for (int x = SZ(others)-1; x >= 0 && x >= SZ(others) - 13000; x--) if (!used[others[x]] && rectime[others[x]] + time < T) {
                    int j = others[x];
                    if (stamina >= spend[j] + 3 && stamina - spend[j] + gain[time + 1] + gain[time + 2] / 4 <= stmax) {
                        int av = time + rectime[j] < T ? 10000 - gain[time + rectime[j]] : 0;
                        if (av > bv) {
                            i = j;
                            bv = av;
                            bx = x;
                        }
                        if (++cnt > 1) break;
                    }
                }
                if (bx != -1) {
                   others.erase(others.begin() + bx);
                }
            }
            if (i == -1) {
                int bv = -1;
                int bx = -1;
                while (used[greed.back()]) greed.pop_back();
                for (int x = SZ(greed)-1; x >= 0 && x >= SZ(greed) - 5000; x--) if (!used[greed[x]]) {
                    int j = greed[x];
                    if (stamina + gain[time + 1] >= spend[j] + 3) {
                        int av = (frag[j] + time >= T) ? pref[j][T - time - 1] : tot[j];
                        if (av > bv) {
                            i = j;
                            bv = av;
                            bx = x;
                        }
                    }
                }
                if (bx != -1) {
                   greed.erase(greed.begin() + bx);
                }
            }

            if (i == -1) {
                if (stamina < 3) throw;
                if (uppers.empty()) {
                    PR(time);
                    break;
                }
                i = uppers.back(); uppers.pop_back();
            }
            used[i] = 1;
            ord.push_back(i);
            if (frag[i] + time >= T) {
                score += pref[i][T - time - 1];
            } else {
                score += tot[i]; // TODO
            }
            stamina -= spend[i];
            if (time + rectime[i] < T) gain[time + rectime[i]] += recover[i];
        }
        PR(score);
        //PR(SZ(uppers));
        for (int x : ord) cout << x << endl;
        break;
    }


    return 0;
}
