class A:
    x = 111
    
class B(A):
    pass
    
b = B()
print(b.x)
A.x = 222
print(b.x)
B.x = 333
print(b.x)
b.x = 444
print(b.x)