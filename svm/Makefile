TARGET = svm_flex

FLEXFLOAT_LIB_DIR  ?= ../../flexfloat/build
FLEXFLOAT_INC_DIR  ?= ../../flexfloat/include
LIBS = -lm  $(FLEXFLOAT_LIB_DIR)/libflexfloat.a -flto
CC = g++
CFLAGS = -O3 -std=c++11 -flto -I$(FLEXFLOAT_INC_DIR)

CONF_MODE ?= flags
OUTPUT_DIR ?= ./

ifeq ($(CONF_MODE),flags)
CFLAGS += -DEXP_SVM_KERNEL_SUM=11 -DFRAC_SVM_KERNEL_SUM=52 -DEXP_SVM_KERNEL_D=11 -DFRAC_SVM_KERNEL_D=52 -DEXP_SVM_KERNEL_X=11 -DFRAC_SVM_KERNEL_X=52 -DEXP_SVM_KERNEL_Y=11 -DFRAC_SVM_KERNEL_Y=52 -DEXP_SVM_KERNEL_TEMP1=11 -DFRAC_SVM_KERNEL_TEMP1=52
CFLAGS += -DEXP_SVM_PREDICT_SVCOEF=11 -DFRAC_SVM_PREDICT_SVCOEF=52 -DEXP_SVM_PREDICT_KVALUE=11 -DFRAC_SVM_PREDICT_KVALUE=52 -DEXP_SVM_PREDICT_SUM=11 -DFRAC_SVM_PREDICT_SUM=52
else
CFLAGS += $(EXT_CFLAGS)
endif


.PHONY: default all clean

default: $(TARGET)
all: default

SOURCES = svm_flex.cpp 
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
