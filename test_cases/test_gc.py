class A:
    def __init__(self):
        #print("AAAAA")
        x = self

# i = 0x7FFFFFFF
i = 10000
while i > 0:
    i = i - 1
    x = A()
print(i)
