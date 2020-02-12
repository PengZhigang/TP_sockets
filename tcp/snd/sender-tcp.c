/*
    Simple émetteur TCP en IPv4
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
	int sockfd;
	socklen_t addrlen;
	struct sockaddr_in receiver_addr;
	
	// Verification du nombre d'arguments de la ligne de commande
	if(argc != 4) {
		printf("USAGE: %s @dest port string\n", argv[0]);
		exit(-1);
	}
	
	/** 
	 *  Creation du socket
	 *  Voir : man socket
	 */
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/**
	 * Definition du format d'adresse du socket
	 * Voir : man 7 ip 
	 */

	// Socket IPv4
	receiver_addr.sin_family = AF_INET;
	// Port d'écoute du recepteur
	// Attention celui-ci doit être au bon format
	// Voir: man htons
	receiver_addr.sin_port   = htons(atoi(argv[2]));

	// On y ajoute l'adresse IP du recepteur
	// Attention celle-ci doit être au bon format
	// Voir: man inet_pton
	if(inet_pton(AF_INET,argv[1],&receiver_addr.sin_addr) != 1) {
		perror("inet_pton");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// Taille de la structure de format d'adresse
	addrlen = sizeof(struct sockaddr_in);

	// Connexion au recepteur
	if(connect(sockfd,(struct sockaddr *)&receiver_addr,addrlen) == -1) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

	/**
	 * Envoi du message en paramètre
	 * Voir : man send
	 */
	if(send(sockfd, argv[3], strlen(argv[3]), 0)) {
		perror("send");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// Fermeture du socket
	close(sockfd);

	return 0;
}
