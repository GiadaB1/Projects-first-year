#include<stdio.h>
#define LEN 81
#define NR 9
#define NC 9
#define MAX 9
#define MARKER '*'
#define J1 '_'
#define J2 '0'
#define CORNER '+'
#define ORZ '-'
#define VRT '|'
#define VALIDA -1
#define CORRETTA 1
#define ERRATA 0
#define VUOTO ' '
#define SPAZIO_VUOTO "  "
#define MIN 1
#define MAXLEN 9
int menu();//FATTO
void ins_schema(char[],int *);//FATTO
void stampa(int [][NC],char [][NC]);// DA FINIRE
void inserisci_v(int [][NC],char [][NC],int*, int*);//FATTO
void rimuovi_v(int [][NC],char [][NC],int*);//FATTO
void verifica(int [][NC], int*, int*);// -1 valido 0 errore 1 corr FATTO 
void carica(char[],int[][NC],int*, int*);//FATTO
void carica_verifica(char[],int[][NC], int*, char[][NC], int*); //DA FARE(non funziona reset se carichi soluzione completa)
void reset(int [][NC],char[][NC],int*);//FATTO
void stampa_riga();//DA FINIRE
int is_compatible(char[], char[], int*);


int main(int argc,char * argv[])
{
	int scelta = 1;
	char schema[LEN+1],maschera[NR][NC];
	int sudoku[NR][NC],i,j,esito;
	int n_vuoti;
	int continua;
	for(i = 0;i < NR; i++)
		for(j = 0; j < NC;j++)
			maschera[i][j] = VUOTO;
	esito = 0;
	do
	{	
		if(scelta == 1)
		{
			
			ins_schema(schema,&n_vuoti);
			schema_valido = controllo_schema(schema);
			carica(schema,sudoku,&n_vuoti, &esito);
			stampa(sudoku,maschera);
		}
		else
			if(scelta == 2)
			{
				inserisci_v(sudoku,maschera,&n_vuoti, &esito);
				stampa(sudoku,maschera);
			}
			else
				if(scelta == 3)	
				{
					rimuovi_v(sudoku,maschera,&n_vuoti);
					stampa(sudoku,maschera);
				}
				else
					if(scelta == 4)
					{
						verifica(sudoku, &n_vuoti, &esito);
						stampa(sudoku,maschera);
					}
					else
						if(scelta == 5)
						{
							carica_verifica(schema,sudoku, &n_vuoti, maschera, &esito);
							stampa(sudoku,maschera);
							reset(sudoku, maschera, &n_vuoti);
						}
						else
							if(scelta == 6)
							{
								reset(sudoku,maschera,&n_vuoti);
								stampa(sudoku,maschera);
							}
							else
								printf("\nVALORE NON VALIDO, RIPROVA.\n");
		if(esito == 1){
			printf("\nVuoi continuare a giocare?\n");
			scanf("%d", &continua);
			if(continua == 1){
				scelta = 1;
				reset(sudoku, maschera, &n_vuoti);
				esito = 0;
			}
			else{
				scelta = 6;
			}
		}
		else
			scelta = menu();
		getchar();
	}while(scelta);

	return 0;
}

int menu()
{
	int scelta;
	printf("\n1 - avvia una nuova partita");
	printf("\n2 - inserisci valore");
	printf("\n3 - cancella valore");
	printf("\n4 - verifica la soluzione attuale");
	printf("\n5 - carica una soluzione e verificala");
	printf("\n6 - riavvia partita attuale");
	printf("\n0 - esci\n");
	printf("Inserire la scelta: ");
	scanf("%d",&scelta);
	return scelta;
}

void inserisci_v(int sudoku[][NC],char maschera[][NC],int * n_vuoti, int*esito)
{
	int i,j,num;
	printf("\nInserire la riga: ");
	scanf("%d",&i);	
	printf("\nInserire la colonna: ");
	scanf("%d",&j);
	printf("\nInserire il numero da caricare: ");
	scanf("%d",&num);
	if(num >= MIN && num <= MAX && maschera[i][j] != MARKER && *n_vuoti != 0)
	{
		sudoku[i - 1][j - 1] = num;
		maschera[i - 1][j - 1] = MARKER;
		(*n_vuoti)--;
		printf("%d\n",*n_vuoti);
	}
	else
		printf("\nCASELLA NON ACCESSIBILE\n\n");
	if(!(*n_vuoti))
		verifica(sudoku, n_vuoti, esito);
}
void rimuovi_v(int sudoku[][NC],char maschera[][NC],int * n_vuoti)
{
	int i,j,num;
	printf("\nInserire la riga: ");
	scanf("%d",&i);	
	printf("\nInserire la colonna: ");
	scanf("%d",&j);
	if(maschera[i - 1][j - 1] == MARKER)
	{
		sudoku[i - 1][j - 1] = 0;
		maschera[i - 1][j - 1] = VUOTO;
		(*n_vuoti)++;
	}
	else
		printf("\nCASELLA NON ACCESSIBILE\n\n");
}

