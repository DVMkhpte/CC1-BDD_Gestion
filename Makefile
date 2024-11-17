# Nom de l'exécutable
EXEC = sgbd

# Fichiers source
SRC = src/*

# Compilateur
CC = gcc

# Option de compilation
CFLAGS = -w -o  # -w pour enlever tous les warnings et -o pour spécifier le nom de l'exécutable
# Cibles
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(SRC)
	@$(CC) $(CFLAGS) $(EXEC) $(SRC)

# Nettoyage - suppression de l'exécutable
clean:
	@rm -f $(EXEC)

# Option pour exécuter le programme
run: $(EXEC)
	@./$(EXEC) $(ARGS)