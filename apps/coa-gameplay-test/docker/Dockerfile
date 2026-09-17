# Worldserver image plus Python for the CoA gameplay test runner.
# The runner, scenarios and SQL updates are mounted from the repository at run time.
ARG BASE_IMAGE=acore/ac-wotlk-worldserver:master
FROM ${BASE_IMAGE}
ARG DOCKER_USER=acore

USER root
RUN apt-get update \
    && apt-get install -y --no-install-recommends python3 \
    && rm -rf /var/lib/apt/lists/*

USER ${DOCKER_USER}
