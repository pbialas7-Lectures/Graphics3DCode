#!/usr/bin/env python3

import argparse
import shutil
from pathlib import Path
import re
import sys

parser = argparse.ArgumentParser(description="Copy Assignment")
parser.add_argument("-f", "--force", action="store_true")
parser.add_argument("source", metavar="source", action="store")
parser.add_argument("dest", metavar="dest", action="store")

args = parser.parse_args()

ASSIGNMENTS_PATH = Path("src/Assignments")


if not ASSIGNMENTS_PATH.exists():
    print("You are not in top directory")
    sys.exit(0)

source_path = ASSIGNMENTS_PATH.joinpath(args.source)

if not source_path.exists() or not source_path.is_dir():
    print(f"Source {source_path.name():s} is not a subdirectory of {ASSIGNMENTS_DIR}")
    sys.exit(0)

dest_path = ASSIGNMENTS_PATH.joinpath(args.dest)

if dest_path.exists():
    if args.force:
        try:
            shutil.rmtree(dest_path.as_posix())
        except Exception as ex:
            print(ex)
            exit(1)
    else:
        print("Destination ", dest_path.name, " exists. Use --force flag to overwite")
        exit(0)


try:
    shutil.copytree(source_path.as_posix(), dest_path.as_posix())
except FileExistsError as err:
    print(err)
    sys.exit(0)

with open(dest_path.joinpath("CMakeLists.txt").as_posix()) as f:
    cmake_lists_txt = f.read()

project_re = re.compile("project\(\s*(\w+)\s*\)", re.I or re.M)
cmake_lists_txt = project_re.sub("project(" + args.dest + ")", cmake_lists_txt)

with open(dest_path.joinpath("CMakeLists.txt").as_posix(), "w") as f:
    f.write(cmake_lists_txt)

# with open("CMakeLists.txt") as f:
#     cmake_lists_txt = f.read()

# if not re.search("add_subdirectory\(" + dest_path.as_posix() + "\)", cmake_lists_txt):
#     with open("CMakeLists.txt", "a") as f:
#         f.write("\nadd_subdirectory(" + dest_path.as_posix() + ")")
