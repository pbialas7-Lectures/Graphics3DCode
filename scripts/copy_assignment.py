#!/usr/bin/env python3

import argparse
import shutil
from pathlib import Path
import re
import sys

parser = argparse.ArgumentParser(description="Copy Assignment")
parser.add_argument("-f", "--force", action="store_true", help="overwrite the destination if it exists")
parser.add_argument("source", metavar="source", action="store")
parser.add_argument("dest", metavar="dest", action="store")

args = parser.parse_args()

ROOT_PATH = Path(__file__).resolve().parent.parent
ASSIGNMENTS_PATH = ROOT_PATH / "src" / "Assignments"

if not ASSIGNMENTS_PATH.is_dir():
    print(f"Cannot find assignments directory {ASSIGNMENTS_PATH}")
    sys.exit(1)

for name in (args.source, args.dest):
    if Path(name).name != name or name in (".", ".."):
        print(f"{name} should be a name of a directory in {ASSIGNMENTS_PATH}, not a path")
        sys.exit(1)

if args.source == args.dest:
    print("Source and destination must be different")
    sys.exit(1)

source_path = ASSIGNMENTS_PATH / args.source

if not source_path.is_dir():
    print(f"Source {args.source} is not a subdirectory of {ASSIGNMENTS_PATH}")
    sys.exit(1)

dest_path = ASSIGNMENTS_PATH / args.dest

if dest_path.exists():
    if args.force:
        try:
            shutil.rmtree(dest_path)
        except Exception as ex:
            print(ex)
            sys.exit(1)
    else:
        print(f"Destination {args.dest} exists. Use --force flag to overwrite")
        sys.exit(1)

try:
    shutil.copytree(source_path, dest_path)
except Exception as ex:
    print(ex)
    sys.exit(1)

cmake_lists_path = dest_path / "CMakeLists.txt"
cmake_lists_txt = cmake_lists_path.read_text()

leading_number_re = re.compile(r"^\d+_")
project_name = leading_number_re.sub("", args.dest)
project_re = re.compile(r"project\(\s*(\w+)\s*\)", re.I)
cmake_lists_txt = project_re.sub("project(" + project_name + ")", cmake_lists_txt)

cmake_lists_path.write_text(cmake_lists_txt)

# Only assignments listed in the top CMakeLists.txt are built.
top_cmake_lists_txt = (ROOT_PATH / "CMakeLists.txt").read_text()
assignments_match = re.search(r"set\(\s*ASSIGNMENTS\s+([^)]*)\)", top_cmake_lists_txt)
if assignments_match and args.dest not in assignments_match.group(1).split():
    print(f"Warning: {args.dest} is not on the ASSIGNMENTS list in {ROOT_PATH / 'CMakeLists.txt'} "
          f"and will not be built")
