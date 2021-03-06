/*******************************************************************************************
 * Tecnologico de Costa Rica                                                               *
 * Ingieneria en Computación                                                               *
 * Compiladores e Interpretes                                                              *
 * Analizador Semantico xhtml                                                              *
 * Tarea Programada 3                                                                      *
 * semantico.c                                                                             *
 * Archivo C que contiene la implementacion de                                             *
 * variables y funciones necesitadas para realizar el analisis semantico                   *
 *                                                                                         *
 * Estudiantes: Joseph Araya Rojas                                                         *
 *				Luis Prado Rodríguez                                       *
 *				Jean Carlo Cervantes                                       *
 *                                                                                         *
 * Profesor: Andrei Fuentes                                                                *
 * *****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#define BLUE "\e[34m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define WHITE "\e[m"
#define CYAN "\e[36m"
#define MAGENTA "\e[35m"


//Expresiones regulares implementadas
static char* languageCode= "^[a-z]{2}$";
static char* url= "^(http\\://)?[a-zA-Z0-9\\-\\.]+\\.(com|org|net|mil|edu|COM|ORG|NET|MIL|EDU)$";
static char* caracter =  "^[a-zA-Z0-9]$";
static char* texto = "^[a-zA-Z0-9\\-\\.]+$";
static char* textoWithSpace = "^[a-zA-Z0-9\\-\\.\\ ]+$";
static char* numero= "^[0-9]+$";
static char* codificacion = "^(ISO\\-8859\\-(1|2|3|4|5|6|7|8|9|10|15)|ISO\\-2022\\-(P|JP-2|KR)|UTF\\-(8|16))$";
static char* usemap = "^#[a-zA-Z0-9\\-\\.]+$";
static char* fecha = "^(19|20)\\d\\d[- /.](0[1-9]|1[012])[- /.](0[1-9]|[12][0-9]|3[01])$";
static char* sizes = "^([0-9]{2}[0-9]{2})+$";
static char* rgb = "^rgb\\([0-9]{1,3}, [0-9]{1,3}, [0-9]{1,3}\\)$";
static char* hexNum= "^#[0-9a-fA-F]+$";
static char* bgcolor = "(^[a-zA-Z0-9\\-\\.]+$|^rgb\\([0-9]{1,3}, [0-9]{1,3}, [0-9]{1,3}\\)$|^#[0-9a-fA-F]+$)";


//Metodo que me retorna 1 si se cumple la expresión regular y 0 si no
int isExpReg(char* expReg, char* dato){
    regex_t regex;
    int reti;

/* Compile regular expression */
  	reti = regcomp(&regex, expReg, REG_EXTENDED|REG_NEWLINE);
/* Execute regular expression */
    reti = regexec(&regex, dato, 0, NULL, 0);
    if(!reti){
    	//Se cumple la expresión
    	regfree(&regex);
        return 1;
    }
    regfree(&regex);
    return 0;
    	
}
////////////////////////////////////////

// funcion que indica warnings // tipo 1=obsoleto 0=solo html5

void printWarnings(char* elemento, int filaValor, int columnaValor,int tipo) 
{
	if(tipo)
	{
		fprintf(stderr,YELLOW"Advertencia en:"WHITE" Fila: "BLUE"%d"WHITE" Columna: "BLUE"%d"WHITE" ,Elemento obsoleto para html5: "BLUE"%s"WHITE".\n",
		filaValor, columnaValor,elemento);
	}
	else
	{
		fprintf(stderr,YELLOW"Advertencia en:"WHITE" Fila: "BLUE"%d"WHITE" Columna: "BLUE"%d"WHITE" ,Elemento válido sólo para html5: "BLUE"%s"WHITE".\n",
		filaValor, columnaValor,elemento);	
		
	}
}

