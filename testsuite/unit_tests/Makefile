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

.PHONY: clean
.PHONY: test

PATHU = ../Unity-master/src/
ROOT = ../../
PATHS = $(ROOT)

# PATH
PATHT = ./
PATHB = build/
PATHO = build/objs/
PATHR = build/results/

BUILD_PATHS = $(PATHB) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)Test_*.c)

HEADERS = $(wildcard $(PATHS)*.h)

COMPILE=gcc -fdata-sections -ffunction-sections -g -c
# -dead_strip is for MacOS 
# TODO: implement -Wl,--gc-sections or -Wl,--as-needed for other systems
LINK=gcc -Wl,-dead_strip
DEPEND=gcc -MM -MG -MF
CFLAGS=-I. -I$(ROOT) -I$(PATHU) -I$(PATHS) -I$(PATHC) -DTEST

RESULTS = $(patsubst $(PATHT)Test_%.c,$(PATHR)Test_%.txt,$(SRCT) )

PASSED = `grep -sh :PASS $(PATHR)*.txt`
FAIL = `grep -sh :FAIL $(PATHR)*.txt`
IGNORE = `grep -sh :IGNORE $(PATHR)*.txt`
INCOMPLETE = `grep -L -- '^-----------------------$$' $(PATHR)*.txt`

test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nRUN ERROR:\n-----------------------"
	@echo "$(INCOMPLETE)"

	@echo "\nDONE"



$(PATHR)%.txt: $(PATHB)%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1

$(PATHR)Test_configure.txt: $(wildcard test_config/*)

$(PATHB)Test_configure.$(TARGET_EXTENSION): $(PATHO)unity.o  $(PATHO)Test_configure.o $(PATHO)configure.o $(PATHO)cJSON.o 
	$(LINK) -o $@ $^


# $(PATHO)override.oo:: $(PATHT)override.c $(PATHT)override_internal.h
# 	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)Test_%.o:: $(PATHT)Test_%.c $(HEADERS)
	$(COMPILE) $(CFLAGS) $< -o $@

# $(PATHO)%.o:: $(PATHS)%.c $(PATHS)%.h $(PATHT)override.h
# 	$(COMPILE)  -include override.h $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHS)%.c $(HEADERS)
	$(COMPILE)  $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHU)%.c $(PATHU)%.h
	$(COMPILE) $(CFLAGS) $< -o $@


# $(PATHB):
# 	$(MKDIR) $(PATHB)

# $(PATHCO):
# 	$(MKDIR) $(PATHCO)

# $(PATHO):
# 	$(MKDIR) $(PATHO)

# $(PATHR):
# 	$(MKDIR) $(PATHR)

$(BUILD_PATHS):
	$(MKDIR) $@


clean:
	$(CLEANUP) $(PATHB)


.PRECIOUS: $(PATHB)Test_%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHO)%.o
.PRECIOUS: $(PATHR)%.txt