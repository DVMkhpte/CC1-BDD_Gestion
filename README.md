# Gestion de Base de Données en C avec Arbre Binaire

Ce projet implémente un système de gestion de base de données (SGBD) en C, avec une structure de données en arbre binaire pour le stockage en mémoire, et une persistance des données via un fichier texte. Le projet permet de gérer des tables, des colonnes et des valeurs, et inclut des fonctionnalités pour les requêtes `SELECT`, `INSERT`,  `DELETE`, `CREATE TABLE`, `DROP`et `SHOW`.

## Sommaire

- [Aperçu](#aperçu)
- [Fonctionnalités](#fonctionnalités)
- [Installation](#installation)
- [Utilisation](#utilisation)
- [Architecture](#architecture)
- [Auteurs](#auteurs)


## Aperçu

Ce projet utilise une structure d’arbre binaire pour organiser les tables et les données de manière efficace, permettant des opérations rapides d'insertion, de recherche et de suppression. Chaque table, colonne et valeur est stockée sous forme de nœud dans l'arbre, et les informations sont persistées dans un fichier texte structuré.

## Fonctionnalités

- Création de tables et de colonnes avec des types de données.
- Insertion de valeurs avec détection automatique du type de données (entier, flottant, ou chaîne de caractères).
- Chargement et sauvegarde des données via un fichier texte structuré.
- Requêtes `SELECT`, `INSERT`, et `DELETE` avec support pour les conditions `WHERE`.
- Interface en ligne de commande pour interagir avec la base de données.

## Installation

### Prérequis

- [GCC](https://gcc.gnu.org/) pour la compilation du projet
- Make (optionnel pour simplifier la compilation)

### Installation

1. Clonez le dépôt :
   ```bash
   git clone https://github.com/DVMkhpte/CC1-BDD_Gestion
   ```

### Compilation et Exécution du programme

1.  Compilez et lancez le programme :
    ```bash
    make run clean
    ```
    ou

	Compilez et lancez le programme avec des arguments :
	
	```bash
    make run clean ARGS="Argument 1 Argument2"
    ```

Lancer les fonctions de test :
	
```bash
	make run clean ARGS="4 test" # Le nom de la base de donnée de change rien
```
## Utilisation

### Commandes Disponibles

- Créer une table :
	```bash
	CREATE TABLE nom_de_table (nom_de_colonne TYPE_DE_COLONNE, nom_de_colonne TYPE_DE_COLONNE);
	```
	- Type de colonne disponible :
		-  INT --> Integer
		- CHAR --> String
		- FLOAT --> Float



- Ajouter une valeur a une table :
	```bash
	INSERT INTO nom_de_table (nom_de_colonne1, nom de colonne2, ...) VALUES (valeur1,valeur2,...); # Le nombre de colonne doit être égal au nombre de valeur.
	```
- Afficher toutes les valeurs d'une colonne spécifique :
	```bash
	SELECT * FROM nom_de_table FOR nom_de_colonne;
	```	
- Afficher la structure de toutes les tables :
	```bash
	SHOW
	```
-  Supprimer toutes les valeurs d'une table :
	```bash
	DELETE * FROM nom_de_table;
	```
- Supprimer une valeur ou plusieurs valeurs selon une conditions :
	```bash
	DELETE * FROM nom_de_table WHERE condition (=, <, >);
	```
- Supprimer une table :
	```bash
	DROP TABLE nom_de_table;
	```
## Architecture

### Structure des Fichiers

Le projet est organisé comme suit :

```.
├── database/             # Fichier .txt de Base de donnée |
├── src/                  # Code source |
├── include/              # Fichiers d'en-tête │   
└── README.md
```

### Principales Structures de Données

- **Arbre Binaire** : Utilisé pour stocker les tables, colonnes et valeurs de manière hiérarchique.
- **Nœud de Table** : Contient le nom de la table et une liste de colonnes.
- **Nœud de Colonne** : Définit le nom et le type de chaque colonne.
- **Nœud de Valeur** : Stocke les valeurs associées aux colonnes, avec une détection de type (entier, flottant ou chaîne).
## Auteurs

- **Enzo Partel** - [GitHub Profil](https://github.com/DVMkhpte)