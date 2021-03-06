/******************************************************************************
* Lire un fichier contenant un tableau de nombres                             *
*-----------------------------------------------------------------------------*
* COMPONENT NAME   = lit_tableau.c                                               *
*                                                                             *
*******************************************************************************/

/*
 * 
syntaxe:  lit_tableau < fichier

*/

/*-------------+
|Include Files |
+-------------*/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


/*--------------------------------+
| Variables globales au programme |
+--------------------------------*/

#define EXT 
#include "tab.h"

#include "lch.h"

extern void identifier_champ();
extern int identifier_5_5();
extern int peindre_5_5();
extern int identifier_7_7();
extern int peindre_7_7();
extern int identifier_9_9();
extern int peindre_9_9();

liste_chainee *racine;
liste_chainee *racine5;
liste_chainee *racine7;
liste_chainee *racine9;

long    peint_min;
long    peint_min_5_5;
long    peint_min_7_7;
long    peint_min_9_9;

int peindre_3_3( char *ptr_arg)
{
    long cell_l_c[3];
    long i,j,k,g, peint, vide;
    
    memcpy(&cell_l_c, ptr_arg, sizeof(cell_l_c));
    
    if ( cell_l_c[2] >= peint_min) {
        i=cell_l_c[0];
        j=cell_l_c[1];
        vide=0;
        peint=0;
        for(k=i; k<i+3; k++){
            for(g=j; g<j+3; g++){
                if(tab[k][g] == 1){
                    peint = peint + 1;
                } else if ( tab[k][g] == 0 ){
                    vide = vide + 1;
                }
            }
        }
        
        if ( peint-vide >= peint_min) {
            printf("%ld %ld 3 1\n",j+1,i+1 );
            for (k=i; k< i+3; k++ ){
                for(g=j; g<j+3; g++){
                    tab[k][g] += 2;
                }
            }
            return(LISTE_DETRUIT);
        } else if ( peint-vide < 2 ) { 
            return(LISTE_DETRUIT);        
        }
    }
    return(OK);
}


/*------------------------------------------------------*/
/*              Programme principal                     */
/*------------------------------------------------------*/

main(int argc, char **argv)
{
    long ic, il;
    
    
    nL=0;
    nC=0;
    racine=NULL;
    racine5=NULL;
    racine7=NULL;
    racine9=NULL;
    peint_min_5_5=(25+9)/2+1; // 21
    peint_min_7_7=(49+25)/2+1; // 41
    peint_min_9_9=(81+49)/2+1; // 69
    memset(tab,'\0',sizeof(tab));

    yylex();
    
    /* printf ("lignes %ld, col %ld \n",nL, nC);
    fflush(stdout); */
    
    
    // detecter_champ();
    identifier_champ();
    
    
    lch_traiter_file(&racine, identifier_5_5);
    lch_traiter_file(&racine, identifier_7_7);
    lch_traiter_file(&racine, identifier_9_9);
    
    for ( peint_min=9*9; peint_min>49; peint_min-- ) {
        lch_controler_file(&racine9, peindre_9_9);  
    }

    for ( peint_min=49; peint_min>25; peint_min-- ) {
        lch_controler_file(&racine7, peindre_7_7);  
        lch_controler_file(&racine9, peindre_9_9);  
    }

    for ( peint_min=25; peint_min>9; peint_min-- ) {
        lch_controler_file(&racine5, peindre_5_5);  
        lch_controler_file(&racine7, peindre_7_7);  
        lch_controler_file(&racine9, peindre_9_9);  
    }
    for ( peint_min=9; peint_min>=2; peint_min-- ) {
        lch_controler_file(&racine, peindre_3_3);
        lch_controler_file(&racine5, peindre_5_5);  
        lch_controler_file(&racine7, peindre_7_7);  
        lch_controler_file(&racine9, peindre_9_9);  
    }
        

    for ( il=0; il < nL ; il++ ) {
        for ( ic=0; ic < nC ; ic++) {
            if ( tab[il][ic] > 0) {
                if ( tab[il][ic]==1 ) {
                    printf("%ld %ld 1 1\n", ic, il );
                } else if ( tab[il][ic]%2 == 0 ){
                    printf("%ld %ld 1 0\n", ic, il );
                }
            } 
        }
    }


    exit(0);
}
