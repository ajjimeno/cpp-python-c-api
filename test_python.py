from deap import gp

def partial(func, *args):
    def wrapper():
        return func(*args)

    return wrapper

class Test:
    def __init__(self):
        self.position = 0
    
    def turn_left(self):
        return partial(self._turn_left)

    def _turn_left(self):
        self.position -= 1

    def turn_right(self):
        return partial(self._turn_right)

    def _turn_right(self):
        self.position += 1

    def run(self,routine):
        routine()

test = Test()

class Operation(object):
    pass

pset = gp.PrimitiveSetTyped("MAIN", [], Operation)
pset.addPrimitive(test.turn_left, [], Operation)
pset.addPrimitive(test.turn_right, [], Operation)

program1="turn_left()"
program2="turn_left()"

f=eval(program1, pset.context)

for i in range(100000000):
    test.run(f)