CXXFLAGS+=-O3 -std=c++11 -fPIC

CXX = clang++

SSOPATH=$(shell pwd)/

CXXFLAGS+=-Iinc

# Project files
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

#Dynamic Library
OS_NAME=$(shell uname -s)
ifeq ($(OS_NAME),Linux)
DYN_SUFFIX=.so
DYN_OPT=-shared -Wl,-soname,lib$(NAME).so
endif
ifeq ($(OS_NAME),Darwin)
DYN_SUFFIX=.dylib
DYN_OPT=-dynamiclib -install_name $(SSOPATH)lib/$(DYN_PRODUCT) -compatibility_version $(VERSION) -current_version $(VERSION)
endif
VERSION=1.0.0
NAME=Solar
STAT_PRODUCT=lib$(NAME).a
DYN_PRODUCT=lib$(NAME)$(DYN_SUFFIX)

LDFLAGS+=-Wl,-rpath -Wl,$(SSOPATH)/lib -L$(SSOPATH)/lib -lSolar
LDFLAGS+=-lSQuIDS -lgsl

MAINS_SRC=$(wildcard mains/*.cpp)
MAINS=$(patsubst mains/%.cpp,bin/%.exe,$(MAINS_SRC))

.PHONY: all clean

# Compilation rules
all: $(STAT_PRODUCT) $(DYN_PRODUCT)

$(DYN_PRODUCT) : $(OBJECTS)
	@echo Linking $(DYN_PRODUCT)
	@$(CXX) $(DYN_OPT)  $(LDFLAGS) -o $(DYN_PRODUCT) $(OBJECTS)
	mv $(DYN_PRODUCT) $(SSOPATH)/lib/$(DYN_PRODUCT)

$(STAT_PRODUCT) : $(OBJECTS)
	@echo Linking $(STAT_PRODUCT)
	@$(AR) -rcs $(STAT_PRODUCT) $(OBJECTS)
	mv $(STAT_PRODUCT) $(SSOPATH)/lib/$(STAT_PRODUCT)

bin/%.exe : mains/%.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) $< $(LDFLAGS) -o $@

mains : $(MAINS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o lib/* bin/* mains/*.o

