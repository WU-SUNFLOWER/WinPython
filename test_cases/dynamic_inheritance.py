class A:
    x = 123
    
class B(A):
    pass
    
b = B()
print(b.x)
A.x = 333
print(b.x)
B.x = 444
print(b.x)
b.x = 999
print(b.x)