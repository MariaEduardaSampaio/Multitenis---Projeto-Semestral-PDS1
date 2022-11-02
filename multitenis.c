#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <time.h>
#include <Windows.h>

const float FPS = 100;
const int SCREEN_W = 960;
const int SCREEN_H = 540;

#define PLAYER_H 20
#define PLAYER_W 70
#define PLAYER_VELOC 5
#define LIMITE_BOLAS 1000
#define DESLOC_BALL 2
#define MAX_BALLS 12

// ****************** INICIO BLOCO DOS JOGADORES **********************

typedef struct player {
	float x;
	float y;
	float veloc;
	int right, left, up, down, attack, points;
	int id;
	ALLEGRO_COLOR color;
} player;

void initplayer1(player *player) // JOGADOR DE CIMA
{
	player->x = SCREEN_W/2;
	player->y = PLAYER_H;
	player->veloc = PLAYER_VELOC;
	player->color = al_map_rgb(rand()%256, 0, rand()%256);
	player->right = 0;
	player->left = 0;
	player->up = 0;
	player->down = 0;
	player->attack = 0;
	player->points = 0;
	player->id = 1;
}

void initplayer2(player *player) // JOGADOR DE BAIXO
{ 
	player->x = SCREEN_W/2;
	player->y = SCREEN_H - PLAYER_H/2;
	player->veloc = PLAYER_VELOC;
	player->color = al_map_rgb(rand()%256, 0, rand()%256);
	player->right = 0;
	player->left = 0;
	player->up = 0;
	player->down = 0;
	player->attack = 0;
	player->points = 0;
	player->id = 2;
}

void draw_players(player player1, player player2)
{
	al_draw_filled_rectangle(player1.x-PLAYER_W/2, player1.y, player1.x+PLAYER_W/2, player1.y-PLAYER_H, player1.color);
	al_draw_filled_rectangle(player2.x-PLAYER_W/2, player2.y, player2.x+PLAYER_W/2, player2.y-PLAYER_H, player2.color);
}

void update_player1(player *player)
{
	if (player->right && ((player->x + player->veloc + PLAYER_W/2) < SCREEN_W))
	{
		player->x += player->veloc;
	}

	if (player->left && ((player->x - player->veloc - PLAYER_W/2) >= 0))
	{
		player->x -= player->veloc;
	}

	if (player->up && ((player->y + player->veloc - PLAYER_H - PLAYER_H/2) >= 0))
			player->y -= player->veloc;

	if (player->down && (player->y - player->veloc < SCREEN_H))
		if (player->y + player->veloc <= SCREEN_H/2)
			player->y += player->veloc;
}

void update_player2(player *player)
{
	if (player->right && ((player->x + player->veloc + PLAYER_W/2) < SCREEN_W))
	{
		player->x += player->veloc;
	}

	if (player->left && ((player->x - player->veloc - PLAYER_W/2) >= 0))
	{
		player->x -= player->veloc;
	}

	if (player->up && ((player->y + player->veloc - PLAYER_H - PLAYER_H/2) >= SCREEN_H/2))
		if (player->y + player->veloc >= SCREEN_H/2)
			player->y -= player->veloc;

	if (player->down && ((player->y - player->veloc) < SCREEN_H))
		player->y += player->veloc;
}

void match (player *player1, player *player2, int winner)
{
	FILE *arqw = fopen("historico_partidas.txt", "a");

	fprintf(arqw, "Pontos da partida\n");
	if (winner == 1)
		fprintf(arqw, "Vencedor: P1\n");
	else if (winner == 2)
		fprintf(arqw, "Vencedor: P2\n");
	else
		fprintf(arqw, "Empate!\n");
	fprintf(arqw, "P1: %d   x   P2: %d\n\n", player1->points, player2->points);

	fclose(arqw);
}
// ****************** FIM BLOCO DOS JOGADORES **********************
// ****************** INICIO BLOCO DAS BOLAS **********************


typedef struct ball {
	float x;
	float y;
	float ray;
	float dx;
	float dy;
	float status;
	ALLEGRO_COLOR color;
} ball;

