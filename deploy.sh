#!/bin/bash

# Upload this repo to gs://6854-experiment-code and then run this script for each runner instance

deploy_startup_script="apt-get update && apt-get install -y make g++ && mkdir -m 777 /experiment-data && gsutil rsync -r gs://6854-experiment-code /experiment-data && cd /experiment-data && make clean all"
deploy_startup_script=$( echo "$deploy_startup_script" | sed 's/ /\\ /g' | sed 's/&/\\&/g' | sed 's/+/\\+/g')

if [ $# -ne 1 ]; then
  echo "Bad"
  exit 1
fi
index=$1

deploy_cmd="gcloud beta compute instances create test-runner-$index --boot-disk-device-name=test-runner-$index --zone=us-central1-a --machine-type=e2-highmem-2 --scopes=https://www.googleapis.com/auth/cloud-platform --image=ubuntu-1804-bionic-v20201201 --image-project=ubuntu-os-cloud --boot-disk-size=10GB --boot-disk-type=pd-standard --metadata=startup-script=$deploy_startup_script"
eval $deploy_cmd
