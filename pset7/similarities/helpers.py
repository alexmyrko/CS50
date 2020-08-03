from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""

    # TODO
    list = []
    list1 = a.splitlines()
    list2 = b.splitlines()
    for l1 in list1:
        for l2 in list2:
            if l1 == l2 and l1 not in list:
                list.append(l1)
    #print(list)
    return list


def sentences(a, b):
    """Return sentences in both a and b"""

    # TODO
    list1 = sent_tokenize(a)
    list2 = sent_tokenize(b)
    list = []
    for l1 in list1:
        for l2 in list2:
            if l1 == l2 and l1 not in list:
                list.append(l1)
    return list


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # TODO
    list1 = []
    list2 = []
    list = []

    for i in range (len(a)-n+1):
        s = a[i:i+n]
        if s not in list1:
            list1.append(s)

    for i in range (len(b)-n+1):
        s = b[i:i+n]
        if s not in list2:
            list2.append(s)

    for l1 in list1:
        for l2 in list2:
            if l1 == l2 and l1 not in list:
                list.append(l1)

    return list