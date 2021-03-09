import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import city1 as city
import netgraph

plt.style.use('my_style')
plt.gca().set_aspect('equal', adjustable='box')
plt.title(f'City Graph (Oneway Fraction = {city.one_way_fraction})')

G = nx.DiGraph()

for i in range (0,10):
    for j in range (0,10):
        G.add_node(i*10+j+1, pos=((i+1),(j+1)))

di_edges = []*100
n_edges = []*100

for i in range (0,100):
     for j in range (0,100):
        if city.adj_matrix.item((i,j)) == 1:
            G.add_edge(i+1,j+1)
            if city.adj_matrix.item((j,i)) == 1:
                di_edges.append((i+1,j+1))
            else:
                n_edges.append((i+1,j+1))


dead_nodes = []
for i in range (0,100):
    deg = 0
    for j in range (0,100):
        deg+=city.adj_matrix.item(i,j)-city.adj_matrix.item(j,i)
    if abs(deg) == 4: #Vede solo quelli interni
        dead_nodes.append(i+1)

pos=nx.get_node_attributes(G,'pos')
nx.draw_networkx_nodes(G,pos,node_color='white',edgecolors='black', node_size=20)
nx.draw_networkx_nodes(G,pos,nodelist=dead_nodes,node_color='red',edgecolors='black', node_size=20)
nx.draw_networkx_edges(G,pos,edgelist=di_edges,arrowstyle='-')
nx.draw_networkx_edges(G,pos, edgelist=n_edges, arrowstyle='->')
plt.savefig('city.png')