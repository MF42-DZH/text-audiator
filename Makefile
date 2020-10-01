CC     = gcc
CFLAGS = -Ofast -std=c18 -Wall -Wextra
TARGET = text-audiator
OBJS   = main wavcreate wavread

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))
src_dir     := $(current_dir)/src
build_dir   := $(current_dir)/build

all: $(TARGET)

$(TARGET): $(OBJS)
	mv $(src_dir)/*.o $(build_dir)
	cd $(build_dir); \
	$(CC) $(CFLAGS) -o $(TARGET) *.o; \
	mv $(TARGET) $(current_dir)

$(OBJS):
	cd $(src_dir); \
	$(CC) $(CFLAGS) -o $@.o -c $@.c

clean:
	rm $(build_dir)/*.o $(current_dir)/$(TARGET)

rebuild: clean all
