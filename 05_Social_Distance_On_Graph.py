import heapq, sys
from collections import deque

def solve():
    data = sys.stdin.buffer.read().split()
    idx = 0
    N = int(data[idx]); idx += 1
    M = int(data[idx]); idx += 1

    edges = []
    for _ in range(M):
        a = int(data[idx]) - 1; idx += 1
        b = int(data[idx]) - 1; idx += 1
        w = int(data[idx]); idx += 1
        edges.append((w, a, b))
    edges.sort()

    par = list(range(N))
    rnk = [0] * N
    rel = [0] * N  # parity relative to parent in DSU tree

    def find(x):
        path = []
        while par[x] != x:
            path.append(x); x = par[x]
        root = x; p = 0
        for node in reversed(path):
            p ^= rel[node]; rel[node] = p; par[node] = root
        return root, (rel[path[0]] if path else 0)

    W_star = float('inf')
    g = [[] for _ in range(N)]

    for w, a, b in edges:
        ra, ca = find(a)
        rb, cb = find(b)
        if ra == rb:
            if ca == cb: W_star = w; break
        else:
            if rnk[ra] < rnk[rb]: ra, rb = rb, ra; ca, cb = cb, ca
            par[rb] = ra; rel[rb] = ca ^ cb ^ 1
            if rnk[ra] == rnk[rb]: rnk[ra] += 1
        g[a].append((b, w)); g[b].append((a, w))

    parity = [-1] * N
    for s in range(N):
        if parity[s] == -1:
            parity[s] = 0; q = deque([s])
            while q:
                u = q.popleft()
                for v, _ in g[u]:
                    if parity[v] == -1:
                        parity[v] = 1 - parity[u]; q.append(v)

    INF = float('inf')

    def min_same(tgt):
        d = [INF] * N; src = [-1] * N; hp = []
        for v in range(N):
            if parity[v] == tgt:
                d[v] = 0; src[v] = v; heapq.heappush(hp, (0, v))
        while hp:
            dv, u = heapq.heappop(hp)
            if dv > d[u]: continue
            for v, w in g[u]:
                if dv + w < d[v]:
                    d[v] = dv + w; src[v] = src[u]; heapq.heappush(hp, (d[v], v))
        res = INF
        for u in range(N):
            for v, w in g[u]:
                if u < v and src[u] != src[v] and src[u] != -1 and src[v] != -1:
                    res = min(res, d[u] + w + d[v])
        return res

    print(min(W_star, min_same(0), min_same(1)))

solve()