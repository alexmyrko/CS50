from cs50 import get_int

while True:
    n = get_int("Height: ")
    if n>0 and n<9:
        break

for i in range (n):
    for j in range (n-i-1):
        print(f" ", end = "")
    for j in range (n-i-1,  n):
        print(f"#", end = "")
    print("  ", end = "")
    for j in range (n-i-1, n):
        print(f"#", end = "")
    print()