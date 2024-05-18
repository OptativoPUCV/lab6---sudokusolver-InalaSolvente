#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int sudo[9][9];
} Node;

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

int is_valid(Node *n) {
  // Verificar filas y columnas
  for (int i = 0; i < 9; i++) {
    int row_check[10] = {0}; // Inicializar un arreglo para verificar filas
    int col_check[10] = {0}; // Inicializar un arreglo para verificar columnas
    for (int j = 0; j < 9; j++) {
      // Verificar filas
      if (n->sudo[i][j] != 0) {
        if (row_check[n->sudo[i][j]] == 1) {
          return 0; // Si el número ya aparece en la fila, no es válido
        } else {
          row_check[n->sudo[i][j]] = 1; // Marcar el número como visto
        }
      }
      // Verificar columnas
      if (n->sudo[j][i] != 0) {
        if (col_check[n->sudo[j][i]] == 1) {
          return 0; // Si el número ya aparece en la columna, no es válido
        } else {
          col_check[n->sudo[j][i]] = 1; // Marcar el número como visto
        }
      }
    }
  }

  // Verificar cuadrantes 3x3
  for (int i = 0; i < 9; i += 3) {
    for (int j = 0; j < 9; j += 3) {
      int quad_check[10] = {
          0}; // Inicializar un arreglo para verificar cuadrantes
      for (int k = i; k < i + 3; k++) {
        for (int l = j; l < j + 3; l++) {
          // Verificar el cuadrante
          if (n->sudo[k][l] != 0) {
            if (quad_check[n->sudo[k][l]] == 1) {
              return 0; // Si el número ya aparece en el cuadrante, no es válido
            } else {
              quad_check[n->sudo[k][l]] = 1; // Marcar el número como visto
            }
          }
        }
      }
    }
  }

  // Si pasó todas las verificaciones, la configuración es válida
  return 1;
}

List *get_adj_nodes(Node *n) {
  List *adj_nodes = createList();

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (n->sudo[i][j] == 0) {
        for (int num = 1; num <= 9; num++) {
          Node *new_node = copy(n);   // Realizar una copia del nodo original
          new_node->sudo[i][j] = num; // Asignar un número a la celda
          if (is_valid(
                  new_node)) { // Verificar si el tablero resultante es válido
            pushBack(
                adj_nodes,
                new_node); // Agregar el nuevo nodo a la lista de adyacentes
          } else {
            free(new_node); // Liberar la memoria del nuevo nodo si no es válido
          }
        }
        return adj_nodes; // Salir de la función después de procesar la celda
                          // vacía
      }
    }
  }

  return adj_nodes;
}

int is_final(Node *n) {
  // Verificar si no hay celdas vacías en el tablero
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (n->sudo[i][j] == 0) return 0;
    }
  }
  return 1; // Si no hay celdas vacías, es un estado final
}

Node *DFS(Node *initial, int *cont) {
  *cont = 0;
  Stack *S = createStack();
  push(S, initial);

  while (!is_empty(S)) {
    Node *current = top(S);
    pop(S);
    (*cont)++;

    if (is_final(current)) {
      clean(S);
      free(S);
      return current;
    }

    List *adj_nodes = get_adj_nodes(current);
    Node *adj_node = front(adj_nodes);
    while (adj_node != NULL) {
      push(S, adj_node);
      adj_node = next(adj_nodes);
    }
    clean(adj_nodes);
    free(adj_nodes);
    free(current);
  }

  clean(S);
  free(S);
  return NULL;
}


int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}