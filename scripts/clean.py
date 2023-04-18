import os
import shutil

script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(script_dir, '..', 'build')

if os.path.exists(build_dir):
    shutil.rmtree(build_dir)
    os.mkdir(build_dir)
    shutil.copy(os.path.join(script_dir, "build.sh"), build_dir)
else:
    print("The build directory does not exist. Nothing to clean.")