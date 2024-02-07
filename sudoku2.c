#include<stdio.h>
#include<time.h>
#include<stdlib.h>


#define NR 9
#define NC 9
#define MAX 9
#define MIN 1
#define J2 0
#define MARKER '*'
#define VUOTO ' '
#define CORNER '+'
#define ORZ '-'
#define VRT '|'
#define SPAZIO_VUOTO "  "

#define VALIDA -1
#define CORRETTA 1
#define ERRATA 0
#define MAXRIP 40
#define MAXELIM 64
#define MINELIM 36
#define MINPRES 8
// inserisci val, cancella val, genera nuovo sudoku, riavvia, esci, verifica sol 
void buca(int[][NC], int*);//
int verifica_elem(int[][NC], int, int, int*);//
void resolver(int[][NC], int[][NC], int);
void gen_sol(int[][NC], int *);//
void gen_sudoku(int[][NC], char[][NC], int*);//
void inserisci(int[][NC], char[][NC], int*);//
void elimina(int[][NC], char[][NC], int*);//
void clear(int[][NC], char[][NC], int*);
int verifica(int[][NC], int*);//
void stampa(int[][NC], char[][NC]);//
void stampa_riga();//
void stampa_esito(int);//
void menu();//
int main()
{
	int scelta = 1;
	int sudoku[NR][NC], nvuoti;
	int ris[NR][NC];
	int esito = VALIDA;
	char maschera[NR][NC];
	do
	{
		switch(scelta){
			case 1:{
				gen_sudoku(sudoku, maschera, &nvuoti);
				stampa(sudoku, maschera);
				break;
			}
			case 2:{
				inserisci(sudoku, maschera, &nvuoti);
				if(!nvuoti){
					esito = verifica(sudoku, &nvuoti);
					stampa_esito(esito);
				}
				stampa(sudoku, maschera);
				break;
			}
			case 3:{
				elimina(sudoku, maschera, &nvuoti);
				stampa(sudoku, maschera);
				break;
			}
			case 4:{
				clear(sudoku, maschera, &nvuoti);
				stampa(sudoku, maschera);
				break;
			}	
			case 5:{
				esito = verifica(sudoku, &nvuoti);
				stampa_esito(esito);
				stampa(sudoku, maschera);
				break;
			}
			case 6:
			{
				printf("\tRisolvo...\n");
				clear(sudoku, maschera, &nvuoti);
				resolver(sudoku, ris, nvuoti);
				stampa(ris, maschera);
				break;
			}
			case 0:
				break;
			default:
				printf("ERRORE\n");
		}
		menu();
		scanf("%d", &scelta);
		if(esito == CORRETTA){
			do
			{
				printf("Partita terminata. Premere 1 per continuare, 0 per terminare\n"); 
				scanf("%d", &scelta);
			}while(scelta != 0 && scelta != 1);
		}
	}while(scelta);
	return 0;
}
void menu()
{
	printf("\n1 - avvia una nuova partita");
	printf("\n2 - inserisci valore");
	printf("\n3 - cancella valore");
	printf("\n4 - riavvia partita attuale");
	printf("\n5 - verifica la soluzione attuale");
	printf("\n6 - ottieni soluzione del sudoku attuale");
	printf("\n0 - esci\n");
	printf("Inserire la scelta: ");
}
void resolver(int sudoku[][NC], int ris[][NC], int nvuoti)
{
    int i, j, k, val;
    int rep, valid;
    for(i = 0; i < NR; i++)
    	for(j = 0; j < NC; j++)
    		ris[i][j] = sudoku[i][j];
    for(i = 0; i < NR; i++){
    	valid = 1;
        for(j = 0, rep = 0; j < NC && valid; j++)
            if(sudoku[i][j] == J2)
            {
                val = rand() % (MAX - MIN + 1) + MIN;
                ris[i][j] = val;
                if(verifica_elem(ris, i, j, &nvuoti))
                    rep = 0;
                else{
                	rep++;
                    j--;
                    if(rep > MAXRIP){
                    	
	                    for(k = 0; k < NR; k++)
							ris[i][k] = sudoku[i][k];
						i = -1;
                    	valid = 0;
            		}
            	}
        	}
	}
}

