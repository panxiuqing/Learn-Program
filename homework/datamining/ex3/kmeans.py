# -*- coding: utf-8 -*-

import numpy as np
import random

tmp = np.loadtxt('data.txt', dtype=np.str, delimiter=' ') #载入文件
data = tmp[:, :-1].astype(np.float) #转换为float

n = len(data) #对象数
v = len(data[0]) #维数

ks = [5, 10, 20] #簇数列表

def dis(x, y): #两个对象间的距离
    return np.sum(np.power(x-y, 2))

def ave(l): #均值
    return sum([data[i] for i in l])/len(l)

for k in ks:
    init_core = random.sample(range(n), k) #随机k个初始中心
    C = [{'means': data[c], 'list': []} for c in init_core] #簇列表，包含均值和簇内对象列表
    mp = {} #对象和所在簇号的映射

    #第一遍聚类
    for i in range(n):
        dis_list = [dis(data[i], dic['means']) for dic in C]
        mini = min(dis_list)
        mp[i] = dis_list.index(mini)
        C[mp[i]]['list'].append(i)

    cycle = True
    while cycle: #直到对象不在簇间转移
        cycle = False
        for dic in C: #更新簇均值
            dic['means'] = ave(dic['list'])
        for i in range(n):
            dis_list = [dis(data[i], dic['means']) for dic in C]
            mini = min(dis_list)
            if mp[i] != dis_list.index(mini): #如果对象所在簇发生转移
                C[mp[i]]['list'].remove(i) #从原先簇中移除
                mp[i] = dis_list.index(mini) #修改映射
                C[mp[i]]['list'].append(i) #加入现簇中
                cycle = True

    print "\nK = %d" % k
    for i in C:
        print "个数: %d\n簇内对象:" % len(i['list']), i['list']