////////////   		printWarnings(elemento,filaValor,columnaValor,1);//// arreglar columna 	
void printErrorSemantico(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor){
	fprintf(stderr,RED"Error semantico:"WHITE" Fila: "BLUE"%d"WHITE" Columna: "BLUE"%d"WHITE" ,Formato incorrecto en el valor: "BLUE"%s"WHITE", "WHITE"del atributo "BLUE"%s"WHITE", perteneciente al elemento  "BLUE"%s"WHITE".\n",filaValor, columnaValor, valorAtributo, atributo,elemento);
}
void printErrorSemanticoAtributoInvalido(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor){
	fprintf(stderr,RED"Error semantico:"WHITE" Fila: "BLUE"%d"WHITE" Columna: "BLUE"%d"WHITE" ,Atributo invalido "BLUE"%s"WHITE", para elemento "BLUE"%s"WHITE".\n",filaAtri, columnaAtri,atributo, elemento);
}
int atributoValido(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor){
	
	/*************************************************************************************************
	**************************************Verificación de atributos de a******************************
	*************************************************************************************************/
	if(!strcmp(elemento,"a")){
		
		if(!strcmp(atributo,"href")){
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"hreflang")){
			if(!isExpReg(languageCode,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"rel")){
			if( !strcmp(valorAtributo,"alternate") || !strcmp(valorAtributo,"author") || !strcmp(valorAtributo,"bookmark") || !strcmp(valorAtributo,"help") || !strcmp(valorAtributo,"license") || !strcmp(valorAtributo,"next") || !strcmp(valorAtributo,"nofollow") || !strcmp(valorAtributo,"noreferrer") || !strcmp(valorAtributo,"prefetch") || !strcmp(valorAtributo,"prev") || !strcmp(valorAtributo,"search") || !strcmp(valorAtributo,"tag"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"target")){
			if( !strcmp(valorAtributo,"_blank") || !strcmp(valorAtributo,"_self") || !strcmp(valorAtributo,"_parent") || !strcmp(valorAtributo,"_top"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de b******************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"b")){
		return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);	
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de blockquote*********************
	*************************************************************************************************/
	else if(!strcmp(elemento,"blockquote")){
		if(!strcmp(atributo,"cite")){
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de los elementos estandar*********
	*************************************************************************************************/
	else if(!strcmp(elemento,"br") || !strcmp(elemento,"dt") || !strcmp(elemento,"dl") || !strcmp(elemento,"dd") || !strcmp(elemento,"em") || !strcmp(elemento,"h1") || !strcmp(elemento,"h2") || !strcmp(elemento,"h3") || !strcmp(elemento,"h4") || !strcmp(elemento,"h5") || !strcmp(elemento,"h6") || !strcmp(elemento,"head") || !strcmp(elemento,"hr") || !strcmp(elemento,"span") || !strcmp(elemento,"strong") || !strcmp(elemento,"tr") || !strcmp(elemento,"title") || !strcmp(elemento,"p") || !strcmp(elemento,"ul") || !strcmp(elemento,"code") || !strcmp(elemento,"div") || !strcmp(elemento,"body"))
	{
		return globalatributes(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaAtri, columnaValor);
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de button*************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"button")){
		if(!strcmp(atributo,"autofocus")){
			if(!strcmp(valorAtributo,"autofocus"))
			{
				printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
				return 1;
			}
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
		}
		else if(!strcmp(atributo,"disabled")){
			if(!strcmp(valorAtributo,"disabled"))
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"form")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
			else
				return 1;
		}
		else if(!strcmp(atributo,"formaction")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}else if(!strcmp(atributo,"formenctype")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"application/x-www-form-urlencoded") || !strcmp(valorAtributo,"multipart/form-data") || !strcmp(valorAtributo,"text/plain"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}else if(!strcmp(atributo,"formmethod")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"get") || !strcmp(valorAtributo,"post"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}else if(!strcmp(atributo,"formnovalidate")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"formnovalidate"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}else if(!strcmp(atributo,"formtarget")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"formtarget"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}else if(!strcmp(atributo,"name")){
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
			else
				return 1;

		}else if(!strcmp(atributo,"type")){
			if( !strcmp(valorAtributo,"button") || !strcmp(valorAtributo,"reset") || !strcmp(valorAtributo,"submit"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}

		}else if(!strcmp(atributo,"value")){
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
			else
				return 1;
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}

	/*************************************************************************************************
	**************************************Verificación de atributos de caption************************
	*************************************************************************************************/

	else if(!strcmp(elemento,"caption")){
		if(!strcmp(atributo,"align")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"left") || !strcmp(valorAtributo,"right") || !strcmp(valorAtributo,"top") || !strcmp(valorAtributo,"botton"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	
	/*************************************************************************************************
	**************************************Verificación de atributos de embed*************************
	*************************************************************************************************/

	else if(!strcmp(elemento,"embed")){
		
		printWarnings(elemento,filaValor,columnaValor,0);//// tipo 1=obsoleto 0=solo html5
		
		if(!strcmp(atributo,"height")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"src")){
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
						//************************NOTA********************
						//**Type no se implemento porque utiliza MmType!**
		else if(!strcmp(atributo,"width")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de form*************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"form")){
						//************************NOTA********************
						//**Accept no se implemento porque utiliza MmType!**
		if(!strcmp(atributo,"accept-charset")){
			if(!isExpReg(codificacion,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		} 
		else if(!strcmp(atributo,"action")){
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"autocomplete")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"on") || !strcmp(valorAtributo,"off"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}else if(!strcmp(atributo,"enctype")){
			if( !strcmp(valorAtributo,"application/x-www-form-urlencoded") || !strcmp(valorAtributo,"multipart/form-data") || !strcmp(valorAtributo,"text/plain"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"method")){
			if( !strcmp(valorAtributo,"get") || !strcmp(valorAtributo,"post"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}

		}else if(!strcmp(atributo,"name")){
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}

		}else if(!strcmp(atributo,"novalidate")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"novalidate"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}else if(!strcmp(atributo,"target")){
			if( !strcmp(valorAtributo,"_blank") || !strcmp(valorAtributo,"_self") || !strcmp(valorAtributo,"_parent") || !strcmp(valorAtributo,"_top"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}

	/*************************************************************************************************
	**************************************Verificación de atributos de html*************************
	*************************************************************************************************/
	
	else if(!strcmp(elemento,"html")){
		if(!strcmp(atributo,"manifest")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		} else if(!strcmp(atributo,"xmlns")){
			if( !strcmp(valorAtributo,"http://www.w3.org/1999/xhtml"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}

	/*************************************************************************************************
	**************************************Verificación de atributos de img*************************
	*************************************************************************************************/

	
	else if(!strcmp(elemento,"img")){
		if(!strcmp(atributo,"align")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"left") || !strcmp(valorAtributo,"right") || !strcmp(valorAtributo,"top") || !strcmp(valorAtributo,"botton") || !strcmp(valorAtributo,"middle"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"alt")) {
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else
				return 1;
			}
		else if(!strcmp(atributo,"border")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"crossorigin")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"anonymous") || !strcmp(valorAtributo,"use-credentials"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"height")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"hspace")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"ismap")){
			if( !strcmp(valorAtributo,"ismap"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"longdesc")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"src")){
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"usemap")){
			if(!isExpReg(usemap,valorAtributo)){//Indica que no cumple con las condiciones de un usemap
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"vspace")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				printf("Warning: No soportado en HTML5, no recomendado en html 4.01");
				return 1;
			}
		}
		else if(!strcmp(atributo,"width")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de input*************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"input")){
		if(!strcmp(atributo,"align")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"left") || !strcmp(valorAtributo,"right") || !strcmp(valorAtributo,"top") || !strcmp(valorAtributo,"botton") || !strcmp(valorAtributo,"middle"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"alt")){
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"autocomplete")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"on") || !strcmp(valorAtributo,"off"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"autofocus")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!strcmp(valorAtributo,"autofocus"))
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
		}
		else if(!strcmp(atributo,"checked")){
			if(!strcmp(valorAtributo,"checked"))
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"disable")){
			if(!strcmp(valorAtributo,"disabled"))
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"form")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"formaction")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"formenctype")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"application/x-www-form-urlencoded") || !strcmp(valorAtributo,"multipart/form-data") || !strcmp(valorAtributo,"text/plain"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			}
		}
		else if(!strcmp(atributo,"formmethod")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"get") || !strcmp(valorAtributo,"post"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"formnovalidate")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"formnovalidate"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"formtarget")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"formtarget"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"height")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"list")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"max")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo) && !isExpReg(fecha, valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"maxlength")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"min")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo) && !isExpReg(fecha, valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"multiple")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"multiple"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"name")){
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"pattern")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			//Utiliza expresiones regulares y no podemos hacer una expresíon regular para reconocer
			//expresiones regulares, es basstante complejo
		}
		else if(!strcmp(atributo,"placeholder")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"readonly")){
			if(!strcmp(valorAtributo,"readonly"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"required")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"required"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"size")){
			if(!isExpReg(numero,valorAtributo) && !isExpReg(fecha, valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"src")){
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"step")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo) && !isExpReg(fecha, valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}	
		else if(!strcmp(atributo,"type")){
			if(!strcmp(valorAtributo,"button") || !strcmp(valorAtributo,"checkbox")|| !strcmp(valorAtributo,"color")|| !strcmp(valorAtributo,"date" ) || !strcmp(valorAtributo,"datetime") || !strcmp(valorAtributo,"datetime-local")|| !strcmp(valorAtributo,"email" ) || !strcmp(valorAtributo,"file")|| !strcmp(valorAtributo,"hidden")|| !strcmp(valorAtributo,"image")|| !strcmp(valorAtributo,"month" )|| !strcmp(valorAtributo,"number")|| !strcmp(valorAtributo,"password")|| !strcmp(valorAtributo,"radio")|| !strcmp(valorAtributo,"range")|| !strcmp(valorAtributo,"reset")|| !strcmp(valorAtributo,"search")|| !strcmp(valorAtributo,"submit")|| !strcmp(valorAtributo,"tel")|| !strcmp(valorAtributo,"text")||	!strcmp(valorAtributo,"time" )|| !strcmp(valorAtributo,"url")|| !strcmp(valorAtributo,"week" ))
			{	
				if(!strcmp(valorAtributo,"hidden"))
				{
					printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
				}
				return 1;
			}	
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"value")){
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"width"))
		{printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo) && !isExpReg(fecha, valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
		{
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
		}
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de li*************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"li")){
		if(!strcmp(atributo,"type")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!strcmp(valorAtributo,"1") || !strcmp(valorAtributo,"a") ||!strcmp(valorAtributo,"A") ||!strcmp(valorAtributo,"i") ||!strcmp(valorAtributo,"I") ||!strcmp(valorAtributo,"disc") ||!strcmp(valorAtributo,"circle") ||!strcmp(valorAtributo,"square"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		} 
		else if(!strcmp(atributo,"value")){
			if(!isExpReg(numero,valorAtributo) && !isExpReg(fecha, valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}

	/*************************************************************************************************
	**************************************Verificación de atributos de link*************************
	*************************************************************************************************/

	else if(!strcmp(elemento,"link")){
		if(!strcmp(atributo,"charset"))
		{
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(codificacion,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"href")){
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"hreflang")){
			if(!isExpReg(languageCode,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"media")){
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		} 
		else if(!strcmp(atributo,"rel")){
			if(!strcmp(valorAtributo,"alternate") || !strcmp(valorAtributo,"achieves") || !strcmp(valorAtributo,"author") || !strcmp(valorAtributo,"bookmark") ||  !strcmp(valorAtributo,"external") || !strcmp(valorAtributo,"first") ||!strcmp(valorAtributo,"help") || !strcmp(valorAtributo,"icon") || !strcmp(valorAtributo,"last") ||!strcmp(valorAtributo,"license") || !strcmp(valorAtributo,"next") || !strcmp(valorAtributo,"nofollow") || !strcmp(valorAtributo,"noreferrer") || !strcmp(valorAtributo,"pingback") || !strcmp(valorAtributo,"prefetch") || !strcmp(valorAtributo,"prev") || !strcmp(valorAtributo,"search") || !strcmp(valorAtributo,"sidebar") || !strcmp(valorAtributo,"styleheet") || !strcmp(valorAtributo,"tag") || !strcmp(valorAtributo,"up"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"rev")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!strcmp(valorAtributo,"alternate") || !strcmp(valorAtributo,"bookmark")  ||!strcmp(valorAtributo,"help") || !strcmp(valorAtributo,"start") || !strcmp(valorAtributo,"next") ||  !strcmp(valorAtributo,"contents") || !strcmp(valorAtributo,"index") || !strcmp(valorAtributo,"glossary") || !strcmp(valorAtributo,"copyrigth") || !strcmp(valorAtributo,"chapter") || !strcmp(valorAtributo,"prev") || !strcmp(valorAtributo,"section") || !strcmp(valorAtributo,"subsection") || !strcmp(valorAtributo,"styleheet") || !strcmp(valorAtributo,"appendix"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"sizes")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(sizes,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"target")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"_blank") || !strcmp(valorAtributo,"_self") || !strcmp(valorAtributo,"_parent") || !strcmp(valorAtributo,"_top"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"type")){
			//No implementamos mmtype
			return 1;
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}

	/*************************************************************************************************
	**************************************Verificación de atributos de meta*************************
	*************************************************************************************************/

	else if(!strcmp(elemento,"meta")){
		if(!strcmp(atributo,"charset")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(codificacion,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"content")){
			
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"http-equiv")){
			if( !strcmp(valorAtributo,"content-type") || !strcmp(valorAtributo,"default-style") || !strcmp(valorAtributo,"refresh"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"name")){
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}

	/*************************************************************************************************
	**************************************Verificación de atributos de ol*************************
	*************************************************************************************************/

	else if(!strcmp(elemento,"ol")){
		if(!strcmp(atributo,"reversed")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"reversed"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"start")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"type")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"1") || !strcmp(valorAtributo,"A") || !strcmp(valorAtributo,"a") || !strcmp(valorAtributo,"I") || !strcmp(valorAtributo,"i"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}	
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	
	/*************************************************************************************************
	**************************************Verificación de atributos de option*************************
	*************************************************************************************************/

	else if(!strcmp(elemento,"option")){
		if(!strcmp(atributo,"disabled")){
			if( !strcmp(valorAtributo,"disabled"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"label")){
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"selected")){
			if( !strcmp(valorAtributo,"selected"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"value")){
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de script*************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"script")){
		if(!strcmp(atributo,"charset")){
			if(!isExpReg(codificacion,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"async")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"async"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"defer")){
			if( !strcmp(valorAtributo,"defer"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"src")){
			if(!isExpReg(url,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"type")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			return 1;//No implementamos MMTYpe
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de style*************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"style")){
		if(!strcmp(atributo,"media")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"scoped")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"scopped"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"type")){
			return 1;//No implementamos mmtype
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	
	/*************************************************************************************************
	**************************************Verificación de atributos de table*************************
	*************************************************************************************************/
	
	else if(!strcmp(elemento,"table")){
		if(!strcmp(atributo,"align")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"left") || !strcmp(valorAtributo,"right") || !strcmp(valorAtributo,"top") || !strcmp(valorAtributo,"botton") || !strcmp(valorAtributo,"middle"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"bgcolor")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(bgcolor,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"border")){
			if( !strcmp(valorAtributo,"1") || !strcmp(valorAtributo,""))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"cellpadding")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"cellspacing")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(strcmp(atributo,"frame")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"void") || !strcmp(valorAtributo,"above") || !strcmp(valorAtributo,"below") || !strcmp(valorAtributo,"hsides") || !strcmp(valorAtributo,"lhs") || !strcmp(valorAtributo,"rhs")  || !strcmp(valorAtributo,"vsides")  || !strcmp(valorAtributo,"box")  || !strcmp(valorAtributo,"border") )
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"rules")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if( !strcmp(valorAtributo,"none") || !strcmp(valorAtributo,"groups") || !strcmp(valorAtributo,"rows") || !strcmp(valorAtributo,"cols") || !strcmp(valorAtributo,"all"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else if(!strcmp(atributo,"summary")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"width")){
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	
	/*************************************************************************************************
	**************************************Verificación de atributos de td*************************
	*************************************************************************************************/

	else if(!strcmp(elemento,"td")){
		if(!strcmp(atributo,"colspan")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"headers")){
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		} 
		else if(!strcmp(atributo,"rowspan")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de th*************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"th")){
		if(!strcmp(atributo,"colspan")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		} 
		else if(!strcmp(atributo,"headers")){
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		} 
		else if(!strcmp(atributo,"rowspan")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"scope")){
			if( !strcmp(valorAtributo,"col") || !strcmp(valorAtributo,"colgroup") || !strcmp(valorAtributo,"row") || !strcmp(valorAtributo,"rowgroup"))
				return 1;
			else{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
	/*************************************************************************************************
	**************************************Verificación de atributos de textarea*************************
	*************************************************************************************************/
	else if(!strcmp(elemento,"textarea")){
		if(!strcmp(atributo,"autofocus")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!strcmp(valorAtributo,"autofocus"))
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
		}
		else if(!strcmp(atributo,"cols")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"disabled")){
			if(!strcmp(valorAtributo,"disabled"))
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
		}
		else if(!strcmp(atributo,"form")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(texto,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"maxlength")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"name")){
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"placeholder")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!isExpReg(textoWithSpace,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}
		}
		else if(!strcmp(atributo,"readonly")){
			if(!strcmp(valorAtributo,"readonly"))
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
		}
		else if(!strcmp(atributo,"required")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!strcmp(valorAtributo,"required"))
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
		}
		else if(!strcmp(atributo,"rows")){
			if(!isExpReg(numero,valorAtributo)){
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			}
			else{
				return 1;
			}	
		}
		else if(!strcmp(atributo,"wrap")){
			printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
			if(!strcmp(valorAtributo,"hard")| !strcmp(valorAtributo,"soft") )
				return 1;
			else
			{
				printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
				return -1;
			} 
		}
		else
			return globalatributes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
}

int globalatributes(char* elemento, char* atributo, char* valorAtributo, int filaAtri, int columnaAtri, int filaValor, int columnaValor){

	/*************************************************************************************************
	**************************************Verificación de atributos globales*************************
	*************************************************************************************************/

	if(!strcmp(atributo,"accesskey")){
		if(!isExpReg(caracter,valorAtributo)){
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
		else
			return 1;
	}
	else if(!strcmp(atributo,"class")){
		if(!isExpReg(texto,valorAtributo)){
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
		else
			return 1;
	} 
	else if(!strcmp(atributo,"contenteditable")){
		printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
		if(!strcmp(valorAtributo,"true") || !strcmp(valorAtributo,"false") || !strcmp(valorAtributo,"inherit"))
			return 1;
		else
		{
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
	} 
	else if(!strcmp(atributo,"contextmenu")){
		printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
		if(!isExpReg(texto,valorAtributo)){
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		} 
		else
			return 1;
	} 
	else if(!strcmp(atributo,"dir")){
		if(!strcmp(valorAtributo,"ltr") || !strcmp(valorAtributo,"rtl") || !strcmp(valorAtributo,"auto"))
			return 1;
		else
		{
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
	} else if(!strcmp(atributo,"draggable")){
		printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
		if(!strcmp(valorAtributo,"true") || !strcmp(valorAtributo,"false") || !strcmp(valorAtributo,"auto"))
			return 1;
		else
		{
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
	}
	else if(!strcmp(atributo,"dropzone")){
		printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
		if(!strcmp(valorAtributo,"copy") || !strcmp(valorAtributo,"move") || !strcmp(valorAtributo,"link"))
			return 1;
		else
		{
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
	} 
	else if(!strcmp(atributo,"hidden")){
		printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
		if(!strcmp(valorAtributo,"hidden"))
			return 1;
		else
		{
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
	}else if(!strcmp(atributo,"id")){
		if(!isExpReg(texto,valorAtributo)){
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
		else
			return 1;

	} else if(!strcmp(atributo,"lang")){
		if(!isExpReg(languageCode,valorAtributo)){
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
		else
			return 1;
	} else if(!strcmp(atributo,"spellcheck")){
		printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
		if(!strcmp(valorAtributo,"true") || !strcmp(valorAtributo,"false"))
			return 1;
		else
		{
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
	}
	//Style no  porque necesita css
	else if(!strcmp(atributo,"tabindex")) {
		if(!isExpReg(numero,valorAtributo)){
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
		else
			return 1;

	}else if(!strcmp(atributo,"title")){
		if(!isExpReg(textoWithSpace,valorAtributo)){
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
		else
			return 1;
	}else if(!strcmp(atributo,"translate")){
		printWarnings(atributo,filaAtri,columnaAtri,0);//// tipo 1=obsoleto 0=solo html5
		if((!strcmp(valorAtributo,"yes"))||(!strcmp(valorAtributo,"no")))
			return 1;
		else
		{
			printErrorSemantico(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
			return -1;
		}
	}
	else{
		return eventAtributtes(elemento,atributo,valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
	}
}
//Función que valida los atributos de eventos, como estos usan script su contenido no se verificará
int eventAtributtes(char* elemento, char* atributo, char* valorAtributo,int filaAtri, int columnaAtri, int filaValor, int columnaValor){

	/*************************************************************************************************
	**************************************Verificación de atributos de eventos************************
	*************************************************************************************************/
	
	if(!strcmp(atributo,"onblur") || !strcmp(atributo,"onchange") || !strcmp(atributo,"onclick") || !strcmp(atributo,"ondblclick")|| !strcmp(atributo,"onfocus")|| !strcmp(atributo,"onkeydown")|| !strcmp(atributo,"onkeypress")|| !strcmp(atributo,"onkeyup")|| !strcmp(atributo,"onload")|| !strcmp(atributo,"onmousedown")|| !strcmp(atributo,"onmousemove")|| !strcmp(atributo,"onmouseout")|| !strcmp(atributo,"onmouseover")|| !strcmp(atributo,"onmouseup")|| !strcmp(atributo,"onreset")|| !strcmp(atributo,"onselect")|| !strcmp(atributo,"onsubmit")|| !strcmp(atributo,"onunload"))
	{
		if(!strcmp(atributo,"onreset"))
		{
			printWarnings(atributo,filaAtri,columnaAtri,1);//// tipo 1=obsoleto 0=solo html5
		}
		return 1;
	}
	else{
		printErrorSemanticoAtributoInvalido(elemento, atributo, valorAtributo, filaAtri, columnaAtri, filaValor, columnaValor);
		return -2;
	}
}
