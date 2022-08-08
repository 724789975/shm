#==========================================================#
#===============    Makefile v1.0    ======================#
#==========================================================#

#==========================================================
#  Commands
CC		= gcc
CPP		= g++
AR		= ar
RANLIB	= ranlib

#  Flags
ifeq ($(BUILD),DEBUG)
D = d
C_LFLAGS		= -g -std=c++11 -O0 -rdynamic -Wall -D_DEBUG -DLINUX
CXX_FLAGS 		= -g -std=c++11 -O0 -rdynamic -Wall -Woverloaded-virtual -D_DEBUG -DLINUX 
else
D = 
C_LFLAGS		= -g -std=c++11 -rdynamic -Wall -DNDEBUG -DLINUX
CXX_FLAGS 		= -g -std=c++11 -rdynamic -Wall -Woverloaded-virtual -DNDEBUG -DLINUX
endif

ARFLAGS			= -rc
#==========================================================

#==========================================================
#  Commands
CODE_DIR = ./
INCLUDE_DIR =./
LIB_FILE = -Lcrypto -lpthread
OUTPUT_DIR =./
OUTPUT = shm
#==========================================================

OBJS = $(foreach i, $(CODE_DIR), $(shell find $(i) -name "*.cpp"))
INCLUDE_FLAG = $(foreach i, $(INCLUDE_DIR), -I$(i))

all:$(OUTPUT)

$(OUTPUT):$(OBJS:.cpp=.o)
	@echo Build...$@
	$(CPP) $(CXX_FLAGS) -o $(OUTPUT_DIR)/$(OUTPUT) $(OBJS:.cpp=.o) $(LIB_FILE)

%.o: %.cpp
#	@echo Compile...$@
	$(CPP) $(CXX_FLAGS) $(INCLUDE_FLAG) -c $< -o $@ -MMD -MP -MF$(@:%.o=%.d) 
	
-include $(OBJS:.cpp=.d)

clean:
	rm -f $(OUTPUT_DIR)/$(OUTPUT)
	rm -f $(OBJS:.cpp=.o)
	rm -f $(OBJS:.cpp=.d)
	
	@echo Clean ...done!
