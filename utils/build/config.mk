# this makefile contains common configuration that should be use throughout the project
# it defines ROOT variable which points to the root of this project

SRCS_DIR := src
OBJS_DIR := bin
TEST_DIR := test
LIBS_DIR := lib

ifeq ($(RELEASE),1)
  OBJS_DIR += /release
endif

UNAME := $(shell uname)
ifneq ($(UNAME),Linux)
ifneq ($(UNAME),Darwin)
  # assume Windows
  UNAME := Windows
endif
endif

# run "make VERBOSE=1" to see all commands
ifeq ($(VERBOSE),0)
  Verb := @
else
  Verb :=
endif
Echo := $(Verb)echo

ROOT=$(shell git rev-parse --show-toplevel)
