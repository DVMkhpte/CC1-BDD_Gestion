# Nom de l'exécutable
EXEC = main

# Fichiers source
SRC = src/*

# Compilateur
CC = gcc

# Option de compilation
CFLAGS = -w

# Cibles
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

# Nettoyage - suppression de l'exécutable
clean:
	rm -f $(EXEC)

# Option pour exécuter le programme
run: $(EXEC)
	./$(EXEC) $(ARGS)