void init_ball(ball *b, int position_ball) 
{
	switch (position_ball)
	{
		case 0: // SUPERIOR DIREITO
			b->y = 0;
			b->color = al_map_rgb(255,255,255);
			b->ray = 5+rand()%6;
			b->x = SCREEN_W - b->ray;
			b->dx = -DESLOC_BALL;
			b->dy = DESLOC_BALL;
			b->status = 1;
		break;

		case 1: // INFERIOR DIREITO
			b->y = SCREEN_H - 4;
			b->color = al_map_rgb(255,255,255);
			b->ray = 5+rand()%6;
			b->x = SCREEN_W - b->ray;
			b->dx = -DESLOC_BALL;
			b->dy = -DESLOC_BALL;
			b->status = 1;
		break;

		case 2: // INFERIOR ESQUERDO
			b->y = SCREEN_H - 4;
			b->color = al_map_rgb(255,255,255);
			b->ray = 5+rand()%6;
			b->x = b->ray;
			b->dx = DESLOC_BALL;
			b->dy = -DESLOC_BALL;
			b->status = 1;
		break;

		case 3: // SUPERIOR ESQUERDO
			b->color = al_map_rgb(255,255,255);
			b->ray = 5+rand()%6;
			b->x = b->ray;
			b->dx = DESLOC_BALL;
			b->dy = DESLOC_BALL;
			b->status = 1;
		break;
	}
}

void collision(ball *ball, player *player, int id)
{ 
	if (player->attack == 1)
	{
		if (id == 1)
		{
			if (((ball->y + ball->ray) <= (player->y + PLAYER_H)) && ((ball->y + ball->ray) >= (player->y - PLAYER_H)))
				if ((ball->x + ball->ray) <= (player->x + PLAYER_W) && (ball->x + ball->ray) >= (player->x - PLAYER_W))
					ball->dy = -ball->dy;
		}
		else if (id == 2) 
		{
			if (((ball->y + ball->ray) <= (player->y + PLAYER_H)) && ((ball->y + ball->ray) >= (player->y - PLAYER_H)))
				if ((ball->x + ball->ray) <= (player->x + PLAYER_W) && (ball->x + ball->ray) >= (player->x - PLAYER_W))
					ball->dy = -ball->dy;
		}
	}
}

//fiz com que a função update_ball sirva apenas para atualizar a bola a todo momento, antigamente ela apenas atualizava se certas condições fossem alcançadas
void update_ball(ball *ball)
{
	ball->x += ball->dx;
	ball->y += ball->dy;

	if (ball->x + ball->ray > SCREEN_W || ball->x - ball->ray < 0){
		ball->dx *= -1;
	}
}

//criei uma nova função apenas para checar os pontos, ela compara o y da bola e depois que ela cruza e contabiliza um ponto, é ativado o ball->status para previnir que ela contabilize mais de um ponto
void check_points(ball *ball, player *player1, player *player2){

	ball->status = 0;
	if (ball->y + ball->ray <= -15 && ball->y + ball->ray >= -16 && ball->status == 0 ) // ponto jogador 2
	{ 
		ball->status = 1;
		player2->points += 1;
	}
	else if (ball->y + ball->ray >= SCREEN_H + 15 && ball->y + ball->ray <= SCREEN_H + 16 && ball->status == 0) // ponto jogador 1
	{ 
		ball->status = 1;
		player1->points += 1;
	}
}

void draw_ball(ball *ball)
{
	al_draw_filled_circle(ball->x, ball->y, ball->ray, ball->color);
}

// ****************** FIM BLOCO DAS BOLAS **********************

