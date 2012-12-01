#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NDEALS      1 /* Numero di mani */
#define NPLAYERS    6   /* Numero di giocatori */
#define NCARDS	  5  /*numero carte di gioco*/
#define TRUE		  1
#define FALSE       0

typedef enum{cuori, quadri, fiori, picche}  cqfp;
typedef enum{got_carta_alta,got_coppia,got_doppia_coppia,got_tris,got_scala,got_colore,got_full,got_poker,got_scala_reale,got_scala_colore}  winning;

struct card {
    int     punti;
    cqfp    seme;
};

typedef struct card card;

struct player {
	//carte giocatore
	card cards[NCARDS];
	//tipo di punteggio
	char *rank;
	//nome
	int nome;
	//valore punteggio
	winning points;
};

typedef struct player player;

struct vincite {
	int nome;
	int num_vincite;
};

typedef struct vincite vincite;

char *is_x(card *cards);
int is_twin(card *cards);
int is_double_twin(card *cards);
int is_tris(card *cards);
int is_straight(card *cards);
int is_full(card *cards);
int is_poker(card *cards);	
int is_royal_straight(card *cards);
///////////////////////////////////////////////////////////////
//calcolo dei punteggi//**//
player *set_scala_reale(player *p,player *wp);
player *set_scala_colore(player *p,player *wp);
player *set_poker(player *p,player *wp);
player *set_full(player *p,player *wp);
player *set_colore(player *p,player *wp);
player *set_scala(player *p,player *wp);
player *set_tris(player *p,player *wp);
player *set_doppia_coppia(player *p,player *wp);
player *set_coppia(player *p,player *wp);
player *set_carta_alta(player *p,player *wp);
player *set_score(player *p,player *wp,winning typeof_w);
player *check_score(player *p,player *wp,int confronta);
//**////////////////////////////////////////////////////////////
//**////////////////////////////////////////////////////////////
void order_cards(card *cards);
card assign_values(int punti, cqfp seme);
//void prn_card_values(card * c_ptr);
void play_poker(card mazzo[52]);
void shuffle(card mazzo[52]);
void swap(card *p, card *q);
void deal_the_cards(card mazzo[52], card hand[NPLAYERS][5]);
int is_flush(card h[5]); /* colore */


int main(void)
{

cqfp seme;
int i, punti;
card mazzo[52];

for (i=0; i<52; ++i) {
    punti = i % 13 + 1;
	//printf("%d\n",punti);
    if (i < 13) seme = cuori;
    else if (i < 26) seme = quadri;
    else if (i < 39) seme = fiori;
    else seme = picche;
    mazzo[i] = assign_values(punti, seme);
}

/*for (i=26; i< 39; ++i) // stampa i fiori 
    prn_card_values(&mazzo[i]);*/
    
play_poker(mazzo);

return 0;

}

//is_x assegna il numero di carte uguali x ad ogni y giocatore
//
char *is_x(card *cards){
	int i;
	int *all_cards;
	all_cards=(int*)calloc(14,sizeof(int));
	for(i=0;i<NCARDS;i++)
		all_cards[cards[i].punti]++;
	for(i=1;i<14;i++)
		//printf("carta %d -> quantità %d\n",i,all_cards[i]);
	return all_cards;
}

