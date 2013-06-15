/*******************************************************************************************
 * Tecnologico de Costa Rica                                                               *
 * Ingieneria en Computación                                                               *
 * Compiladores e Interpretes                                                              *
 * Analizador Semantico xhtml                                                              *
 * Tarea Programada 3                                                                      *
 * semantico.c                                                                             *
 * Header que funciones necesitadas para realizar el analisis semantico                    *
 *                                                                                         *
 * Estudiantes: Joseph Araya Rojas                                                         *
 *  			Luis Prado Rodríguez                                                       *
 *				Jean Carlo Cervantes                                                       *
 *                                                                                         *
 * Profesor: Andrei Fuentes                                                                *
 * *****************************************************************************************/

int atributoValido(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor);
void printErrorSemantico(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor);
void printErrorSemanticoAtributoInvalido(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor);
int isExpReg(char* expReg, char* dato);
int eventAtributtes(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor);
int globalatributes(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor);

