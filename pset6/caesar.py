from cs50 import get_string
import array
import sys
s = sys.argv[1]

for i in range (len(s)):
    if ord(s[i]) < 48 or ord(s[i]) > 57:
        break
    else:
        key = int(s)

text = get_string("Plain text: ")
l = len(text)
res = []
for i in range (l):
    if 64 < ord(text[i]) and ord(text[i]) < 90:
        res.append((ord(text[i]) + key - 65)%26 + 65)
    elif 96 < ord(text[i]) and ord(text[i]) < 122:
        res.append((ord(text[i]) + key - 97)%26 + 97)
    else:
        res.append(ord(text[i]))

print("ciphertext: ")
for i in range (l):
    print(f"{chr(res[i])}", end = "")
print()