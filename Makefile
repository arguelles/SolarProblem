CXXFLAGS+=-O3 -std=c++11 -fPIC

CXX = clang++

SSOPATH=${CURDIR}

LDFLAGS+=-lSQuIDS -lgsl
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
DYN_OPT=-dynamiclib -install_name $(LWPATH)/lib/$(DYN_PRODUCT) -compatibility_version $(VERSION) -current_version $(VERSION)
endif
VERSION=1.0.0
NAME=Solar
STAT_PRODUCT=lib$(NAME).a
DYN_PRODUCT=lib$(NAME)$(DYN_SUFFIX)

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

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o lib/* bin/*

