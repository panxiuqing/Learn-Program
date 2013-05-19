# -*_ coding: utf-8 -*-

from data import data

mini_sup_rate = 0.5

#从l[i]生成c[i+1],并把相同的滤去
def links(l):
    x = []
    set_len = len(l);
    item_len = len(l[0])
    for i in range(set_len):
        for j in range(i, set_len):
            cur_set = l[i] | l[j]
            if len(cur_set) == item_len + 1 and cur_set not in x:
                x.append(cur_set)
    return x

#算法主函数
def apr():
    mini_sup = len(data) * mini_sup_rate
    sets = data.values() #集合列表

    tmpset = set([])
    L = [[]] #频繁项集列表l
    for i in data.values():
        tmpset |= i
    L[0] = filter(lambda x: len(filter(lambda y: x <= y, sets)) >= mini_sup, [set([i]) for i in tmpset]) #频繁单项集

    j = 0
    while L[j] != []:
        L.append(filter(lambda x: len(filter(lambda y: x <= y, sets)) >= mini_sup,
            filter(lambda p: len(filter(lambda q: q <= p, L[j])) == j+2, links(L[j])))) #频繁j+1项集
        j += 1
    return L

for i in apr()[:-1]:
    print i
