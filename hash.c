#include<stdio.h>
#include <sys/stat.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/sha.h>
#include <dirent.h>


#define BUFFER_SIZE 4096

int hashFile(char* source, char* dest) {
    FILE *sourceFile, *destFile;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char buffer[BUFFER_SIZE];
    int bytesRead;
    SHA256_CTX sha256;

    // Open source file for reading
    sourceFile = fopen(source, "rb");
    if (!sourceFile) {
        perror("Error opening source file");
        return 1;
    }

    // Initialize SHA256 context
    SHA256_Init(&sha256);

    // Read source file and update hash context
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0) {
        SHA256_Update(&sha256, buffer, bytesRead);
    }

    // Finalize hash computation
    SHA256_Final(hash, &sha256);

    // Close source file
    fclose(sourceFile);

    // Open destination file for writing
    destFile = fopen(dest, "wb");
    if (!destFile) {
        perror("Error opening destination file");
        return 1;
    }

    // Write hash to destination file
    fwrite(hash, 1, SHA256_DIGEST_LENGTH, destFile);

    // Close destination file
    fclose(destFile);

    return 0;
}

    char* sha256file(char* file) {
    // Open the file for reading in binary mode
    FILE* file_ptr = fopen(file, "rb");
    if (file_ptr == NULL) {
        perror("Error opening file");
        return NULL;
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    unsigned char buffer[4096];
    size_t bytes_read;

    // Read file contents and update hash context
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file_ptr)) > 0) {
        SHA256_Update(&sha256, buffer, bytes_read);
    }

    // Close the file
    fclose(file_ptr);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    // Convert the hash to a hexadecimal string
    char* hash_hex = (char*)malloc((SHA256_DIGEST_LENGTH * 2 + 1) * sizeof(char));
    if (hash_hex == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hash_hex[i * 2], "%02x", hash[i]);
    }

    return hash_hex;
}
    // Définition de la structure Cellule
typedef struct cell {
    char* data;
    struct cell* next;
} Cell;

// Définition du type List comme un pointeur vers Cellule
typedef Cell* List;

