#!/bin/bash
cd "$(dirname "$0")"
cd originalcode
git submodule init
git submodule update
