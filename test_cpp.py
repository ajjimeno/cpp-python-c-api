from RunnerC import Runner
from deap import gp

class Test:
    def __init__(self):
        self.runner = Runner(1)
    
    def turn_left(self):
        return lambda : self.runner.turn_left()

    def turn_right(self):
        return lambda : self.runner.turn_right()
   
    def run(self, f):
        self.runner.run(f)

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