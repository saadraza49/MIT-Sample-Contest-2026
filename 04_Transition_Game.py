import sys
input = sys.stdin.readline

def solve():
    n = int(input())
    A = list(map(int, input().split()))
    
    # To find all nodes on cycles in functional graph
    # For coloring: 0 = unvisited, 1 = in progress, 2 = done
    color = [0] * (n + 1)
    on_cycle = [False] * (n + 1)
    
    for start in range(1, n + 1):
        if color[start] == 0:
            path = []
            node = start
            while color[node] == 0:
                color[node] = 1
                path.append(node)
                node = A[node - 1]
            
            if color[node] == 1:
                # Found a cycle - mark all cycle nodes
                idx = path.index(node)
                for c in path[idx:]:
                    on_cycle[c] = True
            
            for p in path:
                color[p] = 2
    
    print(sum(on_cycle[1:n+1]))

solve()