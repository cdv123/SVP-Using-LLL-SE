#!/bin/bash
latticegen r $1 20 | tr -d '\n' >> tests.txt