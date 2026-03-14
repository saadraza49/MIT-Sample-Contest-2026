#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, t;
    bool operator<(const Edge& other) const {
        return t > other.t; // Sort descending
    }
};

const int MOD = 1e9 + 7;

int N;
int T[405][405];
vector<Edge> edges;

int parent_dsu[805];
int find_set(int v) {
    if (v == parent_dsu[v]) return v;
    return parent_dsu[v] = find_set(parent_dsu[v]);
}

int parent_tree[805];
int left_child[805], right_child[805];
int depth[805];
int up[805][11];
int node_cnt;

void dfs(int u, int p, int d) {
    parent_tree[u] = p;
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i <= 10; ++i) {
        up[u][i] = up[up[u][i - 1]][i - 1];
    }
    if (u > N) {
        dfs(left_child[u], u, d + 1);
        dfs(right_child[u], u, d + 1);
    }
}

int get_lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    int diff = depth[u] - depth[v];
    for (int i = 0; i <= 10; ++i) {
        if ((diff >> i) & 1) u = up[u][i];
    }
    if (u == v) return u;
    for (int i = 10; i >= 0; --i) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    return up[u][0];
}

bool in_left_subtree(int leaf, int ancestor) {
    if (leaf == ancestor) return false;
    int curr = leaf;
    for (int i = 10; i >= 0; --i) {
        if (depth[curr] - (1 << i) > depth[ancestor]) {
            curr = up[curr][i];
        }
    }
    return curr == left_child[ancestor];
}

vector<pair<int, int>> adj[805];
int color[805];
bool possible = true;

void dfs_color(int u, int c) {
    color[u] = c;
    for (auto edge : adj[u]) {
        int v = edge.first;
        int w = edge.second;
        if (color[v] == -1) {
            dfs_color(v, c ^ w);
        } else if (color[v] != (c ^ w)) {
            possible = false;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    int total_edges = N * (N - 1) / 2;
    edges.resize(total_edges);
    for (int i = 0; i < total_edges; ++i) {
        cin >> edges[i].u >> edges[i].v;
        edges[i].t = i + 1;
        T[edges[i].u][edges[i].v] = i + 1;
        T[edges[i].v][edges[i].u] = i + 1;
    }

    sort(edges.begin(), edges.end());

    for (int i = 1; i <= 2 * N; ++i) {
        parent_dsu[i] = i;
    }

    node_cnt = N;
    for (const auto& e : edges) {
        int u = find_set(e.u);
        int v = find_set(e.v);
        if (u != v) {
            node_cnt++;
            left_child[node_cnt] = u;
            right_child[node_cnt] = v;
            parent_dsu[u] = node_cnt;
            parent_dsu[v] = node_cnt;
        }
    }

    dfs(node_cnt, node_cnt, 0);

    for (int i = 1; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            for (int k = j + 1; k <= N; ++k) {
                int t1 = T[i][j], t2 = T[j][k], t3 = T[i][k];
                int maxt = max({t1, t2, t3});
                int x, y, z;
                
                if (maxt == t1) { x = i; y = j; z = k; }
                else if (maxt == t2) { x = j; y = k; z = i; }
                else { x = i; y = k; z = j; }

                int W = get_lca(x, get_lca(y, z));
                int lca_xy = get_lca(x, y);

                if (depth[lca_xy] > depth[W]) continue;

                int lca_xz = get_lca(x, z);
                int lca_yz = get_lca(y, z);

                if (depth[lca_xz] > depth[W]) {
                    int U = lca_xz;
                    int pos_W = in_left_subtree(x, W) ? 1 : 0;
                    int pos_U = in_left_subtree(x, U) ? 1 : 0;
                    int relation = (pos_W == pos_U) ? 1 : 0;
                    adj[W].push_back({U, relation});
                    adj[U].push_back({W, relation});
                } else if (depth[lca_yz] > depth[W]) {
                    int U = lca_yz;
                    int pos_W = in_left_subtree(y, W) ? 1 : 0;
                    int pos_U = in_left_subtree(y, U) ? 1 : 0;
                    int relation = (pos_W == pos_U) ? 1 : 0;
                    adj[W].push_back({U, relation});
                    adj[U].push_back({W, relation});
                }
            }
        }
    }

    for (int i = N + 1; i <= node_cnt; ++i) {
        color[i] = -1;
    }

    int components = 0;
    for (int i = N + 1; i <= node_cnt; ++i) {
        if (color[i] == -1) {
            components++;
            dfs_color(i, 0);
        }
    }

    if (!possible) {
        cout << 0 << "\n";
    } else {
        long long ans = 1;
        for (int i = 0; i < components; ++i) {
            ans = (ans * 2) % MOD;
        }
        cout << ans << "\n";
    }

    return 0;
}