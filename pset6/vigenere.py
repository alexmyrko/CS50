from cs50 import get_string
import array
import sys

def main():
    if len(sys.argv) > 2:
        sys.exit(1)
    key = sys.argv[1]
    keylength = len(key)
    for i in range (keylength):
        if not(key[i].islower() or key[i].isupper()):
            sys.exit(1)

    text = get_string("Plain text: ")
    l = len(text)
    res = []
    j = 0
    for i in range (l):
        if text[i].isupper():
            res.append((ord(text[i]) + shift(key[j%keylength]) - 65)%26 + 65)
            j+=1
        elif text[i].islower():
            res.append((ord(text[i]) + shift(key[j%keylength]) - 97)%26 + 97)
            j+=1
        else:
            res.append(ord(text[i]))

    print("ciphertext: ")
    for i in range (l):
        print(f"{chr(res[i])}", end = "")
    print()

def shift(t):
    if t.isupper():
        return ord(t) - ord('A')
    else:
        return ord(t) - ord('a')

if __name__ == "__main__":
    main()