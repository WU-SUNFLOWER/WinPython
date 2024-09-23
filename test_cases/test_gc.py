class A:
    def __init__(self):
        #print("AAAAA")
        x = self

i = 0xFFFF
while i > 0:
    i = i - 1
    x = A()
print(i)