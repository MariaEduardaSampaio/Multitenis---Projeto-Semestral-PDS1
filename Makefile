ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: multitenis.exe bola.exe circles.exe teclado.exe bouncer.exe misseis.exe pong.exe tela.exe allegro_base.exe	

multitenis.exe: multitenis.o
	gcc -o multitenis.exe multitenis.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

multitenis.o: multitenis.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c multitenis.c		

bola.exe: bola.o
	gcc -o bola.exe bola.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

bola.o: bola.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c bola.c		

circles.exe: circles.o
	gcc -o circles.exe circles.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

circles.o: circles.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c circles.c			
	
bouncer.exe: bouncer.o
	gcc -o bouncer.exe bouncer.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

bouncer.o: bouncer.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c bouncer.c	
	
teclado.exe: teclado.o
	gcc -o teclado.exe teclado.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

teclado.o: teclado.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c teclado.c		
	
pong.exe: pong.o
	gcc -o pong.exe pong.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

pong.o: pong.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c pong.c	
	
tela.exe: tela.o
	gcc -o tela.exe tela.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

tela.o: tela.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c tela.c	
	
allegro_base.exe: allegro_base.o
	gcc -o allegro_base.exe allegro_base.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

allegro_base.o: allegro_base.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c allegro_base.c	

clean:
	del multitenis.o
	del multitenis.exe
	del bola.o 
	del bola.exe
	del circles.o 
	del circles.exe
	del teclado.o 
	del teclado.exe
	del bouncer.o 
	del bouncer.exe	
	del pong.o
	del pong.exe
	del tela.o
	del tela.exe
	del allegro_base.o
	del allegro_base.exe