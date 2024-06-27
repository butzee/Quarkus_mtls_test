#!/bin/bash
# Check if certs directory is empty and if so, copy the built certificates to it
if [ -z "$(ls -A /usr/app/certs)" ]; then
   cp -r /usr/app/build-certs/* /usr/app/certs/
fi