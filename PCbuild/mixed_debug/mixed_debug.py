import os
import sys

def test():
    print "haha"

test()
print os.getcwd()
print sys.getsizeof({i : i for i in xrange(22)});
