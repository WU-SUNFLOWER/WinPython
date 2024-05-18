def outer(n):
    def inner():
        print(n)
    return inner
    
i = 0
func_list = []
while i < 10:
    f = outer(i)
    func_list.append(f)
    i += 1
    
for f in func_list:
    f()