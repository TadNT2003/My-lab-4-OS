#!/usr/bin/bash
echo "Process file:"
cat process.txt
echo "Results"
gcc queue.c sched.c -o sched -lpthread
cat process.txt |  ./sched