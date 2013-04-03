data = {"T1": set(["I1", "I2", "I3", "I5"]),
        "T2": set(["I2", "I4"]),
        "T3": set(["I2", "I3"]),
        "T4": set(["I1", "I2", "I4"]),
        "T5": set(["I1", "I3"]),
        "T6": set(["I1", "I2", "I3"])}

mini_sup_rate = 0.5

def count(item_set, data):
    c = 0
    for i in data.values():
        if item_set <= i:
            c += 1
    return c

def links(l):
    x = []
    for i in range(len(l)):
        for j in range(i, len(l)):
            if len(l[i] & l[j]) == len(l[i]) - 1:
                x.append(l[i] | l[j])
    return x

def apr():
    all_trans = len(data)
    mini_sup = all_trans * mini_sup_rate

    tmpset = set([])
    L = [[]]
    for i in data.values():
        tmpset |= i
    for i in tmpset:
        L[0].append(set([i]))
    L.append([])
    for i in L[0]:
        if count(i, data) >= mini_sup:
            L[1].append(i)
    j = 1
    while L[j] != []:
        L.append([])
        for i in links(L[j]):
            #C[0][i] = count(i, data)
            if count(i, data) >= mini_sup:
                L[j+1].append(i)
        j += 1
    return L

for i in apr():
    print i
