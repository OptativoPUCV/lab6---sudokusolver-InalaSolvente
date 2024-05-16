#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int sudo[9][9];
} Node;

int is_valid_move(Node* n, int row, int col, int num) {
    // Verificar si el número ya está en la misma fila
    for (int i = 0; i < 9; i++) {
        if (n->sudo[row][i] == num) {
            return 0; // No es una jugada válida
        }
    }

    // Verificar si el número ya está en la misma columna
    for (int i = 0; i < 9; i++) {
        if (n->sudo[i][col] == num) {
            return 0; // No es una jugada válida
        }
    }

    // Verificar si el número ya está en el mismo cuadrante 3x3
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (n->sudo[i][j] == num) {
                return 0; // No es una jugada válida
            }
        }
    }

    // Si no se encontró el número en la fila, columna o cuadrante, entonces es una jugada válida
    return 1;
}

Node *createNode() {
  Node *n = (Node *)malloc(sizeof(Node));
  return n;
}

Node *copy(Node *n) {
  Node *new = (Node *)malloc(sizeof(Node));
  *new = *n;
  return new;
}

Node *read_file(char *file_name) {
  Node *n = createNode();
  FILE *file = fopen(file_name, "r");
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (!fscanf(file, "%d", &n->sudo[i][j]))
        printf("failed to read data!");
    }
  }

  fclose(file);
  return n;
}

void print_node(Node *n) {
  int i, j;
  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++)
      printf("%d ", n->sudo[i][j]);
    printf("\n");
  }
  printf("\n");
}

int is_valid(Node *n) { return 1; }

List *get_adj_nodes(Node *n) {
  List *adj_nodes = createList();

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (n->sudo[i][j] == 0) { // Si la celda está vacía
        for (int num = 1; num <= 9; num++) {
          // Verificar si el número 'num' es válido en la posición (i, j)
          if (is_valid_move(n, i, j, num)) {
            // Si es válido, crear un nuevo nodo copiando el nodo actual
            Node *new_node = copy(n);
            // Asignar 'num' a la celda (i, j) en el nuevo nodo
            new_node->sudo[i][j] = num;
            // Agregar el nuevo nodo a la lista de nodos adyacentes
            pushBack(adj_nodes, new_node);
          }
        }
        // Una vez que se han probado todas las posibles jugadas para esta
        // celda, no es necesario seguir probando para esta celda, podemos pasar
        // a la siguiente.
        break;
      }
    }
  }

  return adj_nodes;
}

int is_final(Node *n) { return 0; }

Node *DFS(Node *initial, int *cont) { return NULL; }

/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/