void gen_sudoku(int schema[][NC], char maschera[][NC], int* nvuoti)
{
	int i, j;
	
	gen_sol(schema, nvuoti);
 	buca(schema, nvuoti);
 	for(i = 0; i < MAX; i++)
 		for(j = 0; j < MAX; j++)
 			maschera[i][j] = VUOTO;
}
void inserisci(int sudoku[][NC], char maschera[][NC], int *nvuoti)
{
	int i,j,num;
	printf("\nInserire la riga: ");
	scanf("%d",&i);	
	printf("\nInserire la colonna: ");
	scanf("%d",&j);
	printf("\nInserire il numero da caricare: ");
	scanf("%d",&num);
	if(num >= MIN && num <= MAX && sudoku[i-1][j-1] == 0 && *nvuoti != 0)
	{
		sudoku[i - 1][j - 1] = num;
		maschera[i - 1][j - 1] = MARKER;
		(*nvuoti)--;
	}
	else
		printf("\nCASELLA NON ACCESSIBILE\n\n");
}
void elimina(int sudoku[][NC], char maschera[][NC], int *nvuoti)
{
	int i,j;
	printf("\nInserire la riga: ");
	scanf("%d",&i);	
	printf("\nInserire la colonna: ");
	scanf("%d",&j);
	if(maschera[i - 1][j - 1] == MARKER)
	{
		sudoku[i - 1][j - 1] = 0;
		maschera[i - 1][j - 1] = VUOTO;
		(*nvuoti)++;
	}
	else
		printf("\nCASELLA NON ACCESSIBILE\n\n");
}
void gen_sol(int schema[][NC], int *nvuoti)
{
	int i, ripetizioni, j, ris, valid, is_inf,a,b;
	srand(time(NULL));
	*nvuoti = 81;
	for(i = 0; i < MAX; i++){
		for(j = 0; j < MAX; j++){
			schema[i][j] = 0;
		}
	}
	
	for(i = 0; i < MAX; i++){
		valid = 1;
		for(j = 0; j < MAX && valid; j++)
		{
            is_inf = 0;
			ripetizioni = 0;
			do{
				schema[i][j] = rand() % (MAX - MIN + 1) + MIN;


				ris = verifica_elem(schema, i, j, nvuoti);
				if(ris){
					(*nvuoti)--;
				}
				else
                    ripetizioni++;
				if(ripetizioni > MAXRIP && !ris){
                    for(a = 0; a < MAX ; a++)
                        for(b = 0; b < MAX; b++)
                            schema[a][b] = 0;
                    *nvuoti = 81;
                    i = -1;
                    j = -1;
					is_inf = 1;
					valid = 0;
				}
			}while(ris == 0 && !is_inf);
		}
	}
}
int verifica_elem(int sudoku[][NC], int i, int j, int *nvuoti)
{
	int istart, jstart, valid, h, k;
	istart = i - i % 3;
	jstart = j - j % 3;
	if(*nvuoti){
		valid = -1;
		for(h = 0; h < NR && valid; h++)
			if(sudoku[h][j] != J2 && (sudoku[h][j] == sudoku[i][j] && i != h))
				valid = 0;
		for(h = 0; h < NC && valid; h++)
			if(sudoku[i][h] != J2 && (sudoku[i][h] == sudoku[i][j] && j != h))
				valid = 0;
		
		for(h = istart; h < istart + 3 && valid; h++)
			for(k = jstart; k < jstart + 3 && valid; k++)
				if(sudoku[h][k] != J2 && sudoku[h][k] == sudoku[i][j] && i != h && j != k)
					valid = 0;
	}
	else{
		valid = 1;
		for(h = 0; h < NR && valid; h++)
			if((sudoku[h][j] == sudoku[i][j] && i != h))
				valid = 0;
		for(h = 0; h < NC && valid; h++)
			if((sudoku[i][h] == sudoku[i][j] && j != h))
				valid = 0;
		
		for(h = istart; h < istart + 3 && valid; h++)
			for(k = jstart; k < jstart + 3 && valid; k++)
				if(sudoku[h][k] == sudoku[i][j] && i != h && j != k)
					valid = 0;
	}
	return valid;
}
void buca(int schema[][NC], int *nvuoti)
{
	int target_el, nelim, i, j, isnum, conta[MAX], presenti;
	target_el = rand() % (MAXELIM- MINELIM + 1) + MINELIM;
	*nvuoti = target_el;
	do
	{
		for(i = 0; i < MAX; i++)
			conta[i] = 0;
		presenti = 0;
		do
		{
			for(i = 0; i < MAX && nelim != target_el; i++)
				for(j = 0; j < MAX && nelim != target_el; j++){
					isnum = rand() % 2;
					if(schema[i][j] != 0)
					{
						if(isnum)
							conta[schema[i][j]-1]++;
						else{
							schema[i][j] = 0;
							nelim++;
						}
					}
					
				}
			for(i = 0; i < MAX; i++)
				if(conta[i] != 0)
					presenti++;
		}while(nelim != target_el);
	}while(presenti < MINPRES);
}
int verifica(int sudoku[][NC], int *nvuoti)
{
	int i, j, conta[MAX], valido, h, k, iconta;
	if(*nvuoti)
	{
		valido = VALIDA;
		for(i = 0; i < MAX; i++)
			conta[i] = 0;
		for(i = 0; i < NR && valido; i++)
		{
			for(j = 0; j < NC; j++)
				if(sudoku[i][j] != J2)
					conta[sudoku[i][j]-1]++;
			for(j = 0; j < MAX && valido; j++)
			{
					if(conta[j] > 1)
						valido = ERRATA;
					else 
						if(conta[j])
							conta[j]--;
			}
			for(j = 0; j < NC && valido; j++)
				if(sudoku[i][j] != J2)
					conta[sudoku[j][i]-1]++;
			for(j = 0; j < MAX && valido; j++)
			{
					if(conta[j] > 1)
						valido = ERRATA;
					else 
						if(conta[j])
							conta[j]--;
			}
		}
		for(i = 0; i < NR && valido; i+=3)
		{
			for(j = 0; j < NC && valido; j+=3)
			{
				for(h = i; h < (i + 3);h++)
				{
					for(k = j; k < (j+3); k++)
					{
						if(sudoku[h][k] != J2)
						{
							iconta = sudoku[h][k]-1;
							conta[iconta]++;
						}
					}
				}
				for(h = 0; h < MAX && valido; h++)
				{
					if(conta[h] == 1)
						conta[h] = 0;
					else if(conta[h] > 1)
						valido = ERRATA;
				}
			}
		}
	}
	else
	{
		valido = CORRETTA;
		for(i = 0; i < MAX; i++)
			conta[i] = 0;
		for(i = 0; i < NR && valido; i++)
		{
			for(j = 0; j < NC; j++)
				conta[sudoku[i][j]-1]++;
			for(j = 0; j < MAX && valido; j++)
			{
					if(conta[j] != 1)
						valido = ERRATA;
					else
						conta[j]--;
			}
			for(j = 0; j < NR && valido; j++)
			{
				conta[sudoku[j][i]-1]++;
			}	
			for(j = 0; j < MAX && valido; j++){
				if(conta[j] != 1)
					valido = ERRATA;
				else
					conta[j]--;
			}
		}
		for(i = 0; i < NR && valido; i+=3)
		{
			for(j = 0; j < NC && valido; j+=3)
			{
				for(h = i; h < (i + 3);h++)
				{
					for(k = j; k < (j+3); k++)
					{
						iconta = sudoku[h][k]-1;
						conta[iconta]++;
					}
				}
				for(h = 0; h < MAX && valido; h++)
				{
					if(conta[h] == 1)
						conta[h] = 0;
					else
						valido = ERRATA;
				}
			}
		}
	}

	return valido;	
}

