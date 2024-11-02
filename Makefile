# Nom de l'exécutable
EXEC = src/main

# Fichiers source
SRC = src/*

# Compilateur
CC = gcc

# Cibles
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(SRC)
	$(CC) -o $(EXEC) $(SRC)

# Nettoyage - suppression de l'exécutable
clean:
	rm -f $(EXEC)

# Option pour exécuter le programme
run: $(EXEC)
	./$(EXEC) $(ARGS)