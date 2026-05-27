import sys
from fractions import Fraction as fr 
import numpy as np 

def readfile(path):
    with open(path, "r") as f:
        return eval(f.read())       

# check arguments
if len(sys.argv) < 2 or sys.argv[1] not in ['E', 'X']:
    print("Usage: python script.py [E|X]")
    sys.exit(1)


M = readfile("./M.txt")
#V = readfile("./V.txt")

target = sys.argv[1]
if target == 'E':
    c = readfile("./ce.txt")
    s = np.matmul(c, M) 
    print(s[0])
else:
    c = readfile("./cx.txt")
    s = np.matmul(c, M)
    print(s[7]) 

print(f"count of negative elements in s{target.lower()}: {len([x for x in s if x < 0])}")