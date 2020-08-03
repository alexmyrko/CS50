from cs50 import get_string
from sys import argv
import sys


def main():

    if len(sys.argv)!=2:
        print("Usage: python bleep.py dictionary")
        sys.exit(1)

    filename = argv[1]
    print(filename)
    words = set()
    file = open(filename, "r")
    for line in file:
        words.add(line.rstrip("\n"))

    text = get_string("Enter text: ")
    list = text.split()

    j = 0
    inside = False
    for s in list:
        inside = False
        for word in words:
            if s.lower() == word.lower():
                print("*"*len(s), end = " ")
                inside = True
                break
        if inside == False:
            print(s, end = " ")
    print()

if __name__ == "__main__":
    main()
