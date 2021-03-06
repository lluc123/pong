#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include "balle.h"
#include <pthread.h>
#include <cmath>

typedef int socklen_t;

#include <stdio.h>
#include <stdlib.h>
#define PORT 23

using namespace std;

struct argsThread
{
	SOCKET jsock;
	joueur *player;
};

void jeuxPrincipal(SOCKET jsock[2]);
void* receivJoueur1(void *args);

int main(void)
{
    #if defined (WIN32)
        WSADATA WSAData;	//start WSADATA
        int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
    #else
        int erreur = 0;
    #endif
 

	/************************************
	*			Start le serveur		*
	*************************************/

    SOCKADDR_IN sin;	//address du serveur
    SOCKET sock;		//socket du serveur
    int recsize = sizeof sin;
	SOCKET jsock[2];	//joueur socket
	int personne = -1;	//compteur de joueur
    int sock_err;		//code d'erreur socket
	char bonjour[15] = "Allo 1";	//message envoyer au joueur
    if(!erreur)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
 
        if(sock != INVALID_SOCKET)
        {
            printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);
 
            sin.sin_addr.s_addr = htonl(INADDR_ANY);
            sin.sin_family = AF_INET;
            sin.sin_port = htons(PORT);
            sock_err = bind(sock, (SOCKADDR*) &sin, recsize);
 
            if(sock_err != SOCKET_ERROR)
            {
                sock_err = listen(sock, 5);
                printf("Listage du port %d...\n", PORT);
 
                if(sock_err != SOCKET_ERROR)
                {
                    /* Cr�ation de l'ensemble de lecture */
                    fd_set readfs;
 
                    while(personne < 1)	//tant qu'on a pas 2 joueur
                    {
                        /* On vide l'ensemble de lecture et on lui ajoute 
                        la socket serveur */
                        FD_ZERO(&readfs);
                        FD_SET(sock, &readfs);
 
                        /* Si une erreur est survenue au niveau du select */
                        if(select(sock + 1, &readfs, NULL, NULL, NULL) < 0)
                        {
                            perror("select()");
                            exit(errno);
                        }
 
                        /* On regarde si la socket serveur contient des 
                        informations � lire */
                        if(FD_ISSET(sock, &readfs))
                        {
                            /* Ici comme c'est la socket du serveur cela signifie 
                            forcement qu'un client veut se connecter au serveur. 
                            Dans le cas d'une socket cliente c'est juste des 
                            donn�es qui serons re�ues ici*/
                            SOCKADDR_IN csin;
                            int crecsize = sizeof csin;
							personne++;
                            /* Juste pour l'exemple nous acceptons le client puis 
                            nous refermons imm�diatement apr�s la connexion */
                          //  SOCKET csock = accept(sock, (SOCKADDR *) &csin, &crecsize);
							jsock[personne] = accept(sock, (SOCKADDR *) &csin, &crecsize);
 
                            cout << "Un client s'est connecte " << inet_ntoa(csin.sin_addr) << endl;
                        }
                    }
                }
            }
        }
    }
	send(jsock[0], bonjour, sizeof(bonjour), 0);
	bonjour[5] = '2';
	send(jsock[1], bonjour, sizeof(bonjour), 0);

	/************************************
	*			Jeux Principal			*
	*************************************/

	jeuxPrincipal(jsock);


	//Ferme le jeu
	closesocket(jsock[1]);
	closesocket(jsock[0]);
    #if defined (WIN32)
        WSACleanup();
    #endif
 
    return EXIT_SUCCESS;
}

void jeuxPrincipal(SOCKET jsock[2])
{
	cout <<"cos " <<cos((float )180)<< endl;
	cout  <<"sin "<< sin((float )180)<< endl;
	//CALCUL DE Pong
	sendInfo coord;	//coord de tout les balle et palette
	balle pong;		//Balle pong
	joueur player1(1);	//joueur 1
	joueur player2(2);	//joueur 2
	char receive[5];	
	argsThread threadArgs1;			//argument pour le thread 1
	argsThread threadArgs2;			//argument pour le thread 2
	pthread_t thread1;				//Thread 1
	pthread_t thread2;				//Thread 2

	threadArgs1.jsock = jsock[0];	//Set les argument des thread
	threadArgs1.player = &player1;
	threadArgs2.jsock = jsock[1];
	threadArgs2.player = &player2;

	pthread_create(&thread1,NULL, receivJoueur1,(void *)&threadArgs1);	//On creer les thread
	pthread_create(&thread2,NULL, receivJoueur1,(void *)&threadArgs2);
	//send(jsock[0],(char *) &coord, sizeof(coord), 0);
	
	while(1)
	{
		Sleep(50);		//sleep
		pong.move();	//bouge la balle
		coord.joueur[0] = player1.getrect();	//Set les donner a envoyer
		coord.joueur[1] = player2.getrect();
		coord.balle = pong.getrect();
		/*coord.score[0] = player1.getScore();
		coord.score[1] = player2.getScore();*/

		send(jsock[1],(char *)&coord, sizeof(coord), 0);	//Envoye les donner au 2 joueur
		send(jsock[0],(char *)&coord, sizeof(coord), 0);

		if(pong.getVx() > 0)	//si il va a droite
		{
			if(pong.colision(player2.getrect()))	//colision avec pallette
			{
				pong.changeAngle(player2.getrect());
			}
		}
		else					//si il va a gauche
		{
			if(pong.colision(player1.getrect()))	//colision avec pallette
			{
				pong.changeAngle(player1.getrect());
			}
		}
		if(pong.getVy() > 0)
		{
			if(pong.getY()+pong.getH()>600)
			{
				pong.murRebond();
				pong.setY(600-pong.getH());
			}
		}
		else
		{
			if(pong.getY()<0)
			{
				pong.murRebond();
				pong.setY(0);
			}
		}
	}
}

void* receivJoueur1(void *args)
{
	argsThread *argument =(argsThread *) args;
	int receive;
	char direction = 'a';
	while(1)
	{
		receive = recv(argument->jsock,&direction, sizeof(direction), 0);
		if(receive > 0)
		{
			if(direction == 'h')
				argument->player->bougerHaut();
			else
				argument->player->bougerBas();
		}
	}
}