//coppia
int is_twin(card *cards){
	int i,j,cntrl=0,coppia=FALSE;
	for(i=0; i<NCARDS-1; i++) {
		for (j=i+1; j<NCARDS; j++) {
			if(cards[i].punti==cards[j].punti) {
				if(cntrl<1) {
					coppia=TRUE;
					cntrl++;
				}
				else {
					coppia=FALSE;
					i=NCARDS;
					break;
				}
			}
		}
	}
	return coppia;
}
//doppia coppia
int is_double_twin(card *cards) {
	
	int i,j,cntrl=0,card_rank=0,doppia_coppia=FALSE;
	for(i=0; i<NCARDS-1; i++) {
		for (j=i+1; j<NCARDS; j++) {
			if(cards[i].punti==cards[j].punti) {
				if(cntrl<2 && cards[j].punti!=card_rank) {
					card_rank=cards[i].punti;
					++cntrl;
				}
				else {
					doppia_coppia=FALSE;
					i=NCARDS;
					break;
				}
			}
		}
		if(cntrl==2){
			doppia_coppia=TRUE;
			break;
		}
	}
	return doppia_coppia;
}
//tris
int is_tris(card *cards)
{
	int *p=is_x(cards);
	int x_twin,x_tris,tris;
	x_twin=x_tris=tris=FALSE;
	int i;
	for(i=1;i<14;i++){
		if(p[i]==2){
			x_twin=TRUE;
			//printf("coppia trovata");
		}
		if(p[i]==3){
			//printf("tris trovato");
			x_tris=TRUE;
		}
	}
	if(x_twin==FALSE&&x_tris==TRUE)tris=TRUE;
	free(p);
	return tris;
}
//scala
int is_straight(card *cards){
	int i,cnt,straight=FALSE;
	for(i=0; i<NCARDS-1; i++)
		if(cards[i].punti+1==cards[i+1].punti)
			++cnt;
	if(cnt==4)
		if(cards[i].punti==13&&cards[0].punti==1)
			straight=TRUE;
	else if(cnt==5)
		straight=TRUE;
	return straight;
}
//full
int is_full(card *cards)
{
	int *p=is_x(cards);
	int x_twin,x_tris,full;
	x_twin=x_tris=full=FALSE;
	int i;
	for(i=1;i<14;i++){
		if(p[i]==2){
			x_twin=TRUE;
			//printf("coppia trovata");
		}
		if(p[i]==3){
			//printf("tris trovato");
			x_tris=TRUE;
		}
	}
	if(x_twin==TRUE&&x_tris==TRUE)full=TRUE;
	free(p);
	return full;
}
//poker
//primo metodo 
/*int is_poker(card *cards){
	int i,cnt,poker=FALSE;
	for(i=0; i<NCARDS-2; i++)
		if(cards[i].punti==cards[i+1].punti)
			++cnt;
	if(cnt==4)poker=TRUE;
	else poker=FALSE;
	return poker;
}*/
//secondo metodo utilizzando is_x
int is_poker(card *cards){
	int *p=is_x(cards);
	int poker=FALSE;
	int i;
	for(i=1;i<14;i++)if(p[i]==4)poker=TRUE;
	free(p);
	return poker;
}
//scala reale
int is_royal_straight(card *cards){
	int i,cnt,royal_straight=FALSE;
	for(i=0; i<NCARDS-1; i++)
		if(cards[i].punti+1==cards[i+1].punti)++cnt;
	if(cnt==4)
		if(cards[i].punti==13&&cards[0].punti==1)royal_straight=TRUE;
	return royal_straight;
}
//CONFRONTO DEI PUNTEGGI UGUALI/////////////////////////
///////////////////////////////////////////////////////
player *set_scala_reale(player *p,player *wp){
	int first_card=0;
	if(p->cards[first_card].seme>wp->cards[first_card].seme)return p;
	if(wp->cards[first_card].seme>p->cards[first_card].seme)return --p;
}
player *set_scala_colore(player *p,player *wp){
	int first_card=0;
	int last_card=NCARDS-1;
	if(p->cards[last_card].punti!=wp->cards[last_card].punti){
		if(p->cards[last_card].punti>wp->cards[last_card].punti)return p;
		if(wp->cards[last_card].punti>p->cards[last_card].punti){
			int move_p=p->nome-wp->nome;
			return p-=move_p;
		}
	}
	else{
		if(p->cards[0].seme>wp->cards[0].seme)return p;
		if(wp->cards[0].seme>p->cards[0].seme){
			int move_p=p->nome-wp->nome;
			return p-=move_p;
		}
	}
		
	return p;
}
player *set_poker(player *p,player *wp){
	int *p_cards=is_x(p->cards);
	int *wp_cards=is_x(wp->cards);
	int p_poker,wp_poker;
	int i;
	for(i=1;i<14;i++){
		if(p_cards[i]==4)p_poker=i;
		if(wp_cards[i]==4)wp_poker=i;
	}
	free(p_cards);
	free(wp_cards);
	if(p_poker>wp_poker)return p;
	if(wp_poker>p_poker){
		int move_p=p->nome-wp->nome;
		return p-=move_p;	
	}
}
player *set_full(player *p,player *wp){
	int *p_cards=is_x(p->cards);
	int *wp_cards=is_x(wp->cards);
	int p_full,wp_full;
	int i;
	for(i=1;i<14;i++){
		if(p_cards[i]==3)p_full=i;
		if(wp_cards[i]==3)wp_full=i;
	}
	free(p_cards);
	free(wp_cards);
	if(p_full>wp_full)return p;
	if(wp_full>p_full) {
		int move_p=p->nome-wp->nome;
		return p-=move_p;	
	}
}
player *set_colore(player *p,player *wp){
	int first_card=0;
	if(p->cards[first_card].seme>wp->cards[first_card].seme)return p;	
	if(wp->cards[first_card].seme>p->cards[first_card].seme){
		int move_p=p->nome-wp->nome;
		return p-=move_p;
	}
}
player *set_scala(player *p,player *wp){
	int last_card=NCARDS-1;
	if(p->cards[last_card].punti>wp->cards[last_card].punti)return p;
	if(wp->cards[last_card].punti>p->cards[last_card].punti){
		int move_p=p->nome-wp->nome;
		return p-=move_p;
	}
	return p;	
}
//utilizzo set_full per il tris!
player *set_tris(player *p,player *wp){
	return p;	
}
player *set_doppia_coppia(player *p,player *wp){
	
	int *p_cards=is_x(p->cards);
	int *wp_cards=is_x(wp->cards);
	int p_coppia_low,wp_coppia_low,p_coppia_high,wp_coppia_high;
	int p_st_kicker;
	int wp_st_kicker;
	int i,j,kickers,p_cnt,wp_cnt;
	p_cnt=wp_cnt=0;
	
	for(i=1;i<14;i++){
		
		if(p_cards[i]==2 && p_cnt==0){
			p_coppia_low=i;
			p_cnt++;
		}
		else if(p_cards[i]==2 && p_cnt==1){
			p_coppia_high=i;
			++p_cnt;
		}
		if(wp_cards[i]==2 && wp_cnt==0){
			wp_coppia_low=i;
			wp_cnt++;
		}
		else if(wp_cards[i]==2 && wp_cnt==1){
			wp_coppia_high=i;
			++wp_cnt;
		}
		if(p_cards[i]==1)p_st_kicker=i;
		if(wp_cards[i]==1)wp_st_kicker=i;
	}
	free(p_cards);
	free(wp_cards);
	if(p_coppia_high!=wp_coppia_high){
		if(p_coppia_high>wp_coppia_high)return p;
		if(wp_coppia_high>p_coppia_high) {
			int move_p=p->nome-wp->nome;
			return p-=move_p;	
		}
	}
	else if(p_coppia_low!=wp_coppia_low){
		if(p_coppia_low>wp_coppia_low)return p;
		if(wp_coppia_low>p_coppia_low) {
			int move_p=p->nome-wp->nome;
			return p-=move_p;	
		}
	}
	else{
	if (p_st_kicker!=wp_st_kicker){
		if(p_st_kicker>wp_st_kicker)return p;
		if(wp_st_kicker>p_st_kicker){
			int move_p=p->nome-wp->nome;
			return p-=move_p;
		}
	}
	else return p;
	}
}
player *set_coppia(player *p,player *wp){
	int *p_cards=is_x(p->cards);
	int *wp_cards=is_x(wp->cards);
	int p_coppia,wp_coppia;
	int p_st_kicker,p_nd_kicker,p_rd_kicker;
	int wp_st_kicker,wp_nd_kicker,wp_rd_kicker;
	int *p_kickers,*wp_kickers;
	p_kickers=(int*)calloc(3,sizeof(int));
	wp_kickers=(int*)calloc(3,sizeof(int));
	int i,j,kickers,p_cnt,wp_cnt;
	kickers=3;
	for(i=1;i<14;i++){
		if(p_cards[i]==2)p_coppia=i;
		if(wp_cards[i]==2)wp_coppia=i;
		if(p_cards[i]==1){
			switch(p_cnt){
				case 0:
				p_st_kicker=i;
				p_kickers[0]=p_st_kicker;
				break;
				case 1:
				p_nd_kicker=i;
				p_kickers[1]=p_nd_kicker;
				break;	
				case 2:
				p_rd_kicker=i;
				p_kickers[2]=p_rd_kicker;
				break;
			}
			++p_cnt;
		}
		if(wp_cards[i]==1){
			switch(wp_cnt){
				case 0:
				wp_st_kicker=i;
				wp_kickers[0]=wp_st_kicker;
				break;
				case 1:
				wp_nd_kicker=i;
				wp_kickers[1]=wp_nd_kicker;
				break;	
				case 2:
				wp_rd_kicker=i;
				wp_kickers[2]=wp_rd_kicker;
				break;
			}
			++wp_cnt;
		}
	}
	free(p_cards);
	free(wp_cards);
	if(p_coppia!=wp_coppia){
		if(p_coppia>wp_coppia)return p;
		if(wp_coppia>p_coppia) {
			int move_p=p->nome-wp->nome;
			return p-=move_p;	
		}
	}
	else
	for(j=0;j<kickers;j++)
	if (p_kickers[j]!=wp_kickers[j]){
		if(p_kickers[j]>wp_kickers[j])return p;
		if(wp_kickers[j]>p_kickers[j]){
			int move_p=p->nome-wp->nome;
			return p-=move_p;
		}
	}
		
}
player *set_carta_alta(player *p,player *wp){
	int i;
	for(i=0; i<NCARDS; i++){
		if(p->cards[i].punti>wp->cards[i].punti)
			return p;
		if(wp->cards[i].punti>p->cards[i].punti){
			int move_p=p->nome-wp->nome;
			return p-=move_p;
		}
	}	
}
//FINE CONFRONTO DEI PUNTEGGI UGUALI/////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

