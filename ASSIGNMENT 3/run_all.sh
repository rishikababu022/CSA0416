#!/bin/bash
# Runs every task and stores the command output next to the sources (output_Q1.txt ... output_Q7.txt).
cd "$(dirname "$0")"
for q in 1 2 3 4 5 6 7; do
  echo ">>> Running Q$q ..."
  ./Q${q}_run.sh > output_Q$q.txt 2>&1
done
echo "Done. Outputs: output_Q1.txt ... output_Q7.txt, plots: *.png, CSV: *.csv"
