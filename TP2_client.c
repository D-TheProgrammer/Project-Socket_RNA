#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27016
#define BUFFER_SIZE 256

void envoyer_fichier(SOCKET sockfd, const char *chemin, const char *type) {
    FILE *fichier = fopen(chemin, "rb");
    if (!fichier) {
        printf("Impossible d'ouvrir le fichier %s\n", chemin);
        return;
    }

    fseek(fichier, 0, SEEK_END);
    long taille_fichier = ftell(fichier);
    rewind(fichier);

    char *buffer_fichier = (char *)malloc(taille_fichier);
    fread(buffer_fichier, 1, taille_fichier, fichier);
    fclose(fichier);

    send(sockfd, type, strlen(type), 0);
    send(sockfd, buffer_fichier, taille_fichier, 0);
    free(buffer_fichier);

    printf("%s envoyé avec succès\n", type);
    printf("Souhaitez-vous l'afficher ? (YES/NO) : ");
    char choix[BUFFER_SIZE];
    fgets(choix, sizeof(choix), stdin);
    choix[strcspn(choix, "\r\n")] = 0;

    if (strcmp(choix, "YES") == 0 || strcmp(choix, "yes") == 0) {
        char commande[BUFFER_SIZE];
        snprintf(commande, sizeof(commande), "\"%s\"", chemin);
        system(commande);
        printf("%s affiché.\n", type);
    }
}

void supprimer_espaces_finaux(char *str) {
    int len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') {
        str[len - 1] = '\0';
        len--;
    }
}


int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    int n;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Erreur initialisation Winsock : %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Erreur création socket\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("Erreur connexion\n");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur sur le port %d\n", PORT);

    do {
        memset(buffer, 0, sizeof(buffer));
        printf("Message a envoyer ecrivez ce que vous voulez (les fonctions speciales en dessous) :\n");
        printf("'QUITTER' pour fermer, 'IMAGE', 'VIDEO', 'CALCUL', 'COMPTER' (nb mot), 'RNA' .\n");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\r\n")] = 0;

        if (strcmp(buffer, "IMAGE") == 0 || strcmp(buffer, "image") == 0) {
            printf("Entrez le chemin de l'image : ");
            char chemin_image[BUFFER_SIZE];
            memset(chemin_image, 0, sizeof(chemin_image));
            fgets(chemin_image, sizeof(chemin_image), stdin);
            chemin_image[strcspn(chemin_image, "\r\n")] = 0;
            envoyer_fichier(sockfd, chemin_image, "IMAGE");
            continue;
        }

        if (strcmp(buffer, "VIDEO") == 0 || strcmp(buffer, "video") == 0) {
            printf("Entrez le chemin de la vidéo : ");
            char chemin_video[BUFFER_SIZE];
            memset(chemin_video, 0, sizeof(chemin_video)); 
            fgets(chemin_video, sizeof(chemin_video), stdin);
            chemin_video[strcspn(chemin_video, "\r\n")] = 0;
            envoyer_fichier(sockfd, chemin_video, "VIDEO");
            continue;
        }

        if (strcmp(buffer, "COMPTER") == 0 || strcmp(buffer, "compter") == 0) {
            printf("Entrez un texte pour le mot compte : ");
            char texte[BUFFER_SIZE];
            memset(texte, 0, sizeof(texte)); 
            fgets(texte, sizeof(texte), stdin);
            texte[strcspn(texte, "\r\n")] = 0;
            send(sockfd, "COMPTER", 7, 0);
            send(sockfd, texte, strlen(texte), 0);
            continue;
        }

        if (strcmp(buffer, "CALCUL") == 0 || strcmp(buffer, "calcul") == 0) {
            char calcul_client[BUFFER_SIZE];
            printf("Entrez un calcul (ex: 5+3) : ");
            memset(calcul_client, 0, sizeof(calcul_client)); 
            fgets(calcul_client, sizeof(calcul_client), stdin);
            calcul_client[strcspn(calcul_client, "\r\n")] = 0;
            send(sockfd, "CALCUL", 6, 0);
            send(sockfd, calcul_client, strlen(calcul_client), 0);
            continue;
        }


        if (strcmp(buffer, "RNA") == 0 || strcmp(buffer, "rna") == 0) {
            send(sockfd, "RNA", 3, 0);

            printf("Quel type de porte logique ? (ET, OU, XOR) : ");
            char type_porte[16];
            fgets(type_porte, sizeof(type_porte), stdin);
            type_porte[strcspn(type_porte, "\r\n")] = 0; 
            send(sockfd, type_porte, strlen(type_porte), 0);
            printf("Type de porte envoyé : %s\n", type_porte);
            continue;
        }



        if (strcmp(buffer, "QUITTER") == 0) {
            printf("Déconnexion demandée.\n");
            break;
        }

        n = send(sockfd, buffer, strlen(buffer), 0);
        if (n == SOCKET_ERROR) {
            printf("Erreur d'envoi.\n");
            closesocket(sockfd);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        memset(buffer, 0, sizeof(buffer)); 
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n > 0) {
            printf("Réponse du serveur : %s\n", buffer);
        } else if (n == 0) {
            printf("Connexion fermée par le serveur.\n");
            break;
        } else {
            printf("Erreur de réception : %d\n", WSAGetLastError());
        }
    } while (1);


    closesocket(sockfd);
    WSACleanup();

    return 0;
}
