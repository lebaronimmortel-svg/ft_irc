# <span style="color: cyan;"> ft_irc </span> #

	Le projet ft_irc est dans le but de coder un serveur IRC, c'est à dire un gestionnaire de chat en ligne entre client et serveur. 

	L'IP dit OÙ envoyer les données (la machine).
	Le Port dit À QUI les donner sur cette machine (l'application).
	Le Socket est le CANAL OUVERT (IP + Port) qui permet la connexion effective entre deux programmes.
	
	Le serveur suit ce cycle de vie :
		socket() $\rightarrow$ bind() $\rightarrow$ listen() $\rightarrow$ accept() $\rightarrow$ read() / write().

	Le client suit ce cycle de vie :
		socket() $\rightarrow$ connect() $\rightarrow$ write() / read().

## <span style="color: cyan"> Socket </span> ##

	#include <sys/socket>

	socket(int domain, int type, int protocol);

### Résumé ###

	Elle demande au système d'exploitation de réserver les ressources réseau et retourne un descripteur de fichier (un entier int) qui identifie l'embout de connexion créé.

### Valeur de retour ###

	Un File Descriptor, ou -1 en cas d'échec.

## <span style="color: cyan"> Set socket option </span> ##

	#include <sys/socket>

	int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

### Résumé ###

	Elle permet de modifier la configuration et le comportement par défaut d'un socket déjà créé par la fonction socket()

### Valeur de retour ###

	0, ou -1 en cas d'échec.

## <span style="color: cyan;"> Get socket name </span> ##

	#include <sys/socket>

	int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

### Résumé ###
	
	Elle permet de récupérer l'adresse local (IP et Port) actuellement attribuée à un socket.

### Valeur de retour ###

	0, ou -1 en cas d'échec.

## <span style="color: cyan;"> Get protocol by name </span> ##

	#include <netdb.h>

	struct protoent *getprotobyname(const char *name);

### Résumé ###
	
	Elle sert à rechercher les informations d'un protocole réseau à partir de son nom sous forme de chaîne de caractères et renvoie un pointeur vers une structure protoent contenant le numéro officiel du protocole.

### Valeur de retour ###

	struct protoent 
	{
    	char  *p_name;       // Nom officiel du protocole (ex: "tcp")
    	char **p_aliases;    // Liste des alias (tableau de chaînes terminé par NULL)
    	int    p_proto;      // Le numéro officiel du protocole (ex: IPPROTO_TCP)
	}

## <span style="color: cyan;"> Get host by name </span> ##

	#include <netdb.h>

	struct protoent *getprotobyname(const char *name);

### Résumé ###
	
	Elle sert à rechercher les informations d'un protocole réseau à partir de son nom sous forme de chaîne de caractères et renvoie un pointeur vers une structure protoent contenant le numéro officiel du protocole.

### Valeur de retour ###

	struct protoent 
	{
    	char  *p_name;       // Nom officiel du protocole (ex: "tcp")
    	char **p_aliases;    // Liste des alias (tableau de chaînes terminé par NULL)
    	int    p_proto;      // Le numéro officiel du protocole (ex: IPPROTO_TCP)
	}

## <span style="color: cyan;"> Get address info </span> ##

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>

	int getaddrinfo(const char *node, 
					const char *service,
					const struct addrinfo *hints,
					struct addrinfo **res);

### Résumé ###
	
	getaddrinfo() est la fonction moderne et standard utilisée pour convertir des adresses sous forme de texte en structures d'adresses réseau utilisables directement par socket(), connect() ou bind().

	struct addrinfo
	{
		int              ai_flags;      // Options (ex: AI_PASSIVE pour un serveur)
		int              ai_family;     // AF_INET, AF_INET6, ou AF_UNSPEC (indifférent)
		int              ai_socktype;   // SOCK_STREAM (TCP) ou SOCK_DGRAM (UDP)
		int              ai_protocol;   // Protocole (généralement 0 ou IPPROTO_TCP)
		socklen_t        ai_addrlen;    // Taille de ai_addr
		struct sockaddr *ai_addr;       // Pointeur vers sockaddr (IP + Port)
		char            *ai_canonname;  // Nom canonique de l'hôte
		struct addrinfo *ai_next;       // Pointeur vers le noeud suivant dans la liste
	}

	Vous devez impérativement appeler freeaddrinfo(res) une fois que vous avez fini de créer vos sockets pour éviter les fuites de mémoire (memory leaks):

		void freeaddrinfo(struct addrinfo *res);

### Valeur de retour ###

	0, ou -1 en cas d'erreur

## <span style="color: cyan;"> bind, listen, accept, connect </span> ##

	#include <sys/socket.h>

	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int listen(int sockfd, int backlog);
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

### Résumé ###
	
	bind() associe un socket brut (créé par socket()) à une adresse IP et un port précis de votre machine local.

	listen() transforme le socket en un socket d'écoute prêt à recevoir des demandes de connexion. Elle crée une file d'attente pour stocker les clients qui essaient de se connecter.

	accept() extrait la première demande de connexion dans la file d'attente de listen(). C'est une fonction bloquante (par défaut) : elle met le programme en pause jusqu'à ce qu'un client se connecte. accept() crée un tout nouveau socket dédié à la communication avec ce client précis. Le socket initial (server_fd) reste en écoute pour continuer d'accueillir d'autres clients.

	connect() est utilisée par le client pour initier la poignée de main TCP (handshake) avec un serveur distant en attente sur un port spécifique.

### Valeur de retour ###

	bind() : 0, ou -1 en cas d'erreur
	listen() : 0, ou -1 en cas d'erreur
	accept() : le File Descriptor du nouveau socket client, ou -1 en cas d'erreur
	connect() : 0 si la connection est établie, -1 en cas d'erreur

## <span style="color: cyan;"> htons, htonl, ntohs, ntohl </span> ##

	#include <arpa/inet.h>

	uint16_t htons(uint16_t hostshort);
	uint32_t htonl(uint32_t hostlong);
	uint16_t ntohs(uint16_t netshort);
	uint32_t ntohl(uint32_t netlong);

### Résumé ###
	
	Host to Network Short (16 bits) : convertit un port avant de l'envoyer sur le réseau.
	Host to Network Long (32 bits) : convertit une adresse IPv4 avant de l'envoyer.
	Network to Host Short (16 bits) : convertit un port reçu du réseau pour votre code.
	Network to Host Long (32 bits) : convertit une adresse IPv4 reçue pour votre code.


## <span style="color: cyan;"> inet_addr, inet_ntoa, inet_ntop </span> ##

	#include <arpa/inet.h>

	in_addr_t 	inet_addr(const char *cp);
	char *		inet_ntoa(struct in_addr in);
	const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

### Résumé ###
	
	Ces trois fonctions servent à convertir des adresses IP entre leur format binaire (utilisé par la carte réseau et la structure sockaddr_in) et leur format texte lisible par un humain (ex: "192.168.1.1").

## <span style="color: cyan;">  send, recv </span> ##

	#include <sys/socket.h>

	ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	ssize_t recv(int sockfd, void *buf, size_t len, int flags);

### Résumé ###
	
	Ce sont les deux fonctions fondamentales qui permettent d'émettre et de recevoir des données sur un socket connecté (généralement en TCP).

	send() copie les données de votre buffer mémoire vers le buffer d'envoi du système d'exploitation, qui se charge ensuite de les transmettre sur le réseau.

	recv() extrait les données reçues sur le réseau depuis le buffer de réception du système d'exploitation vers votre propre buffer C++.
	Par défaut, c'est une fonction bloquante : le programme s'arrête sur cette ligne jusqu'à ce qu'au moins un octet soit disponible sur le réseau.

### Valeur de retour ###

	send() :le nombre d'octets réellement acceptés par le système, 
			ou -1 en cas d'échec d'envoi

	recv() : le nombre d'octets lus, 
			ou 0 en cas de déconnexion propre du client/serveur, 
			ou -1 en cas d'erreur de lecture