// Fonction pour initialiser une liste vide
List* initList() {
    List* newList = (List*)malloc(sizeof(List));  // Allouer de la mémoire pour la liste
    if (newList == NULL) {
        perror("Erreur d'allocation mémoire pour la liste");
        exit(EXIT_FAILURE);
    }
    *newList = NULL;  // Initialiser la liste vide (pointeur NULL)

    return newList;
}
// Fonction pour construire et retourner une nouvelle cellule
Cell* buildCell(char* ch) {
    // Allouer de la mémoire pour une nouvelle cellule-
    Cell* newCell = (Cell*)malloc(sizeof(Cell));
    if (newCell == NULL) {
        perror("Erreur d'allocation mémoire pour la cellule");
        exit(EXIT_FAILURE);
    }

    // Allouer de la mémoire et copier la chaîne de caractères ch dans la cellule
    newCell->data = strdup(ch);
    if (newCell->data == NULL) {
        perror("Erreur d'allocation mémoire pour la chaîne de caractères");
        free(newCell);  // Libérer la mémoire allouée pour la cellule
        exit(EXIT_FAILURE);
    }

    // Initialiser le pointeur vers le prochain élément comme NULL
    newCell->next = NULL;

    return newCell;
}
// Fonction pour ajouter un élément en tête d'une liste
void insertFirst(List* L, Cell* C) {
    Cell* newCell=(Cell*)malloc(sizeof(Cell));
    newCell->data = C->data;
    newCell->next =*L;

    
    *L = newCell;
}
// Fonction pour afficher le contenu de la liste
void printList(List L) {
    printf("Contenu de la liste : ");
    Cell* current = L;
    while (current != NULL) {
        printf("%s -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}
// Fonction pour convertir une cellule en une chaîne de caractères
char* ctos(Cell* c) {
    if (c == NULL) {
        return NULL;
    }

    // Allouer de la mémoire pour la chaîne de caractères
    char* result = strdup(c->data);
    if (result == NULL) {
        perror("Erreur d'allocation mémoire pour la chaîne de caractères");
        exit(EXIT_FAILURE);
    }

    return result;
}
// Fonction pour transformer une liste en une chaîne de caractères avec le format "chaîne1|chaîne2|chaîne3|..."
char* ltos(List* L) {
    if (L == NULL || *L == NULL) {
        return NULL;
    }

    // Déclaration d'un tampon pour construire la chaîne de caractères résultante
    char* buffer = (char*)malloc(1);
    buffer[0] = '\0';

    Cell* current = *L;
    while (current != NULL) {
        char* cellString = ctos(current); // Convertir la cellule en une chaîne de caractères
        if (cellString != NULL) {
            size_t cellSize = strlen(cellString);
            size_t oldSize = strlen(buffer);
            buffer = (char*)realloc(buffer, oldSize+ cellSize + 2); // Réallouer la mémoire pour le buffer +2 pour le sepqrqteur I et le terminateur de chaine /0
            if (buffer == NULL) {
                perror("Erreur d'allocation mémoire pour le buffer");
                exit(EXIT_FAILURE);
            }

            strcat(buffer, cellString); // Concaténer la chaîne de la cellule au buffer
            strcat(buffer, "|");        // Ajouter le séparateur "|"

            free(cellString);            // Libérer la mémoire allouée pour la chaîne de la cellule
        }

        current = current->next; // Passer à la cellule suivante
    }

    return buffer;
}
// Fonction pour récupérer le i-ème élément d'une liste
Cell* listGet(List* L, int i) {
    if (L == NULL || *L == NULL || i < 0) {
        return NULL;  // Si la liste est vide ou l'indice est négatif, retourner NULL
    }

    Cell* current = *L;
    int index = 0;

    // Parcourir la liste jusqu'à l'élément à l'indice i
    while (current != NULL && index < i) {
        current = current->next;
        index++;
    }

    // Si current est NULL à ce stade, cela signifie que l'indice dépasse la taille de la liste
    if (current == NULL) {
        return NULL;
    }

    // Sinon, retourner l'élément à l'indice i
    return current;
}

// Fonction utilitaire pour afficher le contenu d'une cellule
void printCell(Cell* cell) {
    if (cell != NULL) {
        printf("%s\n", cell->data);
    } else {
        printf("Cellule non trouvée.\n");
    }
}
Cell* searchList(List* L, char* str) {
    if (*L == NULL) {
        return NULL; // La liste est vide
    }

    Cell* current = *L;
    while (current != NULL) {
        if (strcmp(current->data, str) == 0) {
            return current; // Retourne la référence vers l'élément si trouvé
        }
        current = current->next;
    }

    return NULL; // L'élément n'a pas été trouvé dans la liste
}
// Transforme une chaîne de caractères représentant une liste en une liste chaînée
List* stol(char* s) {
    List* newList = initList();
    char* token = strtok(s, "|");
    while (token != NULL) {
        insertFirst(newList, buildCell(token));
        token = strtok(NULL, "|");
    }
    return newList;
}
// Affiche les éléments de la liste
void printList1(List* L) {
    if (L == NULL || *L == NULL) {
        printf("La liste est vide.\n");
        return;
    }
    Cell* current = *L;
    printf("Contenu de la liste : ");
    while (current != NULL) {
        printf("%s -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}
// Écrit une liste dans un fichier
void ltof(List* L, char* path) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char* listString = ltos(L);
    if (listString == NULL) {
        fprintf(stderr, "La liste est vide.\n");
        fclose(file);
        return;
    }

    fprintf(file, "%s\n", listString);

    fclose(file);
    free(listString);
}
// Lit une liste depuis un fichier
List* ftol(char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    List* newList = initList();
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        char* token = strtok(buffer, "|");
        while (token != NULL) {
            insertFirst(newList, buildCell(token));
            token = strtok(NULL, "|");
        }
    }

    fclose(file);
    return newList;
}
// Affiche les éléments de la liste
void printList25(List* L) {
    if (L == NULL || *L == NULL) {
        printf("La liste est vide.\n");
        return;
    }
    Cell* current = *L;
    printf("Contenu de la liste : ");
    while (current != NULL) {
        printf("%s -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}
// Libère la mémoire allouée pour une liste
void freeList(List* L) {
    Cell* current = *L;
    while (current != NULL) {
        Cell* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(L);
}

// Explore un répertoire et renvoie une liste contenant les noms des fichiers et répertoires
List* listdir(char* root_dir) {
    List* fileList = initList();
    DIR* dir = opendir(root_dir);
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du répertoire");
        exit(EXIT_FAILURE);
    }
   
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            insertFirst(fileList, buildCell(entry->d_name));
        }
    }
   
    closedir(dir);
    return fileList;
}

// Affiche les éléments de la liste
void printList77(List* L) {
    if (L == NULL || *L == NULL) {
        printf("La liste est vide.\n");
        return;
    }
    Cell* current = *L;
    printf("Contenu de la liste : ");
    while (current != NULL) {
        printf("%s -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}
// Vérifie si un fichier existe dans le répertoire courant
int file_exists(char *file) {
    List* fileList = listdir(".");
    Cell* current = *fileList;
    while (current != NULL) {
        if (strcmp(current->data, file) == 0) {
            return 1; // Le fichier existe
        }
        current = current->next;
    }
    return 0; // Le fichier n'existe pas
}
void cp(char *to, char *from) {
    FILE *source_file = fopen(from, "r");

    // Ouvrir le fichier source en lecture
    source_file = fopen(from, "r");
    if (source_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier source");
        exit(EXIT_FAILURE);
    }

    // Ouvrir le fichier de destination en écriture
    FILE *dest_file = fopen(to, "w");
    if (dest_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de destination");
        fclose(source_file);
        exit(EXIT_FAILURE);
    }

    // Lire ligne par ligne depuis le fichier source et copier dans le fichier de destination
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), source_file) != NULL) {
        fputs(buffer, dest_file);
    }

    // Fermer les fichiers
    fclose(source_file);
    fclose(dest_file);

    printf("Le fichier a été copié avec succès.\n");
}
char* hashToPath(char* hash) {

    // Allouer de la mémoire pour le chemin
    char* path = (char*) malloc((strlen(hash) + 12) * sizeof(char)); //add for projetcsv/ et /0  
    if (path == NULL) {
        perror("Erreur lors de l'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    
    strcpy(path,"repertoires/");
    strcat(path,hash);
   
    return path;
}
void blobFile(char* file) { 
    // Calculate SHA-256 hash of the file 
   char* hash = sha256file(file);
     // Get directory path based on hash 
    char* path = hashToPath(hash); 
    // Create directory 
    if (mkdir(path, 0777) == -1) { 
        perror("Error creating directory"); 
        free(hash); 
        free(path); 
        exit(EXIT_FAILURE); } 
        // Copy file to the directory 
        char command[256]; 
        sprintf(command, "cp %s %s/", file, path); 
        system(command); 
        free(hash); 
        free(path); 
        printf("the rep was created successfully"); } 
// part 2
typedef struct {
    char* name;
    char* hash;
    int mode;
} WorkFile;
typedef struct {
    WorkFile* tab;
    int size;
    int n;
} WorkTree;
// Q-4.1 Créer un WorkFile et l'initialiser
WorkFile* createWorkFile(char* name) {
    WorkFile* newWorkFile = (WorkFile*)malloc(sizeof(WorkFile));
    if (newWorkFile == NULL) {
        perror("Erreur d'allocation mémoire pour WorkFile");
        exit(EXIT_FAILURE);
    }
    newWorkFile->name = strdup(name);
    newWorkFile->hash = NULL;
    newWorkFile->mode = 0;
    return newWorkFile;
}
// Q-4.2 Convertir un WorkFile en chaîne de caractères
char* wfts(WorkFile* wf) {
    if (wf == NULL) return NULL;

    char* str = (char*)malloc(sizeof(char) * (strlen(wf->name) + 20)); // Taille suffisante pour stocker le nom, le hash et le mode
    if (str == NULL) {
        perror("Erreur d'allocation mémoire pour la chaîne de caractères");
        exit(EXIT_FAILURE);
    }

    sprintf(str, "%s\t%s\t%d", wf->name, wf->hash, wf->mode);
    return str;
}

// Q-4.3 Convertir une chaîne de caractères en WorkFile
WorkFile* stwf(char* ch) {
    if (ch == NULL) return NULL;

    WorkFile* newWorkFile = (WorkFile*)malloc(sizeof(WorkFile));
    if (newWorkFile == NULL) {
        perror("Erreur d'allocation mémoire pour WorkFile");
        exit(EXIT_FAILURE);
    }

    // Utiliser strtok pour séparer les champs
    char* token = strtok(ch, "\t");
    newWorkFile->name = strdup(token);
    token = strtok(NULL, "\t");
    newWorkFile->hash = strdup(token);
    token = strtok(NULL, "\t");
    newWorkFile->mode = atoi(token);

    return newWorkFile;
}
#define MAX_WORKFILES 100 // Taille maximale du tableau de WorkFile dans WorkTree
// Q-4.4 Allouer et initialiser un WorkTree
WorkTree* initWorkTree() {
    WorkTree* newWorkTree = (WorkTree*)malloc(sizeof(WorkTree));
    if (newWorkTree == NULL) {
        perror("Erreur d'allocation mémoire pour WorkTree");
        exit(EXIT_FAILURE);
    }
    newWorkTree->tab = (WorkFile*)malloc(MAX_WORKFILES * sizeof(WorkFile));
    if (newWorkTree->tab == NULL) {
        perror("Erreur d'allocation mémoire pour le tableau de WorkFile");
        exit(EXIT_FAILURE);
    }
    newWorkTree->size = MAX_WORKFILES;
    newWorkTree->n = 0;
    return newWorkTree;
}
// Q-4.5 Vérifier la présence d'un fichier ou répertoire dans un WorkTree
int inWorkTree(WorkTree* wt, char* name) {
    for (int i = 0; i < wt->n; i++) {
        if (strcmp(wt->tab[i].name, name) == 0) {
            return i; // Fichier trouvé, retourner sa position dans le tableau
        }
    }
    return -1; // Fichier non trouvé
}
// Q-4.6 Ajouter un fichier ou répertoire au WorkTree
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode) {
    // Vérifier si le fichier existe déjà
    if (inWorkTree(wt, name) != -1) {
        return -1; // Fichier existe déjà dans le WorkTree
    }

    // Vérifier si le tableau est plein
    if (wt->n >= wt->size) {
        return -2; // Tableau plein, impossible d'ajouter plus de fichiers
    }

    // Ajouter le fichier au WorkTree
    wt->tab[wt->n].name = strdup(name);
    wt->tab[wt->n].hash = strdup(hash);
    wt->tab[wt->n].mode = mode;
    wt->n++;
    return 0; // Ajout réussi
}
// Q-4.7 Convertir un WorkTree en chaîne de caractères
char* wtts(WorkTree* wt) {
    char* str = (char*)malloc(sizeof(char) * (wt->n * 100)); // Supposons que chaque WorkFile prend au plus 100 caractères
    if (str == NULL) {
        perror("Erreur d'allocation mémoire pour la chaîne de caractères");
        exit(EXIT_FAILURE);
    }
    str[0] = '\0'; // Initialiser la chaîne de caractères vide

    // Parcourir le tableau de WorkFile et ajouter chaque WorkFile à la chaîne de caractères
    for (int i = 0; i < wt->n; i++) {
        char* workFileStr = wfts(&(wt->tab[i])); // Convertir le WorkFile en chaîne de caractères
        strcat(str, workFileStr);
        strcat(str, "\n"); // Ajouter un saut de ligne entre chaque WorkFile
        free(workFileStr); // Libérer la mémoire allouée pour la chaîne de caractères du WorkFile
    }

    return str;
}
// Q-4.8 Convertir une chaîne de caractères en WorkTree
WorkTree* stwt(char* str) {
    WorkTree* newWorkTree = initWorkTree();

    char* line = str;
    char* token;
    while ((token = strsep(&line, "\n")) != NULL) {
        // Skip empty lines
        if (strlen(token) == 0) {
            continue;
        }
        //printf("Token: '%s'\n", token); // Debug statement
        WorkFile* newWorkFile = stwf(token); // Convertir chaque ligne en WorkFile
        //printf("WorkFile created\n"); // Debug statement
        appendWorkTree(newWorkTree, newWorkFile->name, newWorkFile->hash, newWorkFile->mode); // Ajouter le WorkFile au WorkTree
        //printf("WorkFile appended to WorkTree\n"); // Debug statement
        free(newWorkFile->name);
        free(newWorkFile->hash);
        free(newWorkFile);
    }

    return newWorkTree;
}
// Q-4.9 Écrire la chaîne de caractères représentant un WorkTree dans un fichier
int wttf(WorkTree* wt, char* file) {
    // Ouvrir le fichier en mode écriture
    FILE* fptr = fopen(file, "w");
    if (fptr == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    // Convertir le WorkTree en chaîne de caractères et l'écrire dans le fichier
    char* workTreeStr = wtts(wt);
    fprintf(fptr, "%s", workTreeStr);

    // Fermer le fichier et libérer la mémoire allouée pour la chaîne de caractères
    fclose(fptr);
    free(workTreeStr);

    return 1;
}
// Q-4.10 Construire un WorkTree à partir d'un fichier contenant sa représentation en chaîne de caractères
WorkTree* ftwt(char* file) {
    // Ouvrir le fichier en mode lecture
    FILE* fptr = fopen(file, "r");
    if (fptr == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    // Lire le contenu du fichier dans une chaîne de caractères
    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char* buffer = (char*)malloc(fsize + 1);
    if (buffer == NULL) {
        perror("Erreur d'allocation mémoire pour la chaîne de caractères");
        fclose(fptr);
        return NULL;
    }
    fread(buffer, 1, fsize, fptr);
    fclose(fptr);
    buffer[fsize] = '\0';

    // Convertir la chaîne de caractères en WorkTree
    WorkTree* newWorkTree = stwt(buffer);

    // Libérer la mémoire allouée pour la chaîne de caractères
    free(buffer);

    return newWorkTree;
}
  int getChmod(const char *path){ 
    struct stat ret; 
    if (stat(path, &ret) == -1) { return -1; } 
    return 
    (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)|/*owner*/ 
    (ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)|/*group*/ 
    (ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH);/*other*/ }


void setMode(int mode, char* path){ 
    char buff [100]; 
    sprintf(buff, "chmod %d %s", mode, path); 
    system(buff); } 
// Définition de la fonction blobWorkTree
char* blobWorkTree(WorkTree* wt) {
    // Créer l'enregistrement instantané du WorkTree dans un fichier avec l'extension ".t"
    FILE* file = fopen("workTreeSnapshot.t", "w");
    // Écrire les détails du WorkTree dans le fichier
    fprintf(file, "WorkTree Snapshot:\n");
    for (int i = 0; i < wt->n; i++) {
        fprintf(file, "Name: %s, Hash: %s, Mode: %d\n", wt->tab[i].name, wt->tab[i].hash, wt->tab[i].mode);
    }

        char* hash = sha256file(file);
    fclose(file);

    return hash;
}
void freeWorkTree(WorkTree* wt) {
    for (int i = 0; i < wt->n; i++) {
        free(wt->tab[i].name);
        free(wt->tab[i].hash);
    }
    free(wt->tab);
    free(wt);
}
// Q5.2 Définition de la fonction saveWorkTree
char* saveWorkTree(WorkTree* wt, char* path) {
    for (int i = 0; i < wt->n; i++) {
        WorkFile* currentFile = &(wt->tab[i]);
        // Check if the current file is a directory
        if (S_ISDIR(currentFile->mode)) {
            // Recursive call to saveWorkTree for directories
            char* dirHash = saveWorkTree(currentFile->name, path);
            // Update the hash and mode of the directory WorkFile
            currentFile->hash = dirHash;
            currentFile->mode = getChmod(currentFile->name); // Assuming getChmod function is defined
        } else {
            // Regular file, create an instant snapshot
            char* fileHash = sha256file(currentFile->name);
            // Update the hash and mode of the file WorkFile
            currentFile->hash = fileHash;
            currentFile->mode = getChmod(currentFile->name); 
        }
    }
    // Create a snapshot of the entire WorkTree
    char* treeHash = blobWorkTree(wt); 
    
    // Save the WorkTree snapshot to the specified path
    //char* snapshotPath = strcat(path, ".t");
    FILE* snapshotFile = fopen(path, "w");
    if (snapshotFile == NULL) {
        perror("Error opening snapshot file");
        return NULL;
    }
    // Write the tree hash to the snapshot file
    fprintf(snapshotFile, "%s", treeHash);
    fclose(snapshotFile);

    return treeHash;
}
   
int main()
 { 
    system("sha256sum main.c > file.tmp");
    char* hash;
    List* myList = initList();
    char source[] = "main.c"; // Path to source file
    char dest[] = "file1.tmp"; // Path to destination file
    
    // Call hashFile function
    if (hashFile(source, dest) != 0) {
        printf("Error computing hash.\n");
        return 1;
 }
  hash = sha256file(source);
  if(hash != NULL)  {
        printf("hash SH256 du fichier '%s' : %s\n",source,hash);
        Cell* myCell = buildCell(hash);
        if (myCell != NULL) {
            printf("Cellule créée avec succès.\n");
            printf("Contenu de la cellule : %s\n", myCell->data);

            // Libérer la mémoire allouée pour la cellule
            free(myCell->data);
            free(myCell);
        } else {
            printf("Erreur lors de la création de la cellule.\n");
        }
        free(hash);

 }    // Create cells and add them to the list
    Cell* cell1 = buildCell("Cellule 1");
    Cell* cell2 = buildCell("Cellule 2");
    Cell* cell3 = buildCell("Cellule 3");
    insertFirst(myList, cell1);
    insertFirst(myList, cell2);
    insertFirst(myList, cell3);

    // Print contents of the list
    printf("Contents of the list:\n");
    printList(myList);

    // Convert list to string and print
    char* listString = ltos(myList);
    if (listString != NULL) {
        printf("List converted to string: %s\n", listString);
        free(listString);
    } else {
        printf("Error converting list to string\n");
    }

    // Get elements from the list by index
    printf("Content of element at index 0: ");
    printCell(listGet(myList, 0));
    printf("Content of element at index 1: ");
    printCell(listGet(myList, 1));
    printf("Content of element at index 2: ");
    printCell(listGet(myList, 2));

    // Search for an element in the list
    char* searchStr = "Cellule 2";
    Cell* result = searchList(myList, searchStr);
    if (result != NULL) {
        printf("'%s' found in the list.\n", searchStr);
    } else {
        printf("'%s' not found in the list.\n", searchStr);
    }

    // Free memory allocated for the list
    freeList(myList);

    // Convert string to list
    char str[] = "Bonjour|Hello|World";
    List* myListFromString = stol(str);

    // Convert list to string and print
    char* stringFromList = ltos(&myListFromString);
    printf("String from list: %s\n", stringFromList);
    free(stringFromList);

    // Write list to file
    ltof(myListFromString, "output.txt");

    // Read list from file
    List* myListFromFile = ftol("output.txt");

    // Print contents of list read from file
    printf("Contents of list read from file:\n");
    printList(myListFromFile);

    // Free memory allocated for lists
    freeList(myListFromString);
    freeList(myListFromFile);

    // List files in current directory
    List* fileList = listdir(".");
    printf("List of files and directories:\n");
    printList(fileList);
    freeList(fileList);

    // Check if file exists
    char filename[] = "output.txt";
    if (file_exists(filename)) {
        printf("%s exists in the current directory.\n", filename);
    } else {
        printf("%s does not exist in the current directory.\n", filename);
    }

    // Copy file
    char destination[] = "destination.txt";
    cp(destination, source);

    // Convert hash to file path
    char* path = hashToPath(source);
    if (path != NULL) {
        printf("File path corresponding to hash : %s\n", path);
        free(path);
    } else {
        printf("Error converting hash to file path.\n");
    }

    // Manipulate binary file
    //blobFile(source);


    //part 2 
     WorkFile* myWorkFile = createWorkFile(source);
     if (myWorkFile != NULL) {
        printf("WorkFile créé avec succès pour le fichier %s\n", filename);
        printf("Nom du WorkFile : %s\n", myWorkFile->name);
        printf("Hash du WorkFile : %s\n", myWorkFile->hash);
        printf("Mode du WorkFile : %d\n", myWorkFile->mode);

        // Convertir le WorkFile en chaîne de caractères et l'afficher
        char* str = wfts(myWorkFile);
        if (str != NULL) {
            printf("WorkFile converti en chaîne de caractères : %s\n", str);

            // N'oubliez pas de libérer la mémoire allouée pour la chaîne de caractères
            free(str);
        } else {
            printf("Erreur lors de la conversion du WorkFile en chaîne de caractères\n");
        }
         
        


        //  libérer la mémoire allouée pour le WorkFile
        free(myWorkFile->name);
        free(myWorkFile->hash);
        free(myWorkFile);
    }
    char workFileString[] = "example.txt\tabc123\t755";

    // Convertir la chaîne de caractères en WorkFile
    WorkFile* myWorkFile1 = stwf(workFileString);

    // Vérifier si la conversion a réussi
    if (myWorkFile1 != NULL) {
        printf("WorkFile converti avec succès à partir de la chaîne de caractères\n");
        printf("Nom du WorkFile : %s\n", myWorkFile1->name);
        printf("Hash du WorkFile : %s\n", myWorkFile1->hash);
        printf("Mode du WorkFile : %d\n", myWorkFile1->mode);

        // N'oubliez pas de libérer la mémoire allouée pour le WorkFile
        free(myWorkFile1->name);
        free(myWorkFile1->hash);
        free(myWorkFile1);
    } else {
        printf("Erreur lors de la conversion de la chaîne de caractères en WorkFile\n");
    }
    WorkTree* myWorkTree = initWorkTree();
      // Vérifier la présence d'un fichier ou répertoire dans le WorkTree
    int position = inWorkTree(myWorkTree, source);
    if (position != -1) {
        printf("Le fichier %s est présent dans le WorkTree à la position %d.\n", source, position);
    } else {
        printf("Le fichier %s n'est pas présent dans le WorkTree.\n", source);
    }
    // Ajouter un fichier au WorkTree
    int mode = 755;
    int resultApp = appendWorkTree(myWorkTree, source, hash, mode);
    if (resultApp == 0) {
        printf("Le fichier %s a été ajouté au WorkTree avec succès.\n", source);
    } else if (resultApp == -1) {
        printf("Le fichier %s existe déjà dans le WorkTree.\n", source);
    } else if (resultApp == -2) {
        printf("Le WorkTree est plein, impossible d'ajouter plus de fichiers.\n");
    }
    int resultApp1 = appendWorkTree(myWorkTree, "Ewemple.txt", sha256file("Ewemple.txt"), 455);
    if (resultApp1 == 0) {
        printf("Le fichier Ewemple.txt a été ajouté au WorkTree avec succès.\n");
    } else if (resultApp1 == -1) {
        printf("Le fichier Ewemple.txt existe déjà dans le WorkTree.\n");
    } else if (resultApp1 == -2) {
        printf("Le WorkTree est plein, impossible d'ajouter plus de fichiers.\n");
    }
    // Convertir le WorkTree en chaîne de caractères et l'afficher
    char* workTreeStr = wtts(myWorkTree);
    printf("Contenu du WorkTree sous forme de chaîne de caractères :\n%s\n", workTreeStr);
    //Question 5.2
    myWorkTree->size = 2;
    myWorkTree->n = 2;
    char* savePath = "snapshot.t";
    char* treeHash = saveWorkTree(myWorkTree, savePath);
    if (treeHash == NULL) {
        printf("Failed to save WorkTree snapshot.\n");
    } else {
        printf("WorkTree snapshot saved successfully with hash: %s\n", treeHash);
    }//THE END ?????????????????
    // Libérer la mémoire allouée pour le WorkTree et la chaîne de caractères
    free(workTreeStr);
    for (int i = 0; i < myWorkTree->n; i++) {
        free(myWorkTree->tab[i].name);
        free(myWorkTree->tab[i].hash);
    }
    free(myWorkTree->tab);
    free(myWorkTree);
    char workTreeString[] = "file1.txt\tabc123\t755\nfile2.txt\tdef456\t644\ndir1\tghi789\t777\n";

    // Convertir la chaîne de caractères en WorkTree
    WorkTree* myWorkTreeS = stwt(workTreeString);

    if (myWorkTreeS != NULL) {
        printf("WorkTree converti avec succès à partir de la chaîne de caractères\n");

        // Afficher le contenu du WorkTree
        char* workTreeContents = wtts(myWorkTreeS);
        printf("Contenu du WorkTree :\n%s\n", workTreeContents);
        int res7 = wttf(myWorkTreeS,"worktree.txt"); // ecrire le contenu du WorkTree dans worktree.txt
        if(res7 == 1){
        printf("Done\n");
    }
        free(workTreeContents);
        for (int i = 0; i < myWorkTreeS->n; i++) {
            free(myWorkTreeS->tab[i].name);
            free(myWorkTreeS->tab[i].hash);
        }
        free(myWorkTreeS->tab);
        free(myWorkTreeS);
    } else {
        printf("Erreur lors de la conversion de la chaîne de caractères en WorkTree\n");
    }
    WorkTree* myWorkTreeS57 = ftwt("worktree.txt"); 
    char* workTreeContents57 = wtts(myWorkTreeS57);
    printf("Contenu du WorkTree from worktree.txt :\n%s\n", workTreeContents57);
    free(workTreeContents57);
        for (int i = 0; i < myWorkTreeS57->n; i++) {
            free(myWorkTreeS57->tab[i].name);
            free(myWorkTreeS57->tab[i].hash);
        }
        free(myWorkTreeS57->tab);
        free(myWorkTreeS57);  
     

    


 return 0;}


