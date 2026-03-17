#include <iostream>
#include <vector>
using namespace std;
const int MOD = 998244353;
const int MAXN = 3005;
long long fact[MAXN], invFact[MAXN];

long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}


long long modInverse(long long n) {
    return power(n, MOD - 2);
}

void precompute() {
    fact[0] = 1;
    invFact[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    invFact[MAXN - 1] = modInverse(fact[MAXN - 1]);
    for (int i = MAXN - 2; i >= 1; i--) {
        invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
    }
}


long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

int N;
vector<int> adj[MAXN];
int color[MAXN];
int parent_node[MAXN];
vector<int> order;

    parent_node[u] = p;
    for (int v : adj[u]) {
        if (v != p) {
            dfs_setup(v, u, 1 - c);
        }
    }
    order.push_back(u); 
}


long long count_ways(int orientation) {
    vector<vector<long long>> dp(N + 1);
    vector<int> sz(N + 1, 1);
    
    // Base case: Size 1 subtree, 1 way to assign relative rank 1
    for (int i = 1; i <= N; i++) {
        dp[i] = {0, 1}; 
    }
    
    for (int u : order) {
        int p = parent_node[u];
        if (p == 0) continue;
        
        int sz_p = sz[p];
        int sz_u = sz[u];
        bool p_to_u = (color[p] == orientation);
        
        vector<long long> B(sz_u + 1, 0);
        
        if (p_to_u) {
            long long curr = 0;
            for (int j = sz_u; j >= 0; j--) {
                B[j] = curr * nCr(sz_u, j) % MOD;
                curr = (curr + dp[u][j]) % MOD;
            }
        } else {
            long long curr = 0;
            for (int j = 0; j <= sz_u; j++) {
                curr = (curr + dp[u][j]) % MOD;
                B[j] = curr * nCr(sz_u, j) % MOD;
            }
        }
        
        vector<long long> A(sz_p + 1, 0);
        for (int i = 1; i <= sz_p; i++) {
            A[i] = dp[p][i] * nCr(sz_p - 1, i - 1) % MOD;
        }
        
        // Decoupled convolution
        int S_total = sz_p + sz_u;
        vector<long long> C_poly(S_total + 1, 0);
        
        for (int i = 1; i <= sz_p; i++) {
            if (!A[i]) continue;
            for (int k = 0; k <= sz_u; k++) {
                if (B[k]) {
                    C_poly[i + k] = (C_poly[i + k] + A[i] * B[k]) % MOD;
                }
            }
        }
        
        // Rescale mapped permutations
        vector<long long> dp_new(S_total + 1, 0);
        long long factor = nCr(S_total - 1, sz_p - 1);
        
        for (int x = 1; x <= S_total; x++) {
            if (!C_poly[x]) continue;
            long long inv_nCr = invFact[S_total - 1] * fact[x - 1] % MOD * fact[S_total - 1 - (x - 1)] % MOD;
            dp_new[x] = C_poly[x] * factor % MOD * inv_nCr % MOD;
        }
        
        dp[p] = std::move(dp_new);
        sz[p] += sz_u;
    }
    
    long long total = 0;
    for (long long val : dp[1]) {
        total = (total + val) % MOD;
    }
    return total;
}

void solve() {
    if (!(cin >> N)) return;
    
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    precompute();
    
    // Root the tree at 1, setup parent relations and initial colors
    dfs_setup(1, 0, 0);
    
    // Check both orientation paths to respect the alternating condition
    long long res1 = count_ways(0);
    long long res2 = count_ways(1);
    
    cout << (res1 + res2) % MOD << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    solve();
    
    return 0;
}