//  #define DEBUG
#ifndef DEBUG
#include "p8_5.h"
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

vector<set<int>> adj, radj;
vector<int> order, scc, real_name;
vector<bool> visited;
vector<pair<int, int>> ans;
vector<int> tail_id, head_id;
stack<int> single_head, multi_head, single_tail, multi_tail;
void dfs(int u) {
	visited[u] = true;
	for (auto v : adj[u]) {
		if (!visited[v]) dfs(v);
	}
	order.push_back(u);
}
void dfs2(int u, int scc_id) {
	scc[u] = scc_id;
	for (auto v : radj[u]) {
		if (scc[v] == -1) dfs2(v, scc_id);
	}
}
void set_scc() {
	visited.clear(); visited.resize(real_name.size(), false);
	order.clear();
	for (int i = 0; i < real_name.size(); i++) {
		if (!visited[i]) dfs(i);
	}
	scc.clear(); scc.resize(real_name.size(), -1);
	vector<int> new_real_name;
	for (auto it = order.rbegin(); it != order.rend(); it++) {
		if (scc[*it] == -1) {
			dfs2(*it, new_real_name.size());
			new_real_name.push_back(real_name[*it]);
		}
	}
	real_name = new_real_name;
	vector<set<int>> new_adj(real_name.size()), new_radj(real_name.size());
	for (int i = 0; i < adj.size(); i++) {
		for (auto v : adj[i]) {
			if (scc[i] != scc[v]) {
				new_adj[scc[i]].insert(scc[v]);
				new_radj[scc[v]].insert(scc[i]);
			}
		}
	}
	adj = new_adj;
	radj = new_radj;
}
void dest_dfs(int u, const vector<set<int>> &adj, vector<int> &dest_id) {
	for (auto v : adj[u]) {
		if (dest_id[v] == -1)
			dest_dfs(v, adj, dest_id);
		if (dest_id[u] == -1) {
			dest_id[u] = dest_id[v];
			if (dest_id[u] == -2)
				return;
		}
		else if (dest_id[u] != dest_id[v]) {
			dest_id[u] = -2;
			return;
		}
	}
	if (dest_id[u] == -1)
		dest_id[u] = u;
}
void bidfs(int u) {
	visited[u] = true;
	for (auto v : adj[u]) {
		if (!visited[v]) bidfs(v);
	}
	for (auto v : radj[u]) {
		if (!visited[v]) bidfs(v);
	}
	if (adj[u].empty()) {
		dest_dfs(u, radj, head_id);
		if (head_id[u] == -2) {
			multi_tail.push(u);
		} else {
			single_tail.push(u);
		}
	}
	if (radj[u].empty()) {
		dest_dfs(u, adj, tail_id);
		if (tail_id[u] == -2) {
			multi_head.push(u);
		} else {
			single_head.push(u);
		}
	}
}
void init_tail_head() {
	tail_id.clear(); tail_id.resize(real_name.size(), -1);
	head_id.clear(); head_id.resize(real_name.size(), -1);
	single_head = stack<int>();
	multi_head = stack<int>();
	single_tail = stack<int>();
	multi_tail = stack<int>();
	visited.clear(); visited.resize(real_name.size(), false);
}
std::vector<std::pair<int, int>> solve(
    int n, std::vector<std::pair<int, int>> edges) {
	if (n == 1) return vector<pair<int, int>>();
	adj.clear(); adj.resize(n+1);
	radj.clear(); radj.resize(n+1);
	for (auto &edge : edges) {
		if (edge.first == edge.second) continue;
		adj[edge.first - 1].insert(edge.second - 1);
		radj[edge.second - 1].insert(edge.first - 1);
	}
	real_name.clear(); real_name.resize(n);
	for (int i = 0; i < n; i++) {
		real_name[i] = i+1;
	}
	set_scc();
	if (real_name.size() == 1) return vector<pair<int, int>>();
	init_tail_head();
	int last_tail = -1, first_head = -1;
	int last_multi_tail = -1, last_multi_head = -1;
	ans.clear();
	int this_head, this_tail;
	vector<pair<int, int>> new_edges;
	for (int i = 0; i < real_name.size(); i++) {
		if (!visited[i]) {
			if (!multi_head.empty())
				last_multi_head = multi_head.top();
			if (!multi_tail.empty())
				last_multi_tail = multi_tail.top();
			bidfs(i);
			if (multi_head.empty() || multi_head.top() == last_multi_head) {
				this_head = single_head.top();
				single_head.pop();
			} else {
				this_head = multi_head.top();
				multi_head.pop();
			}
			if (multi_tail.empty() || multi_tail.top() == last_multi_tail) {
				this_tail = single_tail.top();
				single_tail.pop();
			} else {
				this_tail = multi_tail.top();
				multi_tail.pop();
			}
			if (first_head == -1)
				first_head = this_head;
			if (last_tail != -1) {
				ans.push_back({real_name[last_tail], real_name[this_head]});
				new_edges.push_back({last_tail, this_head});
			}
			last_tail = this_tail;
		}
	}
	if (this_head != first_head) {
		ans.push_back({real_name[last_tail], real_name[first_head]});
		new_edges.push_back({last_tail, first_head});
	}
	for (auto &edge : new_edges) {
		adj[edge.first].insert(edge.second);
		radj[edge.second].insert(edge.first);
	}
	while (true) {
		set_scc();
		if (real_name.size() == 1) break;
		init_tail_head();
		bidfs(0);
		if (multi_head.empty() || multi_tail.empty()) {
			if (!multi_head.empty()) {
				while (!single_tail.empty()) {
					ans.push_back({real_name[single_tail.top()], real_name[multi_head.top()]});
					single_tail.pop();
				}
			} else if (!multi_tail.empty()) {
				while (!single_head.empty()) {
					ans.push_back({real_name[multi_tail.top()], real_name[single_head.top()]});
					single_head.pop();
				}
			} else
				ans.push_back({real_name[single_tail.top()], real_name[single_head.top()]});
			break;
		}
		while (!multi_head.empty() && !multi_tail.empty()) {
			ans.push_back({real_name[multi_tail.top()], real_name[multi_head.top()]});
			adj[multi_tail.top()].insert(multi_head.top());
			radj[multi_head.top()].insert(multi_tail.top());
			multi_tail.pop();
			multi_head.pop();
		}
	}
	return ans;
}

