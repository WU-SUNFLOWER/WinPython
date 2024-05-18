class A:
    value = 2024
    def __init__(self):
        print("A")
        
class B1(A):
    def __init__(self):
        print("B1")
        super().__init__();
        
class B2(A):
    def __init__(self):
        print("B2")
        super().__init__();
        
class C(B1, B2):
    def __init__(self):
        print("C")
        super().__init__()
    def foo(self):
        print(super().value)
    
c = C()
print(C.mro())
print(isinstance(c, C))
print(isinstance(c, B1))
print(isinstance(c, B2))
print(isinstance(c, A))
print(isinstance(c, object))
print(isinstance(c, dict))
c.foo()