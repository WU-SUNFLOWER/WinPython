l = []
size = 1000
i = size

while i >= 0:
    l.append(i)
    i = i - 1

i = 0
while i < size:
    j = 0
    while j < (size - i):
        if l[j] > l[j + 1]:
            t = l[j]
            l[j] = l[j + 1]
            l[j + 1] = t
        j += 1
    i += 1

print(l)