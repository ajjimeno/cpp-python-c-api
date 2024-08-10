from distutils.core import setup, Extension
import os

os.environ["CFLAGS"] = "-O3 -march=native"
os.environ["LDFLAGS"] = "-O3 -march=native"

module1 = Extension(
    "Simulator",
    sources=["Program.cpp"],
    extra_compile_args=["-g", "-O3", "-march=native"],
)

setup(
    name="Simulator",
    version="1.0",
    description="Fast version of the ARC simulator",
    ext_modules=[module1],
)
