#!/usr/bin/env python3

import subprocess
import re
import os

target_ver = None

orig, changed = "", ""
with open("CCD.rc", "rt") as f:
    for row in f:
        orig += row
        
        found = False
        if not found:
            m = re.search("^(?P<pre>.*?)(?P<ver>[0-9]+)(?P<post>.*INTVERSION.*)$", row, re.DOTALL | re.MULTILINE)
            if m is not None:
                pre, ver, post = m.group("pre"), m.group("ver"), m.group("post")
                if target_ver is None:
                    target_ver = int(ver) + 1
                    print(f"Moving from {ver} to {target_ver}")
                found = True
                changed += pre + str(target_ver) + post
        if not found:
            m = re.search("^(?P<pre>.*)(?P<ver>[0-9],[0-9],[0-9],[0-9])(?P<post>.*COMMAVERSION.*)$", row, re.DOTALL | re.MULTILINE)
            if m is not None:
                pre, ver, post = m.group("pre"), m.group("ver"), m.group("post")
                ver = f"{target_ver // 100},{(target_ver // 10) % 10},{target_ver % 10},0"
                found = True
                changed += pre + ver + post
        if not found:
            changed += row

with open("CCD.rc", "wt") as f:
    f.write(changed)

old_cwd = os.getcwd()
os.chdir(os.path.join("Make Hidden", "bin", "Debug"))
subprocess.check_call(["MakeHidden.exe", f"Build {target_ver}"])
os.chdir(old_cwd)