void stampa(int sudoku[][NC], char maschera[][NC])
{

	int i, j;

	for(i = 0;i < NR;i++)
	{
		if(i == 0 || i == 3 || i == 6)
			stampa_riga();
		printf("%c",VRT);
		for(j = 0;j < NC;j++)
		{
			if(j == 3 || j == 6)
				printf("%c",VRT	);
			if(sudoku[i][j] != 0)
			{
				printf("%d", sudoku[i][j]);
				printf("%c", maschera[i][j]);
			}
			else
				printf("%s",SPAZIO_VUOTO);
		}
		printf("%c",VRT);
		printf("\n");
	}
	stampa_riga();
}

void stampa_riga()
{
	int i,j;
	for(j = 0;j < 3;j++){		// 3 blocchi principali
		printf("%c",CORNER);
		for(i = 0;i < 6;i++)	// 6 caratteri nel blocco
			printf("%c",ORZ);
	}
	printf("%c\n",CORNER);
}
void stampa_esito(int esito)
{
	if(esito == VALIDA)
		printf("\n\tVALIDA\n");
	else{
		if(!esito)
			printf("\n\t ERRATA\n");
		else{
			printf("\n\t CORRETTA \n");
		}
	}
}
void clear(int sudoku[][NC], char maschera[][NC], int* nvuoti)
{
	int i, j;
	
	for(i = 0; i < MAX; i++)
		for(j = 0; j < MAX; j++)
			if(maschera[i][j] == MARKER){
				sudoku[i][j] = 0;
				(*nvuoti)++;
				maschera[i][j] = VUOTO;
			}
}

