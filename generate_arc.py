from copy import deepcopy
import glob
import itertools
import json
import numpy as np


def get_arc():
    es = []

    for file in glob.glob("/Users/E114560/Documents/research/data/arc/training/*.json"):
        with open(file, 'r') as f:
            example = json.load(f)

            e = {}

            e['training'] = []
            #print(example['train']['input'], example['train']['output'])
            for i in range(len(example['train'])):
                e['training'].append([ example['train'][i]['input'], example['train'][i]['output'] ])

            e['testing'] = [[example['test'][0]['input'], example['test'][0]['output']]]

            es.append(e)

    return es

def print_matrix(m, f):
    print("{} {}".format(len(m), len(m[0])), file=f)

    sm = ""
    for r in m:
        line = ""
        for c in r:
            line+=str(c) + " "
        sm += line.strip() + "\n"
    print (sm, end="", file=f)

def write_arc(data):
    path = "/Users/E114560/Documents/research/arc-runner/data"

    count = 0
    for d in data:
        with open(path + "/" + str(count) + ".txt", 'w') as f:
            # Number of training examples
            print (len(d["training"]), file=f)

            for i in d["training"]:
                print_matrix(i[0], f)
                print_matrix(i[1], f)
                pass
            for i in d["testing"]:
                print_matrix(i[0], f)
                print_matrix(i[1], f)

                ll = []
                for lll in i[0]:
                    ll.extend(lll)

                mf = np.bincount(ll).argmax()
                c = deepcopy(i[1])
                
                for ii in range(len(c)):
                    for ij in range(len(c[0])):
                        c[ii][ij] = mf

                print_matrix(c, f)


        count +=1

if __name__ == "__main__":
    write_arc(get_arc())


