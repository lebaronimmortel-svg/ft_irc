# ft_irc #

L'IP dit OÙ envoyer les données (la machine).
Le Port dit À QUI les donner sur cette machine (l'application).
Le Socket est le CANAL OUVERT (IP + Port) qui permet la connexion effective entre deux programmes.

## 1. Le Serveur TCP (server.cpp) ##
 
Le serveur suit ce cycle de vie :
	socket() $\rightarrow$ bind() $\rightarrow$ listen() $\rightarrow$ accept() $\rightarrow$ read() / write().

## 2. Le Client TCP (client.cpp) ##

Le client suit ce cycle de vie :
	socket() $\rightarrow$ connect() $\rightarrow$ write() / read().

## 3. Socket  ##


### int socket(int domain, int type, int protocol); ###

	Elle demande au système d'exploitation de réserver les ressources réseau et retourne un descripteur de fichier (un entier int) qui identifie l'embout de connexion créé.

## Valeur de retour ##