#!/usr/bin/env python

"""Microbenchmark for method call overhead.

This measures simple method calls that are predictable, do not use varargs or
kwargs, and do not use tuple unpacking.
"""

# Python imports
import optparse
import time

# Local imports
import util
from compat import xrange


class Foo(object):

    def foo(self, a, b, c, d):
        # 20 calls
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)
        self.bar(a, b, c)

    def bar(self, a, b, c):
        return
        # 20 calls
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)
        self.baz(a, b)

    def baz(self, a, b):
        # 20 calls
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)
        self.quux(a)

    def quux(self, a):
        # 20 calls
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()
        self.qux()

    def qux(self):
        pass


def test_calls(iterations, timer):
    times = []
    f = Foo()
    for _ in xrange(iterations):
        t0 = timer()
        # 20 calls
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        f.foo(1, 2, 3, 4)
        t1 = timer()
        times.append(t1 - t0)
    return times


print 'entering main'
print __name__

def run_benchmark(options, num_runs, bench_func, *args):
    timer = getattr(time, options.timer)
    data = bench_func(num_runs, timer)
    if options.take_geo_mean:
        product = reduce(operator.mul, data, 1)
        print_(math.pow(product, 1.0 / len(data)))
    else:
        for x in data:
            print x

import falcon

def ggg(x):
    print x
    return x + 1

#@falcon.wrap
def fff(func, *args):
    x = h(1, 2, 3, 4, 4, 4, 4, d=5, e=6, f=7)
    y = func(*args)
    print args
    return x + y

def h(a, b, c, *args, **kwargs):
    print a, b, c, args, kwargs
    return a + b * c + len(args) + len(kwargs)

if __name__ == "__main__":
    falcon.evaluator.start();
    #falcon.dis(h)
    #falcon.dis(fff)
    #print fff(ggg, 123)
    
    
    print 'in main'
    parser = optparse.OptionParser(
        usage="%prog [options] [test]",
        description=("Test the performance of simple Python-to-Python method"
                     " calls."))
    util.add_standard_options_to(parser)
    options, _ = parser.parse_args()

    # Priming run.
    # test_calls(1, time.time)

    run_benchmark(options, options.num_runs, test_calls)
