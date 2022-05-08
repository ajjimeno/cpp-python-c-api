#include <Python.h>

#if PY_MAJOR_VERSION >= 3
#define PY3K
#endif

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <cmath>

class RunnerSimulator
{
public:    
    RunnerSimulator(unsigned int inMaxMoves);
    
    void turnLeft(void);
    void turnRight(void);
    void moveForward(void);
    void run(PyObject* inWrappedFunc);
    void reset(void);
    int getMMaxMoves() { return mMaxMoves; }
private:
    unsigned int mMaxMoves;       //!< Maximum number of moves allowed.
    unsigned int nMoves;          //!< Current number of moves
    unsigned int position;
};