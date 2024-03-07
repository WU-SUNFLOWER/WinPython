def foo(x = 5):
    y = 10
    def say():
        print(x + y)
    x = 1
    print x
    return say

say = foo()
say()