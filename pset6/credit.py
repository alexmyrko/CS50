from cs50 import get_int
import array as arr

bits = 0
sum1 = 0
sum2 = 0

while True:
    number = get_int("Number: ")
    if number>0:
        break

check = number
while check != 0:
    check = int(check/10)
    bits+=1

check = number
a = []
for i in range (bits):
    a.append(check - int(check/10)*10)
    check = int(check/10)
    if i%2 != 0:
        sum1 += (a[i]*2)%10 + int(a[i]*2/10)
    else:
        sum2 += a[i]

if (sum1+sum2)%10 == 0 and bits >= 13:
    begin = a[bits-1]*10 + a[bits-2]
    if a[bits-1] == 4:
        print("VISA")
    elif begin == 34 or begin == 37:
        print("AMEX")
    elif begin >= 51 and begin <= 55:
        print("MASTERCARD")
else:
    print("INVALID")