# Copyright (c) 2024 Alexander Goponenko. University of Central Florida.
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# “Software”), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and
# to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=exe
else
	CLEANUP = rm -Rf
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
endif

ROOT = ./
PATHS = ./
PATHB = build/
PATHO = build/objs/

COMPILE=gcc -g -c
# -dead_strip is for MacOS 
# TODO: implement -Wl,--gc-sections or -Wl,--as-needed for other systems
LINK=gcc
CFLAGS=-std=gnu99 -I. -I$(ROOT) -I$(PATHS)

BUILD_PATHS = $(PATHB) $(PATHO)

SRCT = $(wildcard $(PATHS)*.c)
HEADERS = $(wildcard $(PATHS)*.h)
OBJ = $(patsubst $(PATHS)%.c,$(PATHO)%.o,$(SRCT) )

sevi: $(BUILD_PATHS) $(OBJ)
	$(LINK) -o $(PATHB)sevi $(OBJ)

$(PATHO)%.o: $(PATHS)%.c $(HEADERS)
	$(COMPILE) $(CFLAGS) $< -o $@

.PHONY: clean
.PHONY: test

test: 
	cd testsuite/unit_tests && make

$(BUILD_PATHS):
	$(MKDIR) $@

clean:
	$(CLEANUP) $(PATHB)
	cd testsuite/unit_tests && make clean
