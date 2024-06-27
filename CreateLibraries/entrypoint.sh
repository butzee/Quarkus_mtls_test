#!/bin/bash
# Check if libs directory is empty and if so, copy the built libraries to it
if [ -z "$(ls -A /usr/app/libs)" ]; then
   cp -r /usr/app/build-libs/* /usr/app/libs/
fi