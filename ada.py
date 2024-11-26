import networkx as nx
import random
import os

T = int(input("要跑幾次? "))
N_MAX = 200
C_MAX = 1000
for count in range(T):
	N = random.randint(2, N_MAX)
	M = random.randint(1, N_MAX)
	G = nx.gnm_random_graph(N, M, directed=True)
	G = nx.relabel_nodes(G, {0: N})
	for n in G.nodes():
		if n in (1, N):
			G.nodes[n]["capacity"] = 1000000000
		else:
			G.nodes[n]["capacity"] = random.randint(1, C_MAX)
	for e in G.edges():
		G.edges[e]["capacity"] = random.randint(1, C_MAX)
	with open("input.txt", "w") as f:
		f.write(f"{N} {M}\n")
		f.write(" ".join(str(G.nodes[i]["capacity"]) for i in range(2, N-1)) + "\n")
		for u, v, c in G.edges(data="capacity"):
			f.write(f"{u} {v} {c}\n")
	os.system("./ada < input.txt > output.txt")
	with open("output.txt") as f:
		C_ans = int(f.readline())

	H = nx.DiGraph()
	for u, v, c in G.edges(data="capacity"):
		H.add_edge(f"{u}out", f"{v}in", capacity=c)
	for n, c in G.nodes(data="capacity"):
		H.add_edge(f"{n}in", f"{n}out", capacity=c)
	P_ans = nx.maximum_flow_value(H, f"{1}out", f"{N}in")

	if C_ans == P_ans:
		print(f"Correct {count+1}/{T}")
	else:
		print("input:")
		with open("input.txt") as f:
			print(f.read())
		print("output:")
		print(C_ans)
		break