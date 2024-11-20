#define DEBUG
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
vector<int> depth, to_depth, children;
vector<int> groups;
ll ans;
int art_cnt;
void dfs(int v, int d) {
	depth[v] = d;
	to_depth[v] = d;
	children[v] = 1;

	bool me_art = false, root = (d == 0);
	for (auto u : adj[v]) {
		if (depth[u] == -1) {
			dfs(u, d+1);
			to_depth[v] = min(to_depth[v], to_depth[u]);
			if (to_depth[u] >= d) {
				if (root)
					root = false;
				else {
					groups.push_back(children[u]);
					me_art = true;
				}
			} else
				children[v] += children[u];
		}
		else
			to_depth[v] = min(to_depth[v], depth[u]);
	}
	if (me_art) {
		children[v]--;
		art_cnt++;
	}
}
long long solve(int n, int m, std::vector<int> &u, std::vector<int> &v) {
	adj.clear(); adj.resize(n);
	for (int i = 0; i < m; i++) {
		adj[u[i]].push_back(v[i]);
		adj[v[i]].push_back(u[i]);
	}
	depth.clear(); depth.resize(n, -1);
	to_depth.clear(); to_depth.resize(n);
	children.clear(); children.resize(n);
	groups.clear();
	art_cnt = 0;
	dfs(0, 0);
	ans = n * (n-1) / 2;
	// int rest = adj.size();
	// while (art_cnt--) {
	// 	rest--;
	// 	ans += rest;
	// }
	// for (auto g : groups) {
	// 	ans += (ll)g * (rest - g);
	// 	rest -= g;
	// }
	groups.push_back(n - art_cnt - accumulate(groups.begin(), groups.end(), 0));
	for (auto g : groups) {
		ans -= (ll)g * (g - 1) / 2;
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

