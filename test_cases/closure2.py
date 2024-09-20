def foo(x):
    print(x)
    def bar():
        print(x)
    return bar
foo("Hello World")