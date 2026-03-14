Q = int(input())
for _ in range(Q):
    s = input()
    n = len(s)
    found = False
    # A + B + B, len = a + 2b, a>=1, b>=1
    for b in range(1, n // 2 + 1):
        a = n - 2 * b
        if a < 1:
            continue
        # s[0:a] = A, s[a:a+b] = B, s[a+b:a+2b] = B
        if s[a:a+b] == s[a+b:a+2*b]:
            found = True
            break
    print("YES" if found else "NO")