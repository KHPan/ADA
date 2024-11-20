import networkx as nx
import random
import os

T = int(input())
for count in range(T):
	N = random.randint(1, 10)
	M = random.randint(N-1, min(N*(N-1)//2, 30))
	G = nx.gnm_random_graph(N, M)
	while not nx.is_connected(G):
		G = nx.gnm_random_graph(N, M)
	edges = list(G.edges())
	with open("input.txt", "w") as f:
		f.write(f"{N} {M}\n")
		for u, v in edges:
			f.write(f"{u} {v}\n")
	os.system("./ada2 < input.txt > output.txt")
	with open("output.txt") as f:
		C_ans = int(f.readline())
	G.remove_nodes_from(nx.articulation_points(G))
	P_ans = N * (N-1) // 2
	for g in nx.connected_component_subgraphs(G):
		P_ans -= g.number_of_nodes() * (g.number_of_nodes()-1) // 2
	if C_ans == P_ans:
		print(f"Correct {count+1}/{T}")
	else:
		print("input:")
		with open("input.txt") as f:
			print(f.read())
		print("output:")
		print(C_ans)
		break