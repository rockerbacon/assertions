#!/bin/bash
apt-package/install.sh g++
apt-package/install.sh cmake
apt-package/install.sh make
systemlib/install.sh pthread
git/install.sh https://github.com/rockerbacon/assertions-test.git b9e7fbdcd81dca10d0ac17b15fd20bcf4e302b8e
