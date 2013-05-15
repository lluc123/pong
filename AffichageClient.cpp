/*En tête du programme
--------------------------------
Programme : Labo13_SDL_Etape9.cpp
Auteur : Samuel Asselin
Conception : 25/03/2013
Description du programme : Utilisation de la sdl pour faire des déplacements d'image, concepts de base, animation d'un personnage
*/

//Directives au pré-processeur
#include "SDL.h"
#include "balle.h"
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>

using namespace std;

//Déclaration des fonctions
void initialisationParametres(SDL_Surface *image, SDL_Surface *ecran);
void gestionEvenements(SDL_Surface *image, SDL_Surface *ecran, int &mySocket);
int connexionServeur(int &mySocket, WSAData &wsaData);
void receive(int mySocket, SDL_Surface *ecran);
SDL_Rect convertit(Rect rect);
//void creerRectangle(SDL_Surface *ecran, SDL_Rect *rec1 , SDL_Rect *rec2);

int main (int argc, char *argv[])
{
	int mySocket;
	// the winsock data structure
	WSAData wsaData;
	connexionServeur(mySocket, wsaData);

	if(SDL_Init(SDL_INIT_VIDEO) == 1)	 //Démarrage de la SDL (chargement du système vide)
	{
		cout << "Erreur lors de l'initialisation de la SDL" , SDL_GetError();
		exit(EXIT_FAILURE);
	}

	//La SDL estchargée. Vous pouvez mettre ici le contenu de votre programme
	SDL_Surface *ecran = NULL;		//Variable pointeur pour l'écran
	SDL_Surface *image = NULL;		//Variable pointeur pour l'image
	SDL_Rect *rec1 = NULL;
	SDL_Rect *rec2 = NULL;

	//Chargement de l'icône AVANT SDL_SetVideoMode, copier le bmp dans le même dossier que .cpp
	SDL_WM_SetIcon(SDL_LoadBMP("sdl_icone.bmp"), NULL);		//Pour charger l'endroit de l'image bmp

	ecran = SDL_SetVideoMode (800 , 600, 32, SDL_RESIZABLE| SDL_HWSURFACE | SDL_DOUBLEBUF);		//Défini l'affichage pour l'écran

	initialisationParametres(image, ecran);

	gestionEvenements(image, ecran, mySocket);

	SDL_FreeSurface(image);		// Libère l'espace mémoire de l'image

	SDL_Quit();	//Arrêt de la SDL (libération de la mémoire)
	// shut down winsock
	WSACleanup();
	return EXIT_SUCCESS;
}

//Initialise tous les paramètres pour créer le programme en sdl
void initialisationParametres(SDL_Surface *image, SDL_Surface *ecran)
{
	SDL_WM_SetCaption ("Pong", NULL);						//Change le titre de la fenêtre
	SDL_FillRect(ecran , NULL, SDL_MapRGB(ecran-> format, 0, 0 , 0));	//Change la couleur de du fond de l'écran
					//Bitsurface Rempli la surface destinée au personnage
	SDL_Flip(ecran);
}

//Fais la gestion des différents évènements relié au mouvement de souris ou bien encore en appuyant sur les flèches
void gestionEvenements(SDL_Surface *image, SDL_Surface *ecran, int &mySocket)
{
	bool continuer = true;
	SDL_Event event;
	int posPlaque;

	SDL_EnableKeyRepeat(10,10);		//Pour répéter l'action d'une touche appuyée

	//code du début qui charge la SDL, vérifie le code d'erreur et charge la fenêtre
	while(continuer == true)
	{
		
		//creerRectangle(ecran, rec1 , rec2);
		//SDL_BlitSurface(image , NULL , ecran);		
		SDL_Flip(ecran);
		SDL_FillRect(ecran , NULL, SDL_MapRGB(ecran-> format, 0, 0 , 0));
		receive(mySocket, ecran);
		

		SDL_PollEvent (&event);
		switch(event.type)		//selon le type d'évènement
		{
			case SDL_QUIT:			//si c'est le x de la fenêtre, on arrête le programme
				continuer = false;	//on ferme 
				break;
			case SDL_KEYDOWN:		//Si appui d'une touche 
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE :	//Appuie sur ECHAP, arrête le programme
					continuer = 0;
					break;
				case SDLK_UP:
					/*posPlaque -= 5;
					send (mySocket, posPlaque, 1, 0);*/
					break;
				case SDLK_DOWN:
					/*posPlaque += 5;
					send posPlaque;*/
					break;
			}
		}
	}
}

