/*****************************************************************************/
/**  Analitzador sintàctic **/
/*****************************************************************************/
%{
#include <stdio.h>
#include <string.h>
#include "header.h"
%}

%token PCOMA_ AC_ CC_ AP_ CP_ COMA_ AL_ CL_
%token MAS2_ MAS_ MENOS2_ MENOS_ DIV_ POR_
%token BOOL_ IGUAL_ DIST_ NOT_ AND_ OR_ MY_ MYIG_ MN_ MNIG_ TRUE_ FALSE_ ASIG_
%token CTE_
%token ID_
%token RETURN_ IF_ ELSE_ FOR_ READ_ PRINT_ INT_
 

%%
programa: listaDeclaraciones
        ;
listaDeclaraciones: declaracion
                  | listaDeclaraciones declaracion
                  ;
declaracion: declaracionVariable
            | declaracionFuncion
            ;
declaracionVariable: tipoSimple ID_ PCOMA_
                    | tipoSimple ID_ AC_ CTE_ CC_ PCOMA_
                    ;
tipoSimple: INT_
          | BOOL_
          ;
declaracionFuncion: cabeceraFuncion bloque
                  ;
cabeceraFuncion: tipoSimple ID_ AP_ parametrosFormales CP_
                ;
parametrosFormales: 
                  | listaParametrosFormales
                  ;
listaParametrosFormales: tipoSimple ID_
                        | tipoSimple ID_ COMA_ listaParametrosFormales
                        ;
bloque: AL_  declaracionVariableLocal listaInstrucciones RETURN_ expresion PCOMA_ CL_
      ;
declaracionVariableLocal: 
                        | declaracionVariableLocal declaracionVariable
                        ;
listaInstrucciones:
                  | listaInstrucciones instruccion
                  ;
instruccion:AL_ listaInstrucciones CL_
          | instruccionAsignacion
          | instruccionSeleccion
          | instruccionEntradaSalida
          | instruccionIteracion
          ;
instruccionAsignacion: ID_ ASIG_ expresion PCOMA_
                    | ID_ AC_ expresion CC_ ASIG_ expresion PCOMA_
                    ;
instruccionEntradaSalida: READ_ AP_ ID_ CP_ PCOMA_
                        | PRINT_ AP_ expresion CP_ PCOMA_
                        ;
instruccionSeleccion: IF_ AP_ expresion CP_ instruccion ELSE_ instruccion
                    ;
instruccionIteracion: FOR_ AP_ expresionOpcional PCOMA_ expresion PCOMA_ expresionOpcional CP_ instruccion
                    ;
expresionOpcional:
                | expresion
                | ID_ ASIG_ expresion
                ;
expresion: expresionIgualdad
          | expresion operadorLogico expresionIgualdad   
          ;
expresionIgualdad: expresionRelacional
                  | expresionIgualdad operadorIgualdad expresionRelacional
                  ;
expresionRelacional: expresionAditiva
                  | expresionRelacional operadorRelacional expresionAditiva
                  ;
expresionAditiva: expresionMultiplicativa
                | expresionAditiva operadorAditivo expresionMultiplicativa        
                ;
expresionMultiplicativa: expresionUnaria
                        | expresionMultiplicativa operadorMultiplicativo expresionUnaria
                        ;
expresionUnaria: expresionSufija
                | operadorUnario expresionUnaria
                | operadorIncremento ID_
                ;
expresionSufija: AP_ expresion CP_
                | ID_ operadorIncremento
                | ID_ AC_ expresion CC_
                | ID_ AP_  parametrosActuales CP_
                | ID_
                | constante
                ;
parametrosActuales: 
		| listaParametrosActuales
            ;
listaParametrosActuales: expresion
		| expresion COMA_ listaParametrosActuales
            ;
constante : CTE_
          | TRUE_
          | FALSE_
          ;
operadorLogico: AND_
              | OR_    
              ;
operadorIgualdad: IGUAL_
                | DIST_
                ;
operadorRelacional: MY_
                  | MN_
                  | MYIG_
                  | MNIG_
                  ;
operadorAditivo: MAS_
                | MENOS_
                ;
operadorMultiplicativo: POR_
                      | DIV_
                      ;
operadorUnario: MAS_
              | MENOS_
              | NOT_
              ;
operadorIncremento: MAS2_
                  | MENOS2_
                  ;

%%
