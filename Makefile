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
