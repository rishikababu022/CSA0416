#!/bin/sh
# Creates the repository history required by the assignment: one commit per task.
# Run each commit when you have actually executed that task (progressive commits are
# checked), then push:
#   git remote add origin https://github.com/<you>/CSA04-<RegisterNumber>-Assignment4.git
#   git push -u origin main
set -e
git init -b main 2>/dev/null || git init
git add README.md run_all.sh git_commits.sh && git commit -m "Add README and helper scripts"
git add Task01_* && git commit -m "Task 1: file-system design review"
git add Task02_* && git commit -m "Task 2: allocation methods, calculations and completed code"
git add Task03_* && git commit -m "Task 3: modern file systems and inspection"
git add Task04_* && git commit -m "Task 4: disk scheduling, completed SSTF and simulation"
git add Task05_* && git commit -m "Task 5: distributed storage model"
git add Task06_* && git commit -m "Task 6: video-streaming storage design and simulation"
git log --oneline
