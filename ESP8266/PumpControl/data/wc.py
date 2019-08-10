#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from collections import Counter
with open('prox.data') as f1,open('y.txt','w') as f2:
    c=Counter(x.strip() for x in f1)
    #for word, count in c.most_common(20):
    for word, count in sorted(c.items()):
        #print x,c[x]   #do f2.write() here if you want to write them to f2
        print("{0}: {1}".format(word, count))