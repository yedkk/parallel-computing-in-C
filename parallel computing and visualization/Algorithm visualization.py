from dask import delayed
from typing import List
import time
from dask.distributed import Client


@delayed
def addthem(x: List[float]) -> float:
    """ adds the elements in the list x """
    time.sleep(1)
    return sum(x)


@delayed
def increment(x: float) -> float:
    """ adds 1 to x """
    time.sleep(1)
    return x+1


def go(myarray: List[float], branch_factor: int):
    added=[]
    added1=[]
    for i in myarray:
        added.append(increment(i))
    n=branch_factor
    while len(added) > n:
        s = len(added)//n
        for i in range(s+1):
            added1.append(addthem(added[i*n:(i+1)*n]))
        added=added1
        added1=[]
    else:
        return addthem(added)
0,1,2,3,4,5,6,7,8,9,10], 3)
delayed_object.visualize("tree.png") # creates
delayed_object = go([ a png image of the graph
result = delayed_object.compute() # result = 66
print(result)