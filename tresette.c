#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NDEALS      100 /* Numero di mani */
//#define NPLAYERS    2   /* Numero di giocatori */
//#define NCARDS	  20  /*numero carte di gioco*/
#define TRUE		  1
#define FALSE       0

int NCARDS=0;
int NPLAYERS=0;

typedef enum{spade, denari, bastoni, coppe}  cqfp;
typedef enum{nullo,quattro,cinque,sei,sette,fante,cavallo,re,asso,due,tre} cardvalue;


struct card {
    int     carta;
    cqfp    seme;
	cardvalue valore;
	char *nome;
};

typedef struct card card;

struct player {
	//carte giocatore
	card *cards;
	//carta giocata
	card *carta_giocata;
	//nome
	int nome;
	//punteggio
	int punteggio;
	//tipo umano o computer
	char tipo[100];
};

typedef struct player player;

card assign_values(int carta, cqfp seme, cardvalue valore, char *nome);
//void prn_card_values(card * c_ptr);
void play_tresette(card mazzo[40]);
char *getSeme(cqfp seme);
int is_x(int carta);
void set_score(player *wp,card *carte_giocate);
player *check_score(player *p,player *wp,int confronta);
void order_cards(card *cards);
void shuffle(card mazzo[52]);
void swap(card *p, card *q);

int main(void)
{

cqfp seme;
cardvalue valore;
char *nome;
int i, carta;
card mazzo[40];

//set valori in base alle regole di gioco e nomi delle carte
for (i=0; i<40; ++i) {
    carta = i % 10 + 1;
	//printf(".carta %d ",carta);
	switch (carta) {
		case 1:valore=asso;nome="asso";break;
		case 2:valore=due;nome="due";break;
		case 3:valore=tre;nome="tre";break;
		case 4:valore=quattro;nome="quattro";break;
		case 5:valore=cinque;nome="cinque";break;
		case 6:valore=sei;nome="sei";break;
		case 7:valore=sette;nome="sette";break;
		case 8:valore=fante;nome="fante";break;
		case 9:valore=cavallo;nome="cavallo";break;
		case 10:valore=re;nome="re";break;
		default:break;
	}
	//seme
    if (i < 10) seme = spade;
    else if (i < 20) seme = denari;
    else if (i < 30) seme = bastoni;
    else seme = coppe;
    mazzo[i] = assign_values(carta, seme, valore, nome);
}
printf("\n");
/*for (i=26; i< 39; ++i) // stampa i fiori 
    prn_card_values(&mazzo[i]);*/
	//numero giocatori
while(NPLAYERS!=2 && NPLAYERS!=4){
	printf("Inserire il numero di giocatori (2 o 4): ");
	if(scanf("%d",&NPLAYERS)==0){
		printf("Hai inserito un valore non valido!\n");
		break;
		exit(0);
	}
}
	//distribuzione delle carte in base al num di giocatori
if(NPLAYERS==2)NCARDS=20;
else if(NPLAYERS==4)NCARDS=10;
printf("Inizio la partita con %d giocatori\n\n",NPLAYERS);

play_tresette(mazzo);

return 0;

}

//costruisco le carte
card assign_values(int carta, cqfp seme, cardvalue valore, char *nome)
{
    card c;
    c.carta = carta;
    c.seme = seme;
	c.valore = valore; 
	c.nome = nome;
    return c;
}

