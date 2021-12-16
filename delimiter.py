import random

class Delimiter:
    def __init__(self, key):
        self.key = key
        self.filter_size = 2

    def split(self):
        combos = []
        for i in range(len(self.key) - (2*self.filter_size + 1)): # gets how many times the first delimiter filter is allowed to move
            a = self.key[:i+1]
            for j in range(len(self.key) - (i+1 + 2*self.filter_size)):
                b = self.key[i+self.filter_size+1:i+self.filter_size+j+2]
                c = self.key[i+2*self.filter_size+j+2:]
                combos.append((a, b, c))
        return combos

    def __str__(self):
        return self.key


key = ''.join([chr(random.randint(32,126)) for i in range(500)])
d = Delimiter('heeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeellooo')
print('Python')
res = d.split()