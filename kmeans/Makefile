TARGET = kmeans_flex

FLEXFLOAT_LIB_DIR  ?= ../../flexfloat/build
FLEXFLOAT_INC_DIR  ?= ../../flexfloat/include
LIBS = -lm  $(FLEXFLOAT_LIB_DIR)/libflexfloat.a -flto
CC = g++
CFLAGS = -O3 -std=c++11 -flto -I$(FLEXFLOAT_INC_DIR)

CONF_MODE ?= flags
OUTPUT_DIR ?= ./

ifeq ($(CONF_MODE),flags)
CFLAGS += -DEXP_OBJECT=11 -DFRAC_OBJECT=52  -DEXP_CLUSTER=11 -DFRAC_CLUSTER=52 -DEXP_DIST=11 -DFRAC_DIST=52
CFLAGS += -DEXP_TEMP1=11 -DFRAC_TEMP1=52 -DEXP_TEMP2=11 -DFRAC_TEMP2=52
else
CFLAGS += $(EXT_CFLAGS)
endif


.PHONY: default all clean

default: $(TARGET)
all: default

SOURCES = kmeans_flex.cpp 
OBJECTS = $(patsubst %.cpp, $(OUTPUT_DIR)%.o, $(SOURCES))
HEADERS = $(wildcard *.h)

$(OUTPUT_DIR)%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $(OUTPUT_DIR)$@

clean:
	-rm -f $(OUTPUT_DIR)*.o
	-rm -f $(OUTPUT_DIR)$(TARGET)

clean-all: clean
	-rm -rf mpi_data_* log.txt