void draw_scenario()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H/2, al_map_rgb(0, 128, 0));
	al_draw_line(0, SCREEN_H/2, SCREEN_W, SCREEN_H/2, al_map_rgb(255, 255, 255), 0);
	al_draw_filled_rectangle(0, SCREEN_H/2, SCREEN_W, SCREEN_H, al_map_rgb(50, 205, 50));
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	player player1, player2;
	ball ball[LIMITE_BOLAS];
	char my_text[20];
	char buf[100];
	int i = 0, segundos = 0, playing = 1, winner = 0, count = 0;
	int max_bolas = 12;
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init())
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon())
    {
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

    //cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) 
	{
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) 
	{
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) 
    {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	} 

	//instala o mouse
	if(!al_install_mouse()) 
	{
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon())
	{
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}

	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL)
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	
 	//----------------------- fim - rotinas de inicializacao -----------------------------------

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) 
	{
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());  	

	al_start_timer(timer);

	initplayer1(&player1);
	initplayer2(&player2);

	while(playing)
	{
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) 
		{
			draw_scenario();
			update_player1(&player1);
			update_player2(&player2);
			draw_players(player1, player2);

			for (count = 0; count < max_bolas; count++)
			{
				update_ball(&ball[count]);
				draw_ball(&ball[count]);
				check_points(&ball[count], &player1, &player2);
			}
			if (player1.points >= 10 || player2.points >= 10)
				playing = 0;
			
			if (al_get_timer_count(timer)%(int)FPS == 0)
			{
				if (segundos == 4)
				{
					segundos = 0;
					if (i <= max_bolas)
					{
						init_ball(&ball[i], i%4);			
						i++;
					}
				}
				else 
					segundos++;
			}

			sprintf(my_text, "P1: %d x P2: %d", player1.points, player2.points);	
			al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W - 210, SCREEN_H/2 + 5, 0, my_text);
			
			al_flip_display();

			if(al_get_timer_count(timer)%(int)FPS == 0)
				printf("\n%d segundos se passaram...\n", (int)(al_get_timer_count(timer)/FPS));		
		}

		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			playing = 0;
	
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			printf("\nmouse clicado em: %d, %d", ev.mouse.x, ev.mouse.y);
		
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
			switch (ev.keyboard.keycode)
			{
				// PLAYER 1
				case ALLEGRO_KEY_A:
					player1.left = 1;
				break;

				case ALLEGRO_KEY_D:
					player1.right = 1;
				break;

				case ALLEGRO_KEY_W:
					player1.up = 1;
				break;

				case ALLEGRO_KEY_S:
					player1.down = 1;
				break;

				case ALLEGRO_KEY_J:
				{
					player1.attack = 1;
					for (int count = 0; count < max_bolas; count++)
						collision(&ball[count], &player1, 1);
				}
				break;

				// PLAYER 2
				case ALLEGRO_KEY_LEFT:
					player2.left = 1;
				break;

				case ALLEGRO_KEY_RIGHT:
					player2.right = 1;
				break;

				case ALLEGRO_KEY_UP:
					player2.up = 1;
				break;

				case ALLEGRO_KEY_DOWN:
					player2.down = 1;
				break;

				case ALLEGRO_KEY_SPACE:
				{
					player2.attack = 1;
					for (int count = 0; count < max_bolas; count++)
						collision(&ball[count], &player2, 2);
				}
				break;
			} // FIM SWITCH
		} // FIM ELSE 
		
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
				// PLAYER 1
				case ALLEGRO_KEY_A:
					player1.left = 0;
				break;

				case ALLEGRO_KEY_D:
					player1.right = 0;
				break;

				case ALLEGRO_KEY_W:
					player1.up = 0;
				break;

				case ALLEGRO_KEY_S:
					player1.down = 0;
				break;

				// PLAYER 2
				case ALLEGRO_KEY_LEFT:
					player2.left = 0;
				break;

				case ALLEGRO_KEY_RIGHT:
					player2.right = 0;
				break;

				case ALLEGRO_KEY_UP:
					player2.up = 0;
				break;

				case ALLEGRO_KEY_DOWN:
					player2.down = 0;
				break;
			} // FIM SWITCH
		} // FIM ELSE
	} // FIM WHILE
	

	if (player1.points > player2.points)
	{ 
		winner = player1.id;
		match(&player1, &player2, winner);
		al_clear_to_color(al_map_rgb(0,0,0));
		sprintf(my_text, "P%d ganhou!", winner);	
		al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/3, SCREEN_H/2, 0, my_text);
		sprintf(my_text, "Placar:   P1: %d x P2: %d", player1.points, player2.points);	
		al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/3, SCREEN_H/2+100, 0, my_text);
	}
	else if (player2.points > player1.points)
	{
		winner = player2.id;
		match(&player1, &player2, winner);
		al_clear_to_color(al_map_rgb(0,0,0));
		sprintf(my_text, "P%d ganhou!", winner);	
		al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/3, SCREEN_H/2, 0, my_text);
		sprintf(my_text, "Placar:   P1: %d x P2: %d", player1.points, player2.points);	
		al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/3, SCREEN_H/2+100, 0, my_text);	
	}
	else
	{
		match(&player1, &player2, winner);
		al_clear_to_color(al_map_rgb(0,0,0));
		sprintf(my_text, "Empate!");	
		al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/3, SCREEN_H/2, 0, my_text);
		sprintf(my_text, "Placar:   P1: %d x P2: %d", player1.points, player2.points);	
		al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/3, SCREEN_H/2+100, 0, my_text);		
	}
 /* */
	al_flip_display();	
    al_rest(3);	

	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
	return 0;
}