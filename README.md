# Rhea

### N-to-N Test
Add the following to your bashrc:
```
export RHEA_SOURCE_DIR=/path/to/rhea
export RHEA_SOURCE_DIR=/path/to/rhea/build
```

Make sure to run the following (in the clion terminal if using clion):

```
source ~/.bashrc
```

Create the following directories:

```
mkdir -p /dev/shm/rhea_dir
mkdir -p /dev/shm/${USER}/single_node_jobmanager_server
mkdir -p /dev/shm/${USER}/single_node_workermanager_server
mkdir -p /dev/shm/rhea/rhea_client_service
mkdir -p /dev/shm/rhea/rhea_byteflow_regulator
```

Running tests in a terminal:

```
${RHEA_BUILD_DIR}/sentinel/sentinel_job_manager -conf ${RHEA_SOURCE_DIR}/sentinel/conf/sentinel_conf.json
${RHEA_BUILD_DIR}/rhea_client_service -conf ${RHEA_SOURCE_DIR}/sentinel/conf/sentinel_conf.json
${RHEA_BUILD_DIR}/test/rhea_test_exec -conf ${RHEA_SOURCE_DIR}/sentinel/conf/sentinel_conf.json
```

To run tests in clion, make sure to add the -conf param to your target.