DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

CFLAGS := -I$(DIRHEA) -c -Wall
LDFLAGS := -lrt -lpthread
CC := gcc

all : dirs manager resolutor sumatorio

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

manager: $(DIROBJ)manager.o 
	$(CC) -lm -o $(DIREXE)$@ $^ $(LDFLAGS)

resolutor: $(DIROBJ)resolutor.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDFLAGS)

sumatorio: $(DIROBJ)sumatorio.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDFLAGS)

$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CFLAGS) $^ -o $@

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~
