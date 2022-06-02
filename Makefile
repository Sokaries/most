SHELL = /bin/sh

#Folder z plikami .c
SRCDIR=src
#Folder z plikami .h
INCDIR=include
#Folder z plikami .o .d
OBJDIR=obj

#Nazwa pliku wykonywalnego
BIN1=most_nocond
BIN2=most_cond

DOCS=Dokumentacja
Makefile=Makefile
TaskFile=taskfile

#Wybor kompilatora
CC=gcc
#Flagi automatycznie tworzace zaleznosci miedzy plikami .c i .h
DEPFLAGS=-MP -MD
#Flagi kompilatora
CFLAGS=-Wall -Wextra -I$(INCDIR) $(DEPFLAGS)


#Wszystkie pliki /src/*.c
CFILES1=nocond.c
CFILES2=cond.c
CFILESH=fifo.c

#Wszystkie pliki /obj/*.o
OBJECTS1=$(OBJDIR)/nocond.o $(OBJDIR)/fifo.o
OBJECTS2=$(OBJDIR)/cond.o $(OBJDIR)/fifo.o

#Wszystkie pliki /obj/*.d
DEPFILES=$(OBJDIR)/fifo.d

all: $(BIN1) $(BIN2)


$(BIN1): $(OBJECTS1)
	$(CC) -pthread -o $@ $^

$(BIN2): $(OBJECTS2)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)/
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/:
	@mkdir -p $@

.PHONY: clean
clean:
	@rm -rf $(BIN) $(OBJECTS) $(DEPFILES) $(OBJDIR)

-include $(DEPFILES)
