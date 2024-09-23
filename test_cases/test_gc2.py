def foo(n):
    d = {}
    d['x'] = n
    if n < 20:
        d['y'] = foo(n + 1)
        d['z'] = foo(n + 2)
    return d
    
print(foo(0))