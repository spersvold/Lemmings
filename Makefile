# Name of our executable
TARGETNAME	:= Lemmings

# Our sources
SOURCES		:= $(wildcard Assets/Code/SDL/*.cpp)
SOURCES		+= $(wildcard Assets/Code/TinyXML/*.cpp)
SOURCES		+= $(wildcard Assets/Code/*.cpp)

INCLUDES	:= -IAssets/Code

# Reduced output is default on
AT		:= @

# library information
PROGPREFIX      :=
PROGPOSTFIX     := .exe

CROSS_COMPILE	:=
CCC		:= $(CROSS_COMPILE)g++
COMMON_CFLAGS	:= -Wall -Ofast -fpermissive -Wno-sign-compare -Wno-conversion-null -Wno-unknown-pragmas
MARCH_FLAGS	:= -march=native

PLATFORMINCS	:= -I/usr/include/SDL2
PLATFORMLIBS	:= -lSDL2 -lSDL2_mixer -lSDL2_image

#
# Generate OBJS from SRCS
#
OBJS		= $(SOURCES:%.cpp=%.obj)

#
# Compose compile & link flags
#

PROGRAM		= $(PROGPREFIX)$(TARGETNAME)$(PROGPOSTFIX)
TARGET		= $(PROGRAM)

CCFLAGS		= $(MARCH_FLAGS) $(COMMON_CFLAGS) $(INCLUDES) $(PLATFORMINCS)
LDFLAGS		= $(MARCH_FLAGS) $(LDPATH) $(PLATFORMLIBS)
MKDEP_FLAGS	= $(INCLUDES) $(PLATFORMINCS)

#
# Target section
#

.PHONY:		all
all:		$(TARGET)

.PHONY:		clean
clean:
		@echo "#     Cleaning target & object files ..."
		@-rm -f $(TARGET) $(OBJS); \
		sync

$(PROGRAM):	$(OBJS)
		@echo "#     Linking $@ ..."
		$(AT)$(CCC) $(LDFLAGS) $(OBJS) -o $@

%.obj:		%.cpp
		@echo "#     Compiling $< (C++) ..."
		$(AT)$(CCC) -c $(CCFLAGS) -o $@ $<
