# CSA04 – Operating Systems – Assignment 5 (CO5): Linux, Containers and Virtualisation

- Name: ____________________    Register number: ____________________
- Repository name: CSA04-<RegisterNumber>-Assignment5     GitHub link: ____________________
- Linux used for the runs in this zip: Ubuntu 24.04.5 LTS, 2 CPUs, 8 GB RAM, Python 3 with Flask 3.1.3
- Files are in one flat folder; the prefix is the task (A2_, B1_, B2_, B3_, B4_). Part A (diagrams, tables, reasoning)
  is report work and is not included, except the configuration files that belong to design tasks A2.

## Completed "Complete the Code" files
| Task | File | Check |
|---|---|---|
| B1 TODO 1, 2 (awk / sort / uniq) | B1_log_summary.sh | Output equals B1_log_summary_expected.txt (verified with diff) |
| B2 TODO 1, 2, 3 (Dockerfile) | B2_Dockerfile | Written and reviewed; NOT built here (see below) |
| B3 TODO 1, 2 (deployment.yaml) | B3_deployment.yaml | Parsed with a YAML loader: replicas 3, selector app: result-portal, requests 250m/256Mi, limits 500m/512Mi; NOT applied here |

## What was actually executed in this zip, and what you must run
| Task | Executed here (output file) | Must be run on your machine |
|---|---|---|
| B1.1 | B1_observe_system.sh -> B1_observe_system_output.txt (iostat, ss, htop not installed on this machine; the script falls back) | Rerun for your own server |
| B1.2 | B1_log_summary.sh, B1_disk_alert.sh -> *_output.txt | Install cron: `crontab B1_crontab.txt`, show `crontab -l` (crontab is not available here) |
| B1.3 | B1_firewall_ufw.sh and B1_setup_server.sh print the commands (dry run); the unit, nginx and logrotate files are written | Run with `APPLY=1` as root on a VM, then `systemctl status portal`, `nginx -t`, `ufw status` (nginx, ufw, systemd not available here) |
| B2 | Nothing: the Docker daemon is not running on this machine | `sh B2_commands.sh` (build, run, whoami, history, limits, stats, compose, multistage, ubuntu image) |
| B3.1/B3.2 | Bare-metal only: B3_measure_bare.sh -> B3_measure_bare_output.txt | B3_measure_docker.sh; the VM (B3_vm_cloud-init.yaml with KVM/VirtualBox) |
| B3.3 | Nothing: kubectl, kind and minikube are not installed here | `sh B3_k8s_demo.sh` (3 replicas, delete a pod, scale to 6, HPA) |
| B4.1 | Files written (B4_docker-compose.yml, B4_lms-deployment.yaml) | Start them and show `docker stats` / `kubectl top pods` |
| B4.2/B4.3 | B4_capacity_estimate.py, B4_cost_model.py outputs are EXAMPLES | Rerun with your measured container and VM requests per second and real prices |

## Measured here (bare metal, Flask built-in server, 2 CPU machine)
- Time to first HTTP 200: 0.08 s; memory about 32 MiB RSS; application files 8 KiB, Flask and dependencies about 2.8 MiB.
- Requests per second for /result/<regno> (about 1 ms CPU each): concurrency 1: 963, 8: 837, 32: 811.
- The load generator (B3_loadtest.py) is written in Python and shares the 2 CPUs with the server, so the figures are
  lower bounds. ab, wrk and hey are not installed here. Use the same tool for all three deployments (bare metal,
  VM, container) so the comparison is fair, and note that the container uses gunicorn while the bare-metal run used
  Flask's built-in server because gunicorn could not be installed without network access.

## Other notes
- app.py is a stand-in for the portal: it has /, /health, /result/<regno> (CPU work) and /dbcheck (TCP check of the
  database host). It does not store data in the database; the compose database exists to demonstrate the volume.
- The database password in B2_docker-compose.yml is a demo value; do not reuse it.
- B4_cost_model.py prices are placeholders, not real quotes.
- Alternative concepts: A2 Ansible -> A2_ansible_playbook.yml (not run: Ansible not installed); B2 Ubuntu / multi-stage
  images -> B2_Dockerfile.ubuntu, B2_Dockerfile.multistage; B3 Compose scaling -> B3_compose_scale.yml/.sh;
  B3 deployment on VM -> B3_vm_cloud-init.yaml.
- The assignment requires you to run every task yourself, keep one commit per task and include screenshots. These files
  are a starting point; the outputs from this machine do not replace your own runs.
