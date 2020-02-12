/*
    Simple récepteur TCP en IPv4
    Les arguments des fonctions sont à compléter.
    Copyright (C) 2018  Pierre Neumann

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/ip.h>

int main(int argc, char **argv)
{
	int sockfd,sockfd2;
	char buf[1024];
	socklen_t addrlen;

	struct sockaddr_in receiver_addr;
	struct sockaddr_in sender_addr;

	// Verification du nombre d'arguments de la ligne de commande
	if(argc != 2) {
		printf("Usage: %s port\n", argv[0]);
		exit(-1);
	}

	/** 
	 *  Creation du socket
	 *  Voir : man socket
	 */
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("creation du socket");
		exit(EXIT_FAILURE);
	}

	/**
	 * Definition du format d'adresse du socket
	 * Voir : man 7 ip 
	 */

	// Socket IPv4
	receiver_addr.sin_family = AF_INET;
	// On écoute sur le port en argument
	// Attention celui-ci doit être au bon format
	// Voir: man htons
	receiver_addr.sin_port = htons(atoi(argv[1]));
	// Le socket est lié à toutes les interfaces locales  
	receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Taille de la structure de format d'adresse
	addrlen = sizeof(struct sockaddr_in);

	// On initialise le buffer de reception
	memset(buf,'\0',1024);

	// On assigne le format d'adresse receiver_addr au socket
	if(bind(sockfd,(struct sockaddr *)&receiver_addr,addrlen) == -1) {
		perror("Bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// Voir : man listen 
	if(listen(sockfd,5) == -1)
	{
		perror("listen");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	//On accepte une nouvelle connexion
	// Voir : man 2 accept
	if((sockfd2 = accept(sockfd,(struct sockaddr *)&sender_addr,&addrlen)) == -1)
	{
		perror("accept");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/**
	 * Reception sur le socket
	 * Le contenu sera dans buf
	 * Voir : man recv
	 */
	if(recv(sockfd,buf,1023,0) == -1)
	{
		perror("recv");
		close(sockfd);
		close(sockfd2);
		exit(EXIT_FAILURE);
	}

	// Affichage de la chaine
	printf("%s", buf);

	// Fermeture du socket
	close(sockfd);

	return 0;
}