//CONFRONTO DEI PUNTEGGI
player *set_score(player *p,player *wp,winning typeof_w){
	player *new_wp;
	switch (typeof_w) {
		case got_scala_reale:new_wp=set_scala_reale(p,wp);printf("confronto scala reale\n");break;
		case got_scala_colore:new_wp=set_scala_colore(p,wp);printf("confronto scala colore\n");break;
		case got_poker:new_wp=set_poker(p,wp);printf("confronto poker\n");break;
		case got_full:new_wp=set_full(p,wp);printf("confronto full\n");break;
		case got_colore:new_wp=set_colore(p,wp);printf("confronto scala colore\n");break;
		case got_scala:new_wp=set_scala(p,wp);printf("confronto scala\n");break;
		case got_tris:new_wp=set_full(p,wp);printf("confronto tris\n");break;
		case got_doppia_coppia:new_wp=set_doppia_coppia(p,wp);printf("confronto doppia coppia\n");break;
		case got_coppia:new_wp=set_coppia(p,wp);printf("confronto coppia\n");break;
		case got_carta_alta:new_wp=set_carta_alta(p,wp);printf("confronto carta alta\n");break;
		default:break;
	}
	return new_wp;
}
//assegno punteggio giocatore
player *check_score(player *p,player *wp,int confronta){

	//SCALA REALE
	if (is_flush(p->cards) && is_straight(p->cards) && is_royal_straight(p->cards))
		{p->rank="Scala Reale";p->points=got_scala_reale;}
	//SCALA A COLORE
	else if (is_flush(p->cards) && is_straight(p->cards) && !is_royal_straight(p->cards))
		{p->rank="Scala colore";p->points=got_scala_colore;}
	//POKER
	else if (is_poker(p->cards)){p->rank="Poker";p->points=got_poker;}
	//FULL
	else if (is_full(p->cards)){p->rank="Full";p->points=got_full;}
	//COLORE
	else if (is_flush(p->cards)){p->rank="Colore";p->points=got_colore;}
	//SCALA
	else if (!is_flush(p->cards) && is_straight(p->cards)){p->rank="Scala";p->points=got_scala;}
	//TRIS
	else if (is_tris(p->cards)){p->rank="Tris";p->points=got_tris;}
	//DOPPIA COPPIA
	else if (is_double_twin(p->cards)){p->rank="Doppia coppia";p->points=got_doppia_coppia;}
	//COPPIA
	else if (is_twin(p->cards)){p->rank="Coppia";p->points=got_coppia;}
	//CARTA ALTA
	else {p->rank="Carta alta";p->points=got_carta_alta;}
	
	if(confronta){
		printf("*********confronta**********\n");
		if(p->points>wp->points)return p;
		else if(p->points==wp->points){
			printf("!!!!!!!!!!punti uguali\n");
			int typeof_w=p->points;
			player *new_wp;
			new_wp=set_score(p,wp,typeof_w);
			return new_wp;
			
		}
		else if(p->points<wp->points){
			int move_p=p->nome-wp->nome;
			return p-=move_p;
		}
	}
	else return p;
}
void print_seme(cqfp seme)
{
	if (seme==cuori) printf("SEME CUORI %d\n",seme);
	else if (seme==quadri) printf("SEME QUADRI %d\n",seme);
	else if (seme==fiori) printf("SEME FIORI %d\n",seme);
	else if (seme==picche) printf("SEME PICCHE %d\n",seme);
}

