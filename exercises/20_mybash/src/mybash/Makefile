# src/mybash/Makefile

CC = gcc
CFLAGS = -Wall -I../../include -I../myfile -I../mysed -I../mywc -I../mytrans -D_GNU_SOURCE

TARGET := mybash
BINDIR ?= ../../bin
OBJDIR ?= ../../obj

# 收集所有 .c 文件（排除 main.c，由主 Makefile 单独处理）
LIB_SRCS := ../myfile/myfile.c ../mysed/mysed.c ../mywc/mywc.c \
            ../mytrans/mytrans.c ../mytrans/myhash.c
LIB_OBJS := $(LIB_SRCS:.c=.o)
LIB_OBJS := $(patsubst %.o,$(OBJDIR)/%.o,$(LIB_OBJS))

TARGET_PATH := $(BINDIR)/$(TARGET)

all: $(TARGET_PATH)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: ../myfile/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I../myfile -c $< -o $@

$(OBJDIR)/%.o: ../mysed/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I../mysed -c $< -o $@

$(OBJDIR)/%.o: ../mywc/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I../mywc -c $< -o $@

$(OBJDIR)/%.o: ../mytrans/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I../mytrans -c $< -o $@

clean:
	@rm -rf $(OBJDIR)/*.o