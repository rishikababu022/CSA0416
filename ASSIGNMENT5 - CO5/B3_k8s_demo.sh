#!/usr/bin/env bash
# B3.3 - Kubernetes on kind (needs docker, kind, kubectl). Record the output of each command.
set -x
docker build -f B2_Dockerfile -t simats/result-portal:1.0 .
kind create cluster --config B3_kind-config.yaml
kind load docker-image simats/result-portal:1.0
kubectl apply -f B3_deployment.yaml
kubectl rollout status deployment/result-portal
kubectl get pods -o wide                              # expect 3 pods Running
kubectl describe pod -l app=result-portal | grep -A6 -E "Requests|Limits"
POD=$(kubectl get pods -l app=result-portal -o name | head -n 1)
kubectl delete "$POD"; sleep 3; kubectl get pods      # a new pod is created automatically
kubectl scale deployment/result-portal --replicas=6   # manual scale to 6
kubectl get pods
kubectl scale deployment/result-portal --replicas=3
kubectl apply -f B3_hpa.yaml                          # autoscaler 3..6 (needs metrics-server)
kubectl port-forward service/result-portal 8080:80 &
sleep 2; python3 B3_loadtest.py http://localhost:8080/result/ABC123 --concurrency 32 --duration 60
kubectl get hpa; kubectl get pods
kubectl apply -f B4_lms-deployment.yaml; kubectl top pods
kind delete cluster
