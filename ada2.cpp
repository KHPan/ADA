#ifndef DEBUG
// #define DEBUG
#endif // DEBUG

#ifndef DEBUG
#include "p8_6.h"
#endif
#include <numeric>
#include <vector>
#include <algorithm>
#include <tuple>
#include <climits>
#include <queue>
#include <set>
#include <map>
#include <stack>
#ifdef DEBUG
#include <iostream>
#endif
#define ll long long
using namespace std;

vector<vector<int>> adj;
vector<int> depth, to_depth;
vector<bool> is_art;
void dfs(int v, int d) {
	depth[v] = d;
	to_depth[v] = d;

	bool root = (d == 0);
	for (auto u : adj[v]) {
		if (depth[u] == -1) {
			dfs(u, d+1);
			to_depth[v] = min(to_depth[v], to_depth[u]);
			if (to_depth[u] >= d) {
				if (root)
					root = false;
				else
					is_art[v] = true;
			}
		}
		else
			to_depth[v] = min(to_depth[v], depth[u]);
	}
}
ll dfs2(int v) {
	is_art[v] = true;
	ll ret = 1;
	for (auto u : adj[v]) {
		if (!is_art[u]) {
			ret += dfs2(u);
		}
	}
	return ret;
}
long long solve(int n, int m, std::vector<int> &u, std::vector<int> &v) {
	adj.clear(); adj.resize(n);
	for (int i = 0; i < m; i++) {
		adj[u[i]].push_back(v[i]);
		adj[v[i]].push_back(u[i]);
	}
	depth.clear(); depth.resize(n, -1);
	to_depth.clear(); to_depth.resize(n);
	is_art.clear(); is_art.resize(n, false);
	dfs(0, 0);
	ll ans = (ll)n * (n-1) / 2;
	for (int i = 0; i < n; i++) {
		if (!is_art[i]) {
			ll x = dfs2(i);
			ans -= x * (x-1) / 2;
		}
	}
	return ans;
}

#ifdef DEBUG
int main()
{
	int n, m;
	cin >> n >> m;
	vector<int> u(m), v(m);
	for (int i = 0; i < m; i++) {
		cin >> u[i] >> v[i];
	}
	cout << solve(n, m, u, v) << endl;
}
#endif

