# created by Wenhan
# this script is for drawing .isf file

import matplotlib.pyplot as plt
import sys

def read_isf_file(file_name):
    mode = 0
    bnds = []
    nodes = []
    edges = []
    with open(file_name, 'r') as f:
        for line in f:
            line = line.split()
            if line[0] == '#':
                mode = 0
                continue
            elif line[0] == '[BOUNDARY_BEGIN]':
                mode = 1
                continue
            elif line[0] == '[ISHOCK_NODE_BEGIN]':
                mode = 2
                continue
            elif line[0] == '[ISHOCK_EDGE_BEGIN]':
                mode = 3
                continue
            else:
                pass

            if mode == 0:
                continue
            elif mode == 1:
                bnd = {}
                bnd['id'] = int(line[0])
                pts = []
                for i in range(2, len(line), 2):
                    pt = (float(line[i]), float(line[i+1]))
                    pts.append(pt)
                bnd['pts'] = pts
                bnds.append(bnd)
            elif mode == 2:
                node = {}
                node['id'] = int(line[0])
                pt = (float(line[1]), float(line[2]))
                node['pt'] = pt
                bnd_ids = []
                for i in range(3, len(line)):
                    bnd_ids.append(int(line[i]))
                node['bnd_ids'] = bnd_ids
                nodes.append(node)
            elif mode == 3:
                edge = {}
                edge['id'] = int(line[0])
                edge['from'] = int(line[2])
                edge['to'] = int(line[3])
                edge['left'] = int(line[4])
                edge['right'] = int(line[4])
                edges.append(edge)
            else:
                pass

    return bnds, nodes, edges

def draw_ishock_graph(bnds, nodes, edges):
    for bnd in bnds:
        if len(bnd['pts']) == 2:
            line = bnd['pts']
            (line_xs, line_ys) = zip(*line)
            plt.plot(line_xs, line_ys, color='blue')

    for edge in edges:
        from_id = edge['from']
        to_id = edge['to']

        if from_id != -1 and to_id != -1:
            from_pt = get_pt(nodes, from_id)
            to_pt = get_pt(nodes, to_id)
            plt.plot([from_pt[0], to_pt[0]], [from_pt[1], to_pt[1]], color='red')

    for node in nodes:
        pt = node['pt']
        plt.plot(pt[0], pt[1], 'x',  color='green')
    print("Showing ishock graph ...")
    plt.show()

def get_pt(nodes, id):
    for node in nodes:
        if id == node['id']:
            return node['pt']
    print("ERROR: NO SUCH NODE!")
    return -1, -1


if __name__ == '__main__':
    file_name = sys.argv[1]
    bnds, nodes, edges = read_isf_file(file_name)
    draw_ishock_graph(bnds, nodes, edges)