void order_cards(card *cards)
{
	//variabili di appoggio per il sort
	//crescente in base a numero
	int punti,f;
	cqfp seme;
	card *ordineTmp;
	ordineTmp=(card*)malloc(1*sizeof(card));
	//iteratori
	int i, j;
	printf("\n");
	printf("Ordino le carte\n");
	for (i = 1; i < NCARDS; i++)
	{
		//mi appoggio ad ordineTemp
		//ordineTmp[0].seme=cards[i].seme;
		punti=cards[i].punti;
		seme=cards[i].seme;
		
		for (j = i - 1; (j >= 0) && (cards[j].punti > punti); j--)
		{
			cards[j+1].seme = cards[j].seme;
			cards[j+1].punti = cards[j].punti;
		}
		
		cards[j+1].punti=punti;
		cards[j+1].seme=seme;
	}
	//libero
	free(ordineTmp);
	for (f = 0; f < NCARDS; f++)
	{
		printf("!!!carta %d di ",cards[f].punti);
		print_seme(cards[f].seme);
	}
}

card assign_values(int punti, cqfp seme)
{
    card c;
    c.punti = punti;
    c.seme = seme;
    return c;
}


void play_poker(card mazzo[52])
{

    int flush_cnt = 0, hand_cnt = 0, mazzo_cnt = 0;
    int i,j,k,n;
    //card hand[NPLAYERS][5];
	//*********//
	//giocatori//
	//*********//
    
    srand(time(NULL));
    
	vincite *all_vincite;
	vincite *winner_all_vincite;
	all_vincite=(vincite*)calloc(NPLAYERS,sizeof(vincite));
	for(n=0; n<NDEALS; n++) {
		all_vincite[n].nome=n;
		all_vincite[n].num_vincite=0;
	}
	winner_all_vincite=&all_vincite[0];
	//numero di mani
    for(i=0; i<NDEALS; ++i) {
		player *players;
		player *winPlayer;
		players=(player*)calloc(NPLAYERS,sizeof(player));
        shuffle(mazzo);
		//numero di giocatori
		for(j=0; j<NPLAYERS; j++){
			printf("giocatore %d\n",j);
			//distribuisco le carte ai giocatori			
			for(k=0; k<NCARDS; k++){
				players[j].cards[k]=mazzo[mazzo_cnt];
				//stampo le carte
			//*printf("carta %d \n",players[j].cards[k].punti);
				/*if (players[j].cards[k].seme==cuori) printf("SEME CUORI %d\n",players[j].cards[k].seme);
				if (players[j].cards[k].seme==quadri) printf("SEME QUADRI %d\n",players[j].cards[k].seme);
				if (players[j].cards[k].seme==fiori) printf("SEME FIORI %d\n",players[j].cards[k].seme);
				if (players[j].cards[k].seme==picche) printf("SEME PICCHE %d\n",players[j].cards[k].seme);*/
				//print_seme(players[j].cards[k].seme);
				mazzo_cnt++;
			}
			
			//ordino le carte di ogni giocatore
			order_cards(players[j].cards);
			//alloco la memoria di rank (tipo punteggio giocatore)
			players[j].rank=(char*)calloc(100,sizeof(char));
			players[j].nome=j;
			//assegnp punteggio
			if(j>0)winPlayer=check_score(&players[j],winPlayer,TRUE);
			else winPlayer=check_score(&players[j],winPlayer,FALSE);
			printf("%s\n",players[j].rank);
			//giocatore vincente
			printf("giocatore vincente: giocatore %d con %s\n",winPlayer->nome,winPlayer->rank);
			//punteggio e tipo di punteggio di ogni giocatore
			printf("Punteggio %d\n",winPlayer->points);
			//printf("Tipo di punteggio %s",players[j].rank);
			//next
			printf("\n\n");			
		}
		mazzo_cnt=0;
		printf("Vince il giocatore %d con %s\n",winPlayer->nome,winPlayer->rank);
		all_vincite[winPlayer->nome].nome=winPlayer->nome;
		++all_vincite[winPlayer->nome].num_vincite;
		free(players);
		//tomassetti
        //deal_the_cards(mazzo, hand);
        /*for(j=0; j<NPLAYERS; ++j) {
            ++hand_cnt;
            if (is_flush(hand[j])) {
                ++flush_cnt;
                printf("%s%d\n%s%d\n%s%f\n\n",
                "      Hand number: ", hand_cnt,
                "     Flush number: ", flush_cnt,
                "Flush probability: ", (double) flush_cnt/hand_cnt);
            }
        }*/
    }
	//calcolo il giocatore migliore di tutte le deals
	for(n=0; n<NPLAYERS; n++) {
		if(n!=0)
		if (all_vincite[n].num_vincite>winner_all_vincite->num_vincite)
			winner_all_vincite=&all_vincite[n];
	}
	//stampo 
	printf("Vince l'intera partita il giocatore %d \n",winner_all_vincite->nome);
}

void shuffle(card mazzo[52])
{
    int i,j;
    for (i=0; i<52; ++i) {
        j = rand()%52;
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

void deal_the_cards(card mazzo[52], card hand[NPLAYERS][5])
{
    int card_cnt = 0, i, j;
    
    for (j=0;j<5;++j)
        for (i=0; i<NPLAYERS;++i)
            hand[i][j] = mazzo[card_cnt++];
}

//colore tomassetti
/*int is_flush(card h[5])
{
    int i;
    for (i=1;i<5; ++i)
        if (h[i].seme != h[0].seme)
        return 0;
    return 1;
}*/
int is_flush(card *cards)
{
    int i;
    for (i=1;i<NCARDS; ++i)
        if (cards[i].seme != cards[0].seme)
        return FALSE;
    return TRUE;
}