void reset(int sudoku [][NC],char maschera[][NC],int *n_vuoti)
{
	int i,j;
	for(i = 0;i < NR;i++)
		for(j = 0; j < NC;j++)
			if(maschera[i][j] == MARKER)
			{
				maschera[i][j] = VUOTO;
				sudoku[i][j] = 0;
				(*n_vuoti)++;
			}
}

void verifica(int sudoku[][NC], int *n_vuoti, int*esito)
{
	int i, j, conta[MAX], valido, h, k, iconta;
	if(*n_vuoti)
	{
		
		valido = VALIDA;
		for(i = 0; i < MAX; i++)
			conta[i] = 0;
		for(i = 0; i < NR && valido; i++)
		{
			for(j = 0; j < NC; j++)
				if(sudoku[i][j] != J1 && sudoku[i][j] != J2)
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
				if(sudoku[i][j] != J1 && sudoku[i][j] != J2)
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
		for(i = 0; i < NR-3 && valido; i+=3)
		{
			for(j = 0; j < NC-3 && valido; j+=3)
			{
				for(h = i; h < (i + 3);h++)
				{
					for(k = j; k < (j+3); k++)
					{
						if(sudoku[h][k] != J1 && sudoku[h][k] != J2)
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
		for(i = 0; i < NR-3 && valido; i+=3)
		{
			for(j = 0; j < NC-3 && valido; j+=3)
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
	if(valido == VALIDA)
		printf("\n\tVALIDA\n");
	else 
		if(!valido)
			printf("\n\t ERRATA\n");
		else
			printf("\n\t CORRETTA \n");
	*esito = valido;
}

void ins_schema(char schema[], int * n_vuoti)
{

	int i, flag, null;
	do{
		printf("\nInserire lo schema iniziale: ");
		gets(schema);
		flag = 0;
		null = 0;
		for(i = 0;schema[i] != '\0' && flag == 0;i++)
		{
			if((schema[i] < '0' || schema[i] > '9' ) && (schema[i] != J1 && schema[i] != J2))	
				flag = 1;
			if(schema[i] == J1 || schema[i] == J2)
				null++;
		}
	}while(flag != 0);
	*n_vuoti = null;
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

void carica(char schema[], int sudoku[][NC], int *n_vuoti, int*esito)
{
	int i,j,k;
	k = 0;
	for(i = 0;i < NR;i++)
	{
		for(j = 0;j < NC;j++)
		{
			if(schema[k] != J1)
			{
				sudoku[i][j] = schema[k] - '0';
			}
				else
					sudoku[i][j] = 0;
				
			k++;
		}
	}
	if(*n_vuoti == 0)
		verifica(sudoku, n_vuoti, esito); 
}
void carica_verifica(char schema[],int sudoku[][NC], int *n_vuoti, char maschera[][NC], int*esito)
{
	char soluzione[LEN + 1];
	int ris, i, j, k;
	printf("\nInserire la soluzione da verificare: ");
	gets(soluzione);
	ris = is_compatible(soluzione, schema, n_vuoti);
	if(ris)
	{
		carica(soluzione, sudoku, n_vuoti, esito);
		if((*n_vuoti) != 0)
			verifica(sudoku, n_vuoti, esito);
		k = 0; 
		for(i = 0; i < NR; i++)
		{
			for(j = 0; j < NC; j++)
			{
				if(soluzione[k] != schema[k])
					maschera[i][j] = MARKER;
				else
					maschera[i][j] = VUOTO;
				k++;
			}
		}
	}
	else
	{
		printf("\n\nLa soluzione inserita non è compatibile con lo schema iniziale\n\n");
	}
}

int is_compatible(char soluzione[], char schema[], int* n_vuoti){ //verifica che soluzione ha gli stessi caratteri di schema nelle posizioni dove non ci sono trattini
	int valid, i;
	valid = 1;
	for(i = 0; schema[i] != '\0' && valid; i++)
	{
		if(schema[i] != J1 && schema[i] != J2 && schema[i] != soluzione[i])
			valid = 0; 
		else 
			if(soluzione[i] != J1 && soluzione[i] != J2 && (schema[i] == J1 || schema[i] == J2))
				(*n_vuoti)--;
	}
	return valid;
}