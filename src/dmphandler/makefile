CC = g++
CFLAGS := -fPIC -std=c++11 -Wall -g
LDFLAGS := -fPIC -shared
#dynamic library export file
LDFLAGS += -Wl,--version-script=dmphandler.def

TARGET = dmphandler.dll
INC_PATH += -I./include
STATIC_LIBS +=
STATIC_LIB_PATH +=
LIBS += 
LIB_PATH +=

sources = dmphandler.cpp MiniDumpHandler.cpp

objects := $(patsubst %.cpp, %.o, $(sources))
objects := $(patsubst %.cc, %.o, $(objects))
objects := $(patsubst %.c, %.o, $(objects))
all : $(TARGET)
	@echo "make done"
%.o : %.cpp
	$(CC) $(CFLAGS) $(DEFINES) $(INC_PATH) -c $< -o $@
%.o : %.cc
	$(CC) $(CFLAGS) $(DEFINES) $(INC_PATH) -c $< -o $@
%.o : %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INC_PATH) -c $< -o $@

$(TARGET) : $(objects)
	$(CC) $(LDFLAGS) $^ -Wl,-Bstatic $(STATIC_LIBS) $(STATIC_LIB_PATH) -Wl,-Bdynamic $(LIBS) $(LIB_PATH) -o $@

clean :
	-rm -rf *.o $(TARGET)