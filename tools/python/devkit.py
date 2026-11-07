#!/usr/bin/env python3
"""
EOS Devkit CLI (`devkit.py`) — Remote deployment and Synapse hot-reload utility
for Steam Deck, Raspberry Pi, and EOS Runtime nodes.

Usage:
    /opt/eos/bin/devkit.py status --host 192.168.1.50 [--port 9460]
    /opt/eos/bin/devkit.py reload --host 192.168.1.50 [--port 9460]
    /opt/eos/bin/devkit.py deploy <source_dir_or_binary> <user@host:/opt/eos/app> [--reload]
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys


def run_synapse_cmd(host: str, port: int, action: str) -> int:
    client_bin = shutil.which("synapse-client") or "/usr/bin/synapse-client"
    cmd = [client_bin, "-h", host, "-p", str(port), action]
    return subprocess.run(cmd, check=False).returncode


def deploy_artifact(source: str, destination: str, host: str | None, port: int, reload_after: bool) -> int:
    rsync_cmd = ["rsync", "-avz", "--progress", source, destination]
    rc = subprocess.run(rsync_cmd, check=False).returncode
    if rc != 0:
        return rc

    if reload_after:
        target_host = host
        if not target_host and ":" in destination:
            host_part = destination.split(":", 1)[0]
            target_host = host_part.split("@")[-1]
        if target_host:
            return run_synapse_cmd(target_host, port, "reload")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="EOS Devkit Remote Deploy & Synapse Control CLI")
    sub = parser.add_subparsers(dest="command", required=True)

    for cmd_name in ("status", "list", "ping", "reload"):
        p = sub.add_parser(cmd_name, help=f"Send '{cmd_name}' to remote Synapse node over libsockets TCP")
        p.add_argument("--host", "-H", default="127.0.0.1", help="Target EOS Runtime / Deck / Pi host")
        p.add_argument("--port", "-p", type=int, default=9460, help="Synapse TCP port (default: 9460)")

    p_deploy = sub.add_parser("deploy", help="Sync build bundle via rsync and optionally hot-reload Synapse")
    p_deploy.add_argument("source", help="Local binary, ROM, or bundle directory")
    p_deploy.add_argument("destination", help="Remote rsync destination (e.g. eos@raspberrypi:/opt/eos/app/)")
    p_deploy.add_argument("--host", "-H", default=None, help="Override Synapse host for reload")
    p_deploy.add_argument("--port", "-p", type=int, default=9460, help="Synapse TCP port (default: 9460)")
    p_deploy.add_argument("--reload", "-r", action="store_true", help="Trigger synapse-client reload after sync")

    args = parser.parse_args()
    if args.command in ("status", "list", "ping", "reload"):
        return run_synapse_cmd(args.host, args.port, args.command)
    if args.command == "deploy":
        return deploy_artifact(args.source, args.destination, args.host, args.port, args.reload)
    return 1


if __name__ == "__main__":
    sys.exit(main())
