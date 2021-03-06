/******************************************************************************
* repartition serveurs par groupes                                            *
*                                                                             *
*******************************************************************************/

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

#include "tab.h"


void optimizer_serveur(){
	int i,j;
	int groupeCap[NB_GROUP];
	int hasBeenImproved;
	int groupeRangee[NB_GROUP][NB_RANGEE];
	int lowestGroup, lowestValue;
	int highestGroup, highestValue;	
	int value;
	int previousScore;
	int maxg;
	int protectedRangee[NB_RANGEE];
	/*hold previous position*/
	int transformation, rangee, firstServer, secondServer;

	for(i=0; i<NB_SERV;i++){
		if(serv[i][5] !=-1){
			groupeRangee[serv[i][3]][serv[i][4]]=1;
		}
	}

	for(i=0; i<NB_GROUP;i++){
		groupeCap[i]=cap_garanti_group(i);
	}

	hasBeenImproved=1;
	while(hasBeenImproved==1){
		hasBeenImproved=0;

		memset(protectedRangee,'\0',sizeof(protectedRangee));
		
		lowestGroup=0;
		highestGroup=0;
		lowestValue=groupeCap[0];
		highestValue=groupeCap[0];

		for(i=1; i<NB_GROUP; i++){
			value= groupeCap[i];
			if(value<lowestValue){
				lowestGroup=i;
				lowestValue=value;
			}

			if(value>highestValue){
				highestGroup=i;
				highestValue=value;
			}
		}

		previousScore=lowestValue;
		fprintf(stderr,"highestGroup %d, highestValue %d, lowestGroup %d, lowestValue %d\n",highestGroup, highestValue, lowestGroup, lowestValue);
		/*determiner rangee à ne pas traiter*/
		maxg=-1;
		for(i=1; i<NB_SERV;i++){
			if ( serv[i][3] == lowestGroup && serv[i][5] != -1 ) {
				if(maxg==-1 || maxg<serv[i][1]){
					maxg=serv[i][1];
				}
			}
		}
		for(i=1; i<NB_SERV;i++){
			if ( serv[i][3] == lowestGroup && serv[i][5] != -1 ) {
				if(maxg=serv[i][1]){
					protectedRangee[serv[i][4]]=1;
				}
			}
		}

		/*test de toute les rangee a améliorer*/
		for(i=0; i<NB_RANGEE; i++){
			if(protectedRangee[i]==1){
				continue;
			}
			if(groupeRangee[highestGroup][i]==1 && groupeRangee[lowestGroup][i]==0){
				//tentative de changement de groupe des serveurs
				firstServer=-1;
				for(j=0; j<NB_SERV;j++){
					if ( serv[j][3] == highestGroup && serv[j][5] != -1 && serv[j][4]==i) {
						firstServer=j;
						serv[firstServer][3]=lowestGroup;
						break;
					}
				}

				if(firstServer!=-1){
					if(cap_garanti_group(highestGroup)>previousScore && cap_garanti_group(lowestGroup)>previousScore){
						fprintf(stderr,"Serveur ajoute \n");
                                                groupeRangee[i][highestGroup]=0;
						groupeRangee[i][lowestGroup]=1;
						groupeCap[highestGroup]=cap_garanti_group(highestGroup);
						groupeCap[lowestGroup]=cap_garanti_group(lowestGroup);
						hasBeenImproved=1;
					}else{
						serv[firstServer][3]=highestGroup;
					}
				}
			}

			if(groupeRangee[highestGroup][i]==1 && groupeRangee[lowestGroup][i]==1){
				//tentative d'échange de serveur
				firstServer=-1;
				secondServer=-1;
				for(j=0; j<NB_SERV;j++){
					if ( serv[j][3] == highestGroup && serv[j][5] != -1 && serv[j][4]==i) {
						firstServer=j;
					}
					if ( serv[j][3] == lowestGroup && serv[j][5] != -1 && serv[j][4]==i) {
						firstServer=j;
					}
					if(firstServer!=-1 && secondServer!=-1){
						break;
					}
				}

				if(firstServer!=-1 && secondServer!=-1){
					serv[firstServer][3]=lowestGroup;
					serv[secondServer][3]=highestGroup;
					if(cap_garanti_group(highestGroup)>previousScore && cap_garanti_group(lowestGroup)>previousScore){
						fprintf(stderr,"Serveurs echanges\n");
                                                groupeCap[highestGroup]=cap_garanti_group(highestGroup);
						groupeCap[lowestGroup]=cap_garanti_group(lowestGroup);
						hasBeenImproved=1;
					}else{
						serv[firstServer][3]=highestGroup;
						serv[secondServer][3]=lowestGroup;
					}
				}				
			}
		}
	} 	
}

