# this script is for drawing .osf file

import matplotlib.pyplot as plt
import sys

def read_osf_file(file_name):
    mode = 0
    points = []
    bnds = []
    nodes = []
    edges = []
    with open(file_name, 'r') as f:
        for line in f:
            line = line.split()
            if line[0] == '#':
                mode = 0
                continue
            elif line[0] == '[BOUNDARY_POINT_BEGIN]':
                mode = 1
                continue
            elif line[0] == '[BOUNDARY_LINE_BEGIN]':
                mode = 2
                continue
            elif line[0] == '[ISHOCK_NODE_BEGIN]':
                mode = 3
                continue
            elif line[0] == '[ISHOCK_EDGE_BEGIN]':
                mode = 4
                continue
            else:
                pass

            if mode == 0:
                continue
            elif mode == 1:
                point = {}
                point['id'] = int(line[0])
                pts = []
                pt = (float(line[4]), float(line[5]))
                pts.append(pt)
                point['pts'] = pts
                points.append(point)
            elif mode == 2:
                bnd = {}
                bnd['id'] = int(line[0])
                pts = []
                for point in points:
                	if point['id']==int(line[2]): 
                        	pt = point['pts'][0]
                                pts.append(pt)
                        else:
				continue
                for point in points:
                	if point['id']==int(line[3]): 
                        	pt = point['pts'][0]
                                pts.append(pt)
                        else:
				continue
                bnd['pts'] = pts
                #print(pts)
                bnds.append(bnd)
            elif mode == 3:
                node = {}
                node['id'] = int(line[0])
                pt = (float(line[1]), float(line[2]))
                node['pt'] = pt
                bnd_ids = []
                for i in range(14, len(line)):
                    bnd_ids.append(int(line[i]))
                node['bnd_ids'] = bnd_ids
                nodes.append(node)
            elif mode == 4:
                edge = {}
                edge['id'] = int(line[0])
                edge['from'] = int(line[17])
                edge['to'] = int(line[18])
                edge['left'] = int(line[19])
                edge['right'] = int(line[20])
                edges.append(edge)
            else:
                pass

    return bnds, nodes, edges

def draw_ishock_graph(bnds, nodes, edges):
    for bnd in bnds:
        if len(bnd['pts']) == 2:
            line = bnd['pts']
            #print(line)
            #line_xs, line_ys= zip(*line)
            #print(line[0])
            plt.plot([line[0][0],line[1][0]], [line[0][1],line[1][1]], color='blue')
            plt.axis('equal')
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
    bnds, nodes, edges = read_osf_file(file_name)
    draw_ishock_graph(bnds, nodes, edges)

