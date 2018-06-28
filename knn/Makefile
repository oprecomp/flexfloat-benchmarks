TARGET = knn_flex

FLEXFLOAT_LIB_DIR  ?= ../../flexfloat/build
FLEXFLOAT_INC_DIR  ?= ../../flexfloat/include
LIBS = -lm  $(FLEXFLOAT_LIB_DIR)/libflexfloat.a -flto
CC = g++
CFLAGS = -O3 -std=c++11 -flto -I$(FLEXFLOAT_INC_DIR)

CONF_MODE ?= flags
OUTPUT_DIR ?= ./

ifeq ($(CONF_MODE),flags)
CFLAGS += -DEXP_MALLOC_X=11 -DFRAC_MALLOC_X=52
CFLAGS += -DEXP_COMPAR_C=11 -DFRAC_COMPAR_C=52 -DEXP_COMPAR_D=11 -DFRAC_COMPAR_D=52 -DEXP_COMPAR_TEMP1=11 -DFRAC_COMPAR_TEMP1=52
CFLAGS += -DEXP_VOTE_A=11 -DFRAC_VOTE_A=52 -DEXP_VOTE_B=11 -DFRAC_VOTE_B=52 -DEXP_VOTE_TEMP1=11 -DFRAC_VOTE_TEMP1=52
CFLAGS += -DEXP_STRUCT_AUX_DIST=11 -DFRAC_STRUCT_AUX_DIST=52
else
CFLAGS += $(EXT_CFLAGS)
endif


.PHONY: default all clean

default: $(TARGET)
all: default

SOURCES = knn_flex.cpp
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