void optimizer_serveur2(){
	int i,j;
	int groupeCap[NB_GROUP];
	int hasBeenImproved;
	int groupeRangee[NB_GROUP][NB_RANGEE];
	int lowestGroup, lowestValue;
	int value;
	int previousScore;
	int maxg;
	int protectedRangee[NB_RANGEE];
	int currentGroup;
	/*hold previous position*/
	int transformation, rangee, firstServer, secondServer;

	for(i=0; i<NB_SERV;i++){
		if(serv[i][5] !=-1){
			groupeRangee[serv[i][3]][serv[i][4]]=1;
		}
	}

	for(i=0; i<NB_GROUP;i++){
		groupeCap[i]=cap_garanti_group(i);
	}

	hasBeenImproved=1;
	while(hasBeenImproved==1){
		hasBeenImproved=0;

		memset(protectedRangee,'\0',sizeof(protectedRangee));
		
		lowestGroup=0;
		lowestValue=groupeCap[0];

		for(i=1; i<NB_GROUP; i++){
			value= groupeCap[i];
			if(value<lowestValue){
				lowestGroup=i;
				lowestValue=value;
			}
		}

		previousScore=lowestValue;
		fprintf(stderr,"lowestGroup %d, lowestValue %d\n", lowestGroup, lowestValue);
		/*determiner rangee à ne pas traiter*/
		maxg=-1;
		for(i=1; i<NB_SERV;i++){
			if ( serv[i][3] == lowestGroup && serv[i][5] != -1 ) {
				if(maxg==-1 || maxg<serv[i][1]){
					maxg=serv[i][1];
				}
			}
		}
		for(i=1; i<NB_SERV;i++){
			if ( serv[i][3] == lowestGroup && serv[i][5] != -1 ) {
				if(maxg==serv[i][1]){
					protectedRangee[serv[i][4]]=1;
				}
			}
		}

		/*test de toute les rangee a améliorer*/
		for(currentGroup=0;currentGroup<NB_GROUP;currentGroup++){
		if(hasBeenImproved==1){
			break;
		}
		if(currentGroup==lowestGroup){
			continue;
		}
		for(i=0; i<NB_RANGEE; i++){
			if(protectedRangee[i]==1){
				continue;
			}
			if(groupeRangee[currentGroup][i]==1 && groupeRangee[lowestGroup][i]==0){
				//tentative de changement de groupe des serveurs
				firstServer=-1;
				for(j=0; j<NB_SERV;j++){
					if ( serv[j][3] == currentGroup && serv[j][5] != -1 && serv[j][4]==i && serv[j][1] <= CapRangeImpose) {
						firstServer=j;
						serv[firstServer][3]=lowestGroup;
						break;
					}
				}

				if(firstServer!=-1){
					if(cap_garanti_group(currentGroup)>previousScore && cap_garanti_group(lowestGroup)>previousScore){
						fprintf(stderr,"Serveur ajoute \n");
                                                groupeRangee[currentGroup][i]=0;
						groupeRangee[lowestGroup][i]=1;
						groupeCap[currentGroup]=cap_garanti_group(currentGroup);
						groupeCap[lowestGroup]=cap_garanti_group(lowestGroup);
						hasBeenImproved=1;
						break;
					}else{
						serv[firstServer][3]=currentGroup;
					}
				}
			}

			if(groupeRangee[currentGroup][i]==1 && groupeRangee[lowestGroup][i]==1){
				//tentative d'échange de serveur
				firstServer=-1;
				secondServer=-1;
				for(j=0; j<NB_SERV;j++){
					if ( serv[j][3] == currentGroup && serv[j][5] != -1 && serv[j][4]==i && serv[j][1] <= CapRangeImpose) {
						firstServer=j;
					}
					if ( serv[j][3] == lowestGroup && serv[j][5] != -1 && serv[j][4]==i && serv[j][1] <= CapRangeImpose) {
						secondServer=j;
					}
				}

				if(firstServer!=-1 && secondServer!=-1){
					serv[firstServer][3]=lowestGroup;
					serv[secondServer][3]=currentGroup;
					if(cap_garanti_group(currentGroup)>previousScore && cap_garanti_group(lowestGroup)>previousScore){
						fprintf(stderr,"Serveurs echanges\n");
                                                groupeCap[currentGroup]=cap_garanti_group(currentGroup);
						groupeCap[lowestGroup]=cap_garanti_group(lowestGroup);
						hasBeenImproved=1;
						break;
					}else{
						serv[firstServer][3]=currentGroup;
						serv[secondServer][3]=lowestGroup;
					}
				}				
			}
		}
		}
	} 
}