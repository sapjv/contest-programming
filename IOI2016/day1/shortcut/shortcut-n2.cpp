#include "shortcut.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

struct shortcut {
	int N;
	vector<ll> X, Y;
	ll C;
	vector<ll> max_minus;
	vector<ll> max_plus;

	ll V;
	ll misum, masum, midif, madif;

	// i is first unreachable from j, or -1 if none
	void update(int i, int j) {
		i++;
		misum = max(misum, (C-V) + max_plus[i] + (Y[j] + X[j]));
		midif = max(midif, (C-V) + max_minus[i] + (Y[j] + X[j]));
		masum = min(masum, (V-C) - max_minus[i] - (Y[j] - X[j]));
		madif = min(madif, (V-C) - max_plus[i] - (Y[j] - X[j]));
	}
	
	bool do_sweep() {
		if(!(misum <= masum)) return false;
		if(!(midif <= madif)) return false;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				ll sum = X[i] + X[j];
				ll dif = X[j] - X[i];
				if(misum <= sum && sum <= masum &&
						midif <= dif && dif <= madif) return true;
			}
		}
		return false;
	}

	bool is_good(ll v) {
		V = v;
		//cerr << V << '\n';
		for(int i = 0; i < N; i++) {
			if(Y[i] > V) return false;
		}
		misum = 0, masum = INF, midif = 0, madif = INF;
		for(int j = 0; j < N; j++) {
			int i = j - 1;
			while(i >= 0 && (Y[j] + X[j]) + (Y[i] - X[i]) <= V) {
				i--;
			}
			//cerr << i << ' ' << j << '\n';
			update(i, j);
		}
		return do_sweep();
	}

	void precomp() {
		max_minus.resize(N+1);
		max_plus.resize(N+1);
		max_minus[0] = max_plus[0] = -INF;
		for(int i = 0; i < N; i++) {
			max_minus[i+1] = max(max_minus[i], Y[i] - X[i]);
			max_plus[i+1] = max(max_plus[i], Y[i] + X[i]);
		}
	}

	ll go() {
		precomp();
		ll mi = 0, ma = INF;
		while(ma - mi > 1) {
			ll md = (mi + ma) / 2;
			if(is_good(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		return ma;
	}

	shortcut(int n, vector<int> l, vector<int> d, int c)
		: N(n), X(N), Y(N), C(c) {
		X[0] = 0;
		for(int i = 1; i < N; i++) {
			X[i] = X[i-1] + l[i-1];
		}
		for(int i = 0; i < N; i++) {
			Y[i] = d[i];
		}
	}
};

ll find_shortcut(int n, vector<int> l, vector<int> d, int c) {
	return shortcut(n, l, d, c).go();
}
