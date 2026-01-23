import os
import shutil
import subprocess
import sys

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(ROOT_DIR, "build")

def run(cmd, cwd=None):
    print(">>", " ".join(cmd))
    subprocess.check_call(cmd, cwd=cwd)

def configure(build_type):
    os.makedirs(BUILD_DIR, exist_ok=True)
    run([
        "cmake",
        "-S", ROOT_DIR,
        "-B", BUILD_DIR,
        f"-DCMAKE_BUILD_TYPE={build_type}"
    ])

def build(build_type):
    configure(build_type)
    run([
        "cmake",
        "--build", BUILD_DIR,
        "--config", build_type
    ])

def clean():
    if os.path.exists(BUILD_DIR):
        print("Removing build directory...")
        shutil.rmtree(BUILD_DIR)
    else:
        print("Build directory does not exist.")

def usage():
    print("""
Usage:
  python build.py build [Debug|Release]
  python build.py clean

Examples:
  python build.py build Debug
  python build.py build Release
  python build.py clean
""")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()
        sys.exit(1)

    command = sys.argv[1].lower()

    if command == "build":
        build_type = sys.argv[2] if len(sys.argv) >= 3 else "Debug"
        if build_type not in ("Debug", "Release"):
            print("Build type must be Debug or Release")
            sys.exit(1)
        build(build_type)

    elif command == "clean":
        clean()

    else:
        usage()
        sys.exit(1)
