#ifndef DEBUG
// #define DEBUG
#endif // DEBUG

#ifndef DEBUG
#include "p9_4.h"
#endif
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
#include <stdlib.h>
#include <time.h>
#endif
#define ll long long
using namespace std;

int N, cap[400][400], flow[400][400];
int s, t;
vector<int> path;
vector<bool> vis;
bool dfs(int cur) {
	vis[cur] = true;
	if (cur == t) {
		path.clear();
		path.push_back(cur);
		return true;
	}
	for (int i = 0; i < N; i++) {
		if (!vis[i] && flow[cur][i] < cap[cur][i]) {
			if (dfs(i)) {
				path.push_back(cur);
				return true;
			}
		}
	}
	return false;
}
int Maximum_Flow(int n, std::vector<std::pair<int, int>>& edges,
        std::vector<int> capv, std::vector<int> cape) {
	if (n == 2) {
		return 0;
	}
	N = n * 2;
	capv.push_back(INT_MAX);
	capv.insert(capv.begin(), INT_MAX);
	for (int i = 0; i < 400; i++) {
		for (int j = 0; j < 400; j++) {
			cap[i][j] = 0;
			flow[i][j] = 0;
		}
	}
	for (int i = 0; i < edges.size(); i++) {
		cap[(edges[i].first - 1) * 2 + 1][(edges[i].second - 1) * 2] = cape[i];
	}
	for (int i = 0; i < capv.size(); i++) {
		cap[i * 2][i * 2 + 1] = capv[i];
	}
	s = 1;
	t = N - 1;
	vis.clear(); vis.resize(N, false);
	while (dfs(s)) {
		#ifdef DEBUG
		for (int i = 0; i < path.size(); i++) {
			cerr << path[i] << " ";
		}
		cerr << endl;
		#endif
		int min_flow = INT_MAX;
		for (int i = 0; i < path.size() - 1; i++) {
			min_flow = min(min_flow, cap[path[i + 1]][path[i]] - flow[path[i + 1]][path[i]]);
		}
		for (int i = 0; i < path.size() - 1; i++) {
			flow[path[i + 1]][path[i]] += min_flow;
			flow[path[i]][path[i + 1]] -= min_flow;
		}
		vis.clear(); vis.resize(N, false);
	}
	int ans = 0;
	for (int i = 0; i < N; i++) {
		ans += flow[s][i];
	}
	return ans;
}

#ifdef DEBUG
int main()
{
	int N, M;
	cin >> N >> M;
	vector<pair<int, int>> edges;
	vector<int> capv, cape;
	for (int i = 0; i < N - 2; i++) {
		int a;
		cin >> a;
		capv.push_back(a);
	}
	for (int i = 0; i < M; i++) {
		int a, b, c;
		cin >> a >> b >> c;
		edges.push_back({ a, b });
		cape.push_back(c);
	}
	cout << Maximum_Flow(N, edges, capv, cape) << endl;
	return 0;
}
#endif
