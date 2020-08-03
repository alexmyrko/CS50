from cs50 import get_float

while True:
    n = get_float("Change owed: ")
    if n>=0:
        break

n = n * 100

tf = int(n/25)
ten = int((n-tf*25)/10)
five = int((n-tf*25 - ten*10)/5)
print(int(tf+ten+five+(n-tf*25-ten*10-five*5)))