# Docker-Lite
build my own docker container from scratch using C 

🚀 Built My Own Mini-Container Runtime (No Docker, No Python)

I recently finished a project where I created a lightweight container environment from scratch, using only Linux primitives.
No Docker.
No Python.
Just low-level system calls and namespaces.

What it does:

Runs a process with its own PID namespace (process tree fully isolated)

Uses a separate mount namespace → its own file system view

Has a unique host name inside the container

Blocks access to system-level tools that aren't installed (like Python), proving real isolation

Behaves like a minimal container environment, not just a chroot hack

Why this matters:
Instead of relying on Docker abstractions, I learned exactly how containers work under the hood—how Linux isolates processes, manages resources, and creates secure, reproducible environments.

Tech used:

Linux namespaces

C

chroot + pivot_root concepts

Shell scripting

Process isolation fundamentals

This project forced me to understand the fundamentals instead of hiding behind frameworks—and it changed how I think about system-level engineering.

If you want to check it out or give feedback, I’m happy to share more details.

commands to run 

cd /mnt/c/Users/sarthak/Desktop/"docker lite" (directory of file)

gcc -o docker-lite docker-lite.c (build the code)

sudo ./docker-lite ./my-container-root /bin/bash (run shell)



cd app
python3 ChessMain.py (run ai chess bot inside of the container)