#ifdef DEBUG
int main()
{
	int T;
	cin >> T;
	if (T < 0) {
		srand(time(NULL));
		T = 1000;
		while (T--) {
			int n = rand() % 10 + 1;
			int m = rand() % 10 + 1;
			vector<pair<int, int>> edges;
			for (int i = 0; i < m; i++) {
				int u = rand() % n + 1;
				int v = rand() % n + 1;
				edges.push_back({u, v});
			}
			auto ans = solve(n, edges);
			adj.clear(); adj.resize(n+1);
			for (auto &edge : edges) {
				adj[edge.first].insert(edge.second);
			}
			for (auto &edge : ans) {
				adj[edge.first].insert(edge.second);
			}
			for (int i = 1; i <= n; i++) {
				visited.clear(); visited.resize(n+1, false);
				order.clear();
				dfs(i);
				for (int i = 1; i <= n; i++) {
					if (!visited[i]) {
						cout << "Wrong Answer" << endl;
						cout << n << " " << edges.size() << endl;
						for (auto &edge : edges) {
							cout << edge.first << " " << edge.second << endl;
						}
						cout << "Answer" << endl;
						for (auto &edge : ans) {
							cout << edge.first << " " << edge.second << endl;
						}
						cout << "Wrong Answer" << endl;
						return 0;
					}
				}
			}
			cout << "Correct" << T << endl;
		}
	} else {
		while (T--) {
			int n, m;
			cin >> n >> m;
			vector<pair<int, int>> edges;
			while (m--) {
				int u, v;
				cin >> u >> v;
				edges.push_back({u, v});
			}
			auto ans = solve(n, edges);
			cout << "start answer" << endl;
			for (auto &edge : ans) {
				cout << edge.first << " " << edge.second << endl;
			}
			cout << "end answer" << endl;
		}
	}
	return 0;
}
#endif
