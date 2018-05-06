#!/bin/bash
find . -name '*[ch]' -exec etags -a '{}' \;
