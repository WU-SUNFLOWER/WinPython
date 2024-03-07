def f():
    x = 1
    def g():
        x = 2
        return x
        
g = f()
print g()