from distutils.core import setup, Extension

module1 = Extension('Simulator',
                    sources = ['Program.cpp'],
                    extra_compile_args = ["-O2"])

setup (name = 'Simulator',
       version = '1.0',
       description = 'Fast version of the ARC simulator',
       ext_modules = [module1]) 