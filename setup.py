from distutils.core import setup, Extension

module1 = Extension('RunnerC',
                    sources = ['RunnerSimulator.cpp'])

setup (name = 'RunnerC',
       version = '1.0',
       description = 'Fast version of the ARC runner',
       ext_modules = [module1]) 