int connexionServeur(int &mySocket, WSAData &wsaData)
{
	// Included needed library
#pragma comment(lib, "ws2_32.lib")

	cout << "Welcome to redKlyde's Networking Tutorials!\n";
	cout <<"Tutorial # 2 : Making the Connection - ClientSide\n\n";

	// an error code we will use to get more information about our errors
	int error;
	char adressIp[18];

	cout << "Donnez l'address ip du serveur" << endl;
	cin >> adressIp;

	// We will still use winsock for our network communication, so startup winsock like normal.

	// startup winsock
	error = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// check for errors
	if (error == SOCKET_ERROR) 
	{
		printf("Could Not Start Up Winsock!\n");
		return 1;
	}

	printf("WinSocket started\n");

	// We will still use a socket to communicate with, so create a socket just like we did before.

	// create my socket
	mySocket = socket(AF_INET, SOCK_STREAM, 0);

	// make sure nothing bad happened
	if (mySocket == SOCKET_ERROR) 
	{
		printf("Error Opening Socket!\n");
	}

	printf("Socket Opened!\n");

	// Up to this point everything is pretty much the same, but at this point in part 1, we did a bind()
	// and setup the server socket for listening.  But this is the client application.  We want to CONNECT
	// to the server, not ACCEPT incoming connections.  Though much like the server, we will fill in a
	// sockaddr_in structure with the appropriate information about the application we want to connect to,
	// but then make a call to connect() instead.

	// We need some special information though.  Specifically, we need the location of the server.  What we
	// need is the IP of the server computer.  But typically we don't always know the IP of a computer, especially
	// once we distribute an application.  That IP could also change if we switched Internet providers or added
	// more server computers.  So though we COULD use the IP (i.e. 192.169.1.100), we typically use the web address
	// instead (www.gametutorials.com).

	// Now that we have the address of the server, we have to set up a sockaddr_in structure that will describe
	// the type of connection we are trying to make.

	// our sockaddr structure
	struct sockaddr_in server;

	// fill in the server info
	server.sin_family = AF_INET;
	server.sin_port = htons((unsigned short) 23);
	server.sin_addr.s_addr = inet_addr(adressIp);

	// This is much the same as the server.  We are making an Internet connection, so we use AF_INET.  We specify
	// the port in network byte ordering.  But instead of using INADDR_ANY, we specify an address.  This is the
	// address we retrieved from the hostent structure.

	// We now have all the information we need to connect to the server.  We make the connection with a call to
	// connect().  Connect takes 3 arguments.  The first is the socket we will communicate with, the second is
	// the sockaddr structure that describes our connection.  The third is simply the sizeof the sockaddr structure.

	// connect to the server
	error = connect(mySocket, (sockaddr*)&server, sizeof(server));
	// check for errors
	if (error == SOCKET_ERROR) 
	{
		printf("Error connecting to server!\n");
	}

	// Assuming our server is running, we should have made a connection to the server at this point.  The server will
	// receive the connection, and continue with the program after the accept() call.  Since that is all the server was
	// designed to do, it will cleanup itself and shutdown.  And since this is all we have intended with this part of the
	// client tutorial, we will do the same.

	char message[15];
	recv(mySocket, message, sizeof(message), 0);
	// close our socket
	//closesocket(mySocket);

	printf("Socket Closed\n");

	

	printf("WinSocket Shutdown\n");

	printf("Press any key to continue ...\n");
	fflush(stdin);
	cout << message;
	fflush(stdin);
	getchar();
	return mySocket;
}
/*
void creerRectangle(SDL_Surface *ecran, SDL_Rect rec1 , SDL_Rect rec2)
{
	SDL_FillRect(ecran, &rec1, SDL_MapRGB(ecran->format, 223, 223, 223));
	SDL_FillRect(ecran, &rec2, SDL_MapRGB(ecran->format, 223, 223, 223));
}*/

void receive(int mySocket, SDL_Surface *ecran)
{
	//recois
	sendInfo recois;
	char allo[5] = "Pass";
	send(mySocket,allo,sizeof(allo), 0);
	//recevoir les infos de la balle et des plaquettes
	int totalrecu = recv(mySocket, (char *) &recois, sizeof(recois), 0);
	if(totalrecu > 0)
	{
		SDL_FillRect(ecran, &convertit(recois.joueur[1]), SDL_MapRGB(ecran->format, 0, 223, 0));
		SDL_FillRect(ecran, &convertit(recois.joueur[0]), SDL_MapRGB(ecran->format, 0, 0, 223));
		SDL_FillRect(ecran, &convertit(recois.balle), SDL_MapRGB(ecran->format, 223, 0, 0));
	}
}

SDL_Rect convertit(Rect rect)
{
	SDL_Rect good_rect;
	good_rect.x = rect.x;
	good_rect.y = rect.y;
	good_rect.w = rect.w;
	good_rect.h = rect.h;
	return good_rect;
}