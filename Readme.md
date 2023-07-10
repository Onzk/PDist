# Module de simulation de gestion de température

### Projet réalisé par le groupe 10 :
- AGNEKITOM Tchilabalo
- WOTOBOE Koffi Roland
- GAMLIGO Mawena Charles
- KOUDOSSOU Messan Dhani Justin


### Prérequis :

- Système d'exploitation Linux 64 Bits
- Avoir GCC et Java, installés sur le système 
- Installer le paquet libjson-c-dev avec :
    ````bash
    sudo apt install libjson-c-dev
    ````
    
### Comment démarrer le programme

- Se placer à la racine du projet.
- Compiler l'ensemble des fichiers et programmes du fichier avec la commande suivante :
````bash
gcc systeme/c/CommunicationTemperature.c -ljson-c -o systeme/c/CT -g3 & gcc systeme/c/GestionConsole.c -ljson-c -o systeme/c/GC -g3 & gcc piece/c/Chauffage.c -ljson-c -o piece/c/Chauffage -g3 & gcc piece/c/Thermometre.c -ljson-c -o piece/c/Thermometre -g3 & gcc console/c/Commande.c -ljson-c -o console/c/Commande -g3 & gcc console/c/Controle.c -ljson-c -o console/c/Controle -g3 & javac piece/java/Air.java & javac systeme/java/ServerRMI.java & javac console/java/ControleRMI.java
````

- Démarrer les programmes du système central avec les commandes suivantes :
````bash
./systeme/c/CT
````
````bash
./systeme/c/GC
````
````bash
java systeme.java.ServerRMI
````

- Démarrer les programmes de la piece avec les commandes suivantes :
```` bash
java piece.java.Air 230.0.0.0 8878 nom_de_la_piece
````
```` bash
./piece/c/Thermometre
````
```` bash
./piece/c/Chauffage
````


- Démarrer les consoles de contrôle et de commande avec les commandes suivantes :
````bash
./console/c/Controle
````
````bash
./console/c/Commande
````
````bash
java console.java.ControleRMI
````


## NB 

Le programme Air.java, nécessite pour démarrer (dans certains cas), la connection à un réseau 
    wifi, avec ou sans connection Internet.
