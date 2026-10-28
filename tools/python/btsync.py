#!/usr/bin/env python3
"""
Launch peer-to-peer file synchronization daemon (Resilio Sync / btsync).
"""

import subprocess
import sys

if __name__ == "__main__":
    sys.exit(
        subprocess.run(
            ["/opt/btsync/btsync", "--webui.listen", "0.0.0.0:8888"],
            check=False,
        ).returncode
    )
