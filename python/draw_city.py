import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import importlib

city_number = 1
city_name = f'city{city_number}'
city = importlib.import_module(city_name, package=None)

plt.style.use('my_style')
plt.gca().set_aspect('equal', adjustable='box')
plt.title(f'City Graph (Oneway Fraction = {city.one_way_fraction})')

G = nx.DiGraph()

rows = city.n_rows
coloumns = city.n_coloumns
adj_rows = rows*coloumns

def is_boundary(node):
    if node in range (0,coloumns) or node in range((rows-1)*coloumns,rows*coloumns) or node%coloumns == 0 or node%(coloumns-1) == 0:
        return True
    else:
        return False

def is_corner(node):
    if node == 0 or node == coloumns-1 or node == (rows-1)*coloumns or node == rows*coloumns-1:
        return True
    else:
        return False


for i in range (0,rows):
    for j in range (0,coloumns):
        G.add_node(i*coloumns+j+1, pos=((i+1),(j+1)))


di_edges = []
n_edges = []

# Splits one arrow from double arrow edges
for i in range (0,adj_rows):
     for j in range (0,adj_rows):
        if city.adj_matrix.item((i,j)) == 1:
            G.add_edge(i+1,j+1)
            if city.adj_matrix.item((j,i)) == 1:
                di_edges.append((i+1,j+1))
            else:
                n_edges.append((i+1,j+1))


# Looks for unreachable nodes and edges
dead_nodes = []
dead_edges = []
for i in range (0,adj_rows):
    deg = 0
    max_deg = 0
    for j in range (0,adj_rows):
        deg+=city.adj_matrix.item(i,j)-city.adj_matrix.item(j,i)
    if is_boundary(i):
        if is_corner(i):
            max_deg = 2
        else:
            max_deg = 3
    else:
        max_deg = 4
    if max_deg == abs(deg):
        dead_nodes.append(i+1)
        for j in range(0,adj_rows):
            if city.adj_matrix.item(i,j):
                dead_edges.append((i+1,j+1))
            elif city.adj_matrix.item(j,i) == 1:
                dead_edges.append((j+1,i+1))

def dead_cleaner():
    nx.draw_networkx_edges(G,pos,edgelist=dead_edges,edge_color='white')
    nx.draw_networkx_nodes(G,pos,nodelist=dead_nodes,node_color='white',edgecolors='white', node_size=20)

            
        

pos=nx.get_node_attributes(G,'pos')
nx.draw_networkx_nodes(G,pos,node_color='white',edgecolors='black', node_size=20)
nx.draw_networkx_nodes(G,pos,nodelist=dead_nodes,node_color='red',edgecolors='black', node_size=20)
nx.draw_networkx_edges(G,pos,edgelist=di_edges,arrowstyle='-')
nx.draw_networkx_edges(G,pos, edgelist=n_edges, arrowstyle='->')
nx.draw_networkx_edges(G,pos,edgelist=dead_edges,edge_color='red')
dead_cleaner()
plt.savefig(city_name+'.png')