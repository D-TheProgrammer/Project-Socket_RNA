# Project-Socket_RNA 

# Client-Serveur avec Sockets – Communication et Fonctions Avancées  

Ce projet est un système client-serveur utilisant des sockets sous Windows. Il permet aux clients d’envoyer des messages au serveur, qui les affiche jusqu’à ce qu’une commande de fermeture soit reçue.  

## SOMMAIRE / SUMMARY  
- [Présentation en français / Presentation in French](#français)  
- [Présentation en anglais / Presentation in English](#english)

## __[FRANÇAIS]__
## 📌 Fonctionnalités principales  
- **Envoi de messages** : Le client peut envoyer du texte qui est affiché sur le terminal du serveur.  
- **Commande `IMAGE`** : Permet d’envoyer une image au serveur.  
- **Commande `VIDEO`** : Permet d’envoyer une vidéo au serveur.  
- **Commande `CALCUL`** : Effectue un calcul mathématique simple.  
- **Commande `COMPTER`** : Compte le nombre de mots dans une phrase envoyée.  
- **Commande `RNA`** : Lance un réseau de neurones artificiels pour les portes logiques (ET, OU, XOR).  
- **Fermeture de connexion** : `quitter` ou `QUITTER` permet d’arrêter la communication et de fermer la session.  

## 📌 Installation et utilisation  

### 1️ Démarrer le serveur  
Avant toute communication, vous devez lancer le serveur sur une machine en exécutant :  


### 2️ Lancer le client  
Sur une autre machine (ou un autre terminal), exécutez le client avec :  
```bash
.\TP2_serveur.exe
```

### 3️ Envoyer des messages  
Le client peut envoyer du texte simple, qui sera affiché sur le terminal du serveur.  
```bash
.\TP2_client.exe
```

### 4️ Utiliser les commandes spéciales  
- **IMAGE** : Envoie une image (`image_test.jpg`).  
- **VIDEO** : Envoie une vidéo (`video.mp4`).  
- **CALCUL** : Effectue un calcul mathématique simple (ex: `5 + 3`).  
- **COMPTER** : Compte le nombre de mots dans une phrase envoyée.  
- **RNA** : Entraîne un réseau de neurones artificiels pour les portes logiques.  

### 5️ Fermer la connexion  
Envoyez `quitter` ou `QUITTER` pour fermer la connexion entre le client et le serveur.  

## 📌 Compilation  

Ce projet a été développé sous **Windows**, donc certaines bibliothèques spécifiques (`-lws2_32`) sont nécessaires.  

### 🔧 Compilation des fichiers  
#### Serveur  
```bash
gcc TP2_serveur.c -o TP2_serveur -lws2_32
```

#### Client  
```bash
gcc TP2_client.c -o TP2_client -lws2_32
```

### 🚀 Exécution  
#### Démarrer le serveur  
```bash
.\TP2_serveur.exe
```
#### Lancer le client  
```bash
.\TP2_client.exe
```

## 📂 Ressources fournies  
- **`image_test.jpg`** : Fichier image test.  
- **`video.mp4`** : Fichier vidéo léger pour les tests.  

💡 *Projet réalisé sous Windows – certaines importations peuvent être différentes sur d'autres systèmes d’exploitation.* 🚀

---

## __[ENGLISH]__  
## 📌 Main features
- **Sending messages**: The client can send text that is displayed on the server terminal.
- **`IMAGE` command**: Allows you to send an image to the server.
- **`VIDEO` command**: Allows you to send a video to the server.
- **`CALCULATION` command**: Performs a simple mathematical calculation.
- **`COUNT` command**: Counts the number of words in a sent sentence.
- **`RNA` command**: Launches an artificial neural network for logic gates (AND, OR, XOR).
- **Connection closure**: `quit` or `QUIT` stops the communication and closes the session.


## 📌 Installation and use

### 1️ Start the server
Before any communication, you must launch the server on a machine by running:

### 2️ Launch the client
On another machine (or another terminal), run the client with:
```bash
.\TP2_serveur.exe
```

### 3️ Send messages
The client can send simple text, which will be displayed on the server terminal.
```bash
.\TP2_client.exe
```

### 4️ Use special commands
- **IMAGE**: Sends an image (`image_test.jpg`).
- **VIDEO**: Sends a video (`video.mp4`).
- **CALCUL**: Performs a simple mathematical calculation (e.g.: `5 + 3`).
- **COMPTER** : Counts the number of words in a sent sentence.
- **RNA** : Trains an artificial neural network for logic gates.

### 5️ Close connection
Send `quitter` or `QUITTER` to close the connection between the client and the server.


## 📌 Compilation

This project was developed under **Windows**, so some specific libraries (`-lws2_32`) are required.

### 🔧 Compiling files
#### Server
```bash
gcc TP2_serveur.c -o TP2_serveur -lws2_32
```

#### Client
```bash
gcc TP2_client.c -o TP2_client -lws2_32
```

### 🚀 Execution
#### Start the server
```bash
.\TP2_serveur.exe
```
#### Start the client
```bash
.\TP2_client.exe
```

## 📂 Resources provided
- **`image_test.jpg`** : Test image file.
- **`video.mp4`** : Light video file for testing.

💡 *Project made on Windows – some imports may be different on other operating systems.* 🚀
