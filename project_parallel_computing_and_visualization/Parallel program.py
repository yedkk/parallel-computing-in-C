#author: Kangdong Yuan

#import packages
import os
os.environ["PATH"] += os.pathsep + 'D:/Program Files (x86)/Graphviz2.38/bin/'
from dask import delayed
from typing import List
import time
from dask.distributed import Client, wait, progress, as_completed
import timeit

# add function

def addthem(x: List[float]) -> float:
    """ adds the elements in the list x """
    return sum(x)

# increment function

def increment(x: float) -> float:
    """ adds 1 to x """
    return x+1

# do the prefix sum
def go(myarray: List[float], branch_factor: int):
    added=[]
    added1=[]
    for i in myarray:
        added.append(c.submit(increment,i))
    n=branch_factor
    while len(added) > n:
        s = len(added)//n
        for i in range(s+1):
            added1.append(c.submit(addthem, added[i*n:(i+1)*n]))
        added=added1
        added1=[]
    else:
        return c.submit(addthem, added)





if __name__ == "__main__":
    #asynchronous = True, n_workers = 1000, threads_per_worker = 1
    c = Client(n_workers = 20, threads_per_worker = 1)
    import random
    randomlist = []
    for i in range(0, 10):
        n = random.randint(1, 9)
        randomlist.append(n)
    start = timeit.default_timer()
    delayed_object = go(randomlist, 2)
    print(delayed_object.result())
    end = timeit.default_timer()
    delayed_object.visualize("tree2.png")
    print(abs(end-start))



