class Z:
    def say(self):
        print("I am Z")

class X(Z):
    def say(self):
        print("I am X")

class Y(Z):
    pass

class B(Y, X):
    def say(self):
        print("I am B")

class A(Z):
    def say(self):
        print("I am A")

class C(B, A):
    pass

c = C()
c.say()
print(C.mro())

c = Z()
c.say()
print(Z.mro())