//gioco tresette
void play_tresette(card mazzo[40])
{

	int mazzo_cnt = 0;
    int i,j,k;
    //card hand[NPLAYERS][5];
	//*********//
	//giocatori//
	//*********//
    
    srand(time(NULL));
	
	//somma dei punteggi
    int punteggio_pc,punteggio_sc,punteggio_pg,punteggio_sg;
	punteggio_pc=punteggio_sc=punteggio_pg=punteggio_sg=0;
	/////////////////////////////////
	
	for(i=0; i<NDEALS; ++i) {
		player *players;
		players=(player*)calloc(NPLAYERS,sizeof(player));
        shuffle(mazzo);
		//numero di giocatori
		for(j=0; j<NPLAYERS; j++){
			//distribuisco le carte ai giocatori	
			int nome;
			players[j].cards=(card*)calloc(NCARDS,sizeof(card));
			players[j].nome=j;
			nome=players[j].nome;
			players[j].punteggio=0;
			printf("giocatore %d\n",nome);
			while(strcmp(players[j].tipo,"human")!=0 && strcmp(players[j].tipo,"computer")!=0){
				printf("Il giocatore %d gioca come umano o come computer? (human || computer): ",nome);
				scanf("%s",&players[j].tipo);
			}
			for(k=0; k<NCARDS; k++){
				players[j].cards[k]=mazzo[mazzo_cnt];
				int valore;
				char *nome_carta,*nome_seme;
				valore=players[j].cards[k].valore;
				nome_carta=players[j].cards[k].nome;
				nome_seme=getSeme(players[j].cards[k].seme);
				printf("carta %s di %s con valore %d\n",nome_carta,nome_seme,valore);
				mazzo_cnt++;
			}
			//ordino le carte di ogni giocatore
			//order_cards(players[j].cards);
						
		}
		//gioco
		int passata,posizione;//carte rimaste!
		posizione=0;
		//per ogni passata di tresette
		for(passata=NCARDS;passata>0;passata--){
			printf("PASSATA NUMERO %d\n",passata);
			card *carte_giocate;
			player *winner;
			carte_giocate=(card*)calloc(NPLAYERS,sizeof(card));
			for(j=0; j<NPLAYERS; j++){
				int carta,valore,seme;
				char *nome_carta,*nome_seme;
				printf("giocatore %d\n",players[j].nome);
				//scelgo la carta
				int carta_scelta;
				//se giocatore umano
				if(strcmp(players[j].tipo,"human")==0){
					printf("Scegli una carta tra:\n");
					int f;
					//per le carte rimaste xhiedo di selezionare una carta
					for(f=posizione;f<NCARDS;f++)
						printf("%s di %s (%d per selezionare)\n",players[j].cards[f].nome,getSeme(players[j].cards[f].seme),f);
					scanf("%d",&carta_scelta);
				}
				//se computer scelgo una carta tra qulle rimaste
				else carta_scelta=rand()%passata+posizione;
				
				carta=players[j].cards[carta_scelta].carta;
				valore=players[j].cards[carta_scelta].valore;
				nome_carta=players[j].cards[carta_scelta].nome;
				nome_seme=getSeme(players[j].cards[carta_scelta].seme);
				printf("carta scelta %s di %s con valore %d\n",nome_carta,nome_seme,valore);
				//metto la carta sul piatto
				carte_giocate[j]=players[j].cards[carta_scelta];
				//assegno al giocatore la carta giocata
				players[j].carta_giocata=&carte_giocate[j];
				//aggiorno vincitore
				if(j>0){
					printf("confronto giocatore %d che ha giocato un %s di %s con giocatore %d\n",players[j].nome,nome_carta,nome_seme,winner->nome);
					winner=check_score(&players[j],winner,TRUE);
					printf("vince giocatore %d con un %s di %s \n",winner->nome,winner->carta_giocata->nome,getSeme(winner->carta_giocata->seme));
				}
				else winner=check_score(&players[j],winner,FALSE);
				//elimino la carta giocata
				players[j].cards[carta_scelta].carta=0;
				order_cards(players[j].cards);
			}
			//aggiungo punteggio a giocatore vincente
			set_score(winner,carte_giocate);
			free(carte_giocate);
			++posizione;
		}
		//stampo eventuali vincite e punteggi		
		for(j=0; j<NPLAYERS; j++){
			int punteggio=players[j].punteggio/3;
			printf("giocatore %d -> punti %d\n",players[j].nome,punteggio);
			if(NPLAYERS==2){
				if(j==0)punteggio_pg+=punteggio;
				if(j==1)punteggio_sg+=punteggio;
				if(j==1){
					printf("Punteggio primo giocatore -> punti %d\n",punteggio_pg);
					printf("Punteggio secondo giocatore -> punti %d\n",punteggio_sg);
				}
			}	
			if(NPLAYERS==4){
				if(j==0 || j==2)punteggio_pc+=punteggio;
				if(j==1 || j==3)punteggio_sc+=punteggio;
				if(j==3){
					printf("Punteggio prima coppia -> punti %d\n",punteggio_pc);
					printf("Punteggio seconda coppia -> punti %d\n",punteggio_sc);
				}
			}
		}
		//azzero mano
		mazzo_cnt=0;
		free(players);
		if(punteggio_pc>=21){printf("Vince la prima coppia -> punti %d\n",punteggio_pc);exit(0);}
		else if(punteggio_sc>=21){printf("Vince la seconda coppia -> punti %d\n",punteggio_sc);exit(0);}	
		if(punteggio_pg>=21){printf("Vince il primo giocatore -> punti %d\n",punteggio_pg);exit(0);}
		else if(punteggio_sg>=21){printf("Vince il secondo giocatore -> punti %d\n",punteggio_sg);exit(0);}
	}
	//calcolo il giocatore migliore di tutte le deals
}
//return della stringa seme
char *getSeme(cqfp seme){
	char *nome_seme;
	if(seme==spade) nome_seme="spade"; 
	else if(seme==denari) nome_seme="denari"; 
	else if(seme==bastoni) nome_seme="bastoni"; 
	else if(seme==coppe) nome_seme="coppe"; 
	return nome_seme;
}
//punteggio per ogni carta
int is_x(int carta){
	switch(carta){
	case asso:return 3;break;
	case fante:
	case cavallo:
	case re:
	case due:
	case tre:
	return 1;
	break;
	default:
	return 0;
	break;
	}
}
//set del punteggio
void set_score(player *wp,card *carte_giocate){
	int i,punteggio;
	punteggio=0;
	for(i=0;i<4;i++){
		punteggio+=is_x(carte_giocate[i].valore);
	}
	wp->punteggio+=punteggio;
}
//check del giocatore vincente
//aggiorno giocatore vincente ad ogni passaggio
player *check_score(player *p,player *wp,int confronta){
	if(confronta){
		int move_p=p->nome-wp->nome;
		printf("*********confronta**********\n");
		if(wp->carta_giocata->seme==p->carta_giocata->seme){
			printf("stesso seme\n");
			if(p->carta_giocata->valore>wp->carta_giocata->valore)
				return p;
			if(wp->carta_giocata->valore>p->carta_giocata->valore){
				return p-=move_p;
			}
		}
		else return p-=move_p;
	}
	else return p;
}
//ordino le carte
void order_cards(card *cards)
{
	//variabili di appoggio per il sort
	//crescente in base a numero
	int carta,f;
	cqfp seme;
	cardvalue valore;
	char *nome;
	//iteratori
	int i, j;
	printf("Ordino le carte\n\n");
	for (i = 1; i < NCARDS; i++)
	{
		carta=cards[i].carta;
		seme=cards[i].seme;
		valore=cards[i].valore;
		nome=cards[i].nome;
		for (j = i - 1; (j >= 0) && (cards[j].carta > carta); j--)
		{
			cards[j+1].seme = cards[j].seme;
			cards[j+1].carta = cards[j].carta;
			cards[j+1].valore = cards[j].valore;
			cards[j+1].nome = cards[j].nome;
		}
		
		cards[j+1].carta=carta;
		cards[j+1].seme=seme;
		cards[j+1].valore=valore;
		cards[j+1].nome=nome;
	}
	/*
	for (f = 0; f < NCARDS; f++)
	{
		printf("!!!carta %d di ",cards[f].punti);
		print_seme(cards[f].seme);
	}*/
}

void shuffle(card mazzo[40])
{
    int i,j;
    for (i=0; i<40; ++i) {
        j = rand()%40;
        swap(&mazzo[i], &mazzo[j]);
    }
}

void swap(card *p, card *q)
{
    card tmp;
    tmp = *p;
    *p = *q;
    *q = tmp;
}
