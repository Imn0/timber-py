import os
import re
import subprocess
import sys
from pathlib import Path
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext


def get_timber_version():
    tmb_c_path = Path(__file__).parent / "timber-c" / "src" / "tmb.c"

    if not tmb_c_path.exists():
        print(f"Warning: {tmb_c_path} not found, using default version")
        return "0.0.0"

    with open(tmb_c_path, "r") as f:
        content = f.read()

    major = re.search(r'const\s+char\*\s+const\s+TMB_MAJOR_V\s*=\s*"(\d+)"', content)
    minor = re.search(r'const\s+char\*\s+const\s+TMB_MINOR_V\s*=\s*"(\d+)"', content)
    patch = re.search(r'const\s+char\*\s+const\s+TMB_PATCH_V\s*=\s*"(\d+)"', content)

    if major and minor and patch:
        version = f"{major.group(1)}.{minor.group(1)}.{patch.group(1)}"
        print(f"Detected timber version: {version}")
        return version
    else:
        print("Warning: Could not parse version from tmb.c, using default")
        return "0.0.0"


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        if not isinstance(ext, CMakeExtension):
            super().build_extension(ext)
            return

        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))


        build_temp = Path(self.build_temp) / ext.name
        build_temp.mkdir(parents=True, exist_ok=True)

        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={'Debug' if self.debug else 'Release'}",
            "-DBUILD_SHARED_LIBS=OFF",
            "-DBUILD_TESTS=OFF",
            "-DBUILD_EXAMPLES=OFF",
            "-DBUILD_PYTHON_BINDINGS=OFF",
        ]

        build_args = ["--config", "Debug" if self.debug else "Release"]

        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            build_args += ["--parallel", str(os.cpu_count() or 1)]

        try:
            subprocess.check_call(
                ["cmake", ext.sourcedir] + cmake_args,
                cwd=build_temp,
            )
        except subprocess.CalledProcessError as e:
            print(f"CMake configuration failed: {e}", file=sys.stderr)
            raise

        try:
            subprocess.check_call(
                ["cmake", "--build", "."] + build_args,
                cwd=build_temp,
            )
        except subprocess.CalledProcessError as e:
            print(f"CMake build failed: {e}", file=sys.stderr)
            raise


VERSION = get_timber_version()

setup(
    name="timber",
    version=VERSION,
    packages=find_packages(),
    ext_modules=[CMakeExtension("timber._timber", sourcedir=".")],
    cmdclass={"build_ext": CMakeBuild},
    python_requires=">=3.6",
    zip_safe=False,
    description="Python bindings for timber-c logging library",
    author="Your Name",
    author_email="your.email@example.com",
)