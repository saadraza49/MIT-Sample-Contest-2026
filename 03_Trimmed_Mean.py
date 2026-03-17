# Trimmed mean
n = int(input())
grades = sorted(map(int, input().split()))
trimmed = grades[n:4*n]
print(sum(trimmed) / (3*n))