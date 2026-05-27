# 20_mybash/Makefile

CC = gcc
CFLAGS = -Wall -I./include -I./src/myfile -I./src/mysed -I./src/mywc -I./src/mytrans -D_GNU_SOURCE

BINDIR := ./bin
OBJDIR := ./obj

OBJS := $(OBJDIR)/main.o \
        $(OBJDIR)/myfile.o \
        $(OBJDIR)/mysed.o \
        $(OBJDIR)/mywc.o \
        $(OBJDIR)/mytrans.o \
        $(OBJDIR)/myhash.o

TARGET := $(BINDIR)/mybash

all: $(TARGET)

$(OBJDIR) $(BINDIR):
	@mkdir -p $@

$(OBJDIR)/main.o: src/mybash/main.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/myfile.o: src/myfile/myfile.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I./include -I./src/myfile -c $< -o $@

$(OBJDIR)/mysed.o: src/mysed/mysed.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I./include -I./src/mysed -c $< -o $@

$(OBJDIR)/mywc.o: src/mywc/mywc.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I./include -I./src/mywc -c $< -o $@

$(OBJDIR)/mytrans.o: src/mytrans/mytrans.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I./include -I./src/mytrans -c $< -o $@

$(OBJDIR)/myhash.o: src/mytrans/myhash.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I./include -I./src/mytrans -c $< -o $@

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -rf $(OBJDIR)/*.o $(BINDIR)/mybash