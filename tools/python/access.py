#!/usr/bin/env python3
"""
Mount host virtual filesystems (/dev, /sys, /run, /proc) and chroot into a target rootfs.

Usage:
    access.py [rootfs_path] [interpreter]
"""

import os
import subprocess
import sys

DEFAULT_INTERPRETER = "/bin/sh"


def chroot_into(rootfs: str, interpreter: str = DEFAULT_INTERPRETER) -> int:
    abspath = os.path.abspath(rootfs)
    devpath = os.path.join(abspath, "dev")
    syspath = os.path.join(abspath, "sys")
    runpath = os.path.join(abspath, "run")
    procpath = os.path.join(abspath, "proc")

    subprocess.run(["mount", "--rbind", "/dev", devpath], check=False)
    subprocess.run(["mount", "--rbind", "/sys", syspath], check=False)
    subprocess.run(["mount", "--rbind", "/run", runpath], check=False)
    subprocess.run(["mount", "-t", "proc", "proc", procpath], check=False)

    return subprocess.run(["chroot", abspath, interpreter], check=False).returncode


if __name__ == "__main__":
    target_dir = sys.argv[1] if len(sys.argv) > 1 else ""
    target_shell = sys.argv[2] if len(sys.argv) > 2 else DEFAULT_INTERPRETER
    sys.exit(chroot_into(target_dir, target_shell))
