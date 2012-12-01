#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NDEALS      1 /* Numero di mani */
#define NPLAYERS    4   /* Numero di giocatori */
#define NCARDS	  10  /*numero carte di gioco*/
#define TRUE		  1
#define FALSE       0

typedef enum{spade, debari, bastoni, coppe}  cqfp;
typedef enum{quattro,cinque,sei,sette,otto,nove,fante,cavallo,re,asso,due,tre} cardvalue;


struct card {
    int     carta;
    cqfp    seme;
	cardvalue valore;
};

typedef struct card card;

struct player {
	//carte giocatore
	card cards[NCARDS];
	//nome
	int nome;
};

typedef struct player player;

card assign_values(int punti, cqfp seme, cardvalue valore);
//void prn_card_values(card * c_ptr);
void play_tresette(card mazzo[40]);
void shuffle(card mazzo[52]);
void swap(card *p, card *q);

int main(void)
{

cqfp seme;
cardvalue valore;
int i, carta;
card mazzo[40];

for (i=0; i<40; ++i) {
    punti = i % 10 + 1;
	switch (carta) {
		case 1:
		valore=asso;
		break;
		case 2:
		valore=due;
		break;
		case 3:
		valore=tre;
		break;
		case 4:
		valore=quattro;
		break;
		case 5:
		valore=cinque;
		break;
		case 6:
		valore=sei;
		break;
		case 7:
		valore=sette;
		break;
		case 8:
		valore=fante;
		break;
		case 9:
		valore=cavallo;
		break;
		case 10:
		valore=re;
		break;
		default:
		break;
	}
    if (i < 10) seme = cuori;
    else if (i < 20) seme = quadri;
    else if (i < 30) seme = fiori;
    else seme = picche;
    mazzo[i] = assign_values(carta, seme, valore);
}

/*for (i=26; i< 39; ++i) // stampa i fiori 
    prn_card_values(&mazzo[i]);*/
    
play_tresette(mazzo);

return 0;

}

card assign_values(int punti, cqfp seme, cardvalue valore)
{
    card c;
    c.punti = punti;
    c.seme = seme;
	c.valore = valore; 
    return c;
}


void play_tresette(card mazzo[40])
{

	mazzo_cnt = 0;
    int i,j,k;
    //card hand[NPLAYERS][5];
	//*********//
	//giocatori//
	//*********//
    
    srand(time(NULL));
    
	for(i=0; i<NDEALS; ++i) {
		players=(player*)calloc(NPLAYERS,sizeof(player));
        shuffle(mazzo);
		//numero di giocatori
		for(j=0; j<NPLAYERS; j++){
			//distribuisco le carte ai giocatori			
			for(k=0; k<NCARDS; k++){
				players[j].cards[k]=mazzo[mazzo_cnt];
				mazzo_cnt++;
			}
			//ordino le carte di ogni giocatore
			//order_cards(players[j].cards);
			players[j].nome=j;			
		}
		mazzo_cnt=0;
		free(players);
	}
	//calcolo il giocatore migliore di tutte le deals
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
