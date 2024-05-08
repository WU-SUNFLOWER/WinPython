def outer(n):
    def inner():
        print(n)
    return inner
    
i = 0
while i < 10:
    f = outer(i)
    f()
    i += 1