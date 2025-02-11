#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <math.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_TAILLE 256
#define FILE_BUFFER_SIZE 1024
#define PORT 27016

int calcul_client(char *expression) {
    int a, b;
    char operateur;
    sscanf(expression, "%d %c %d", &a, &operateur, &b);

    switch (operateur) {
        case '+':
            return a + b;

        case '-':
            return a - b;

        case '*':
            return a * b;

        case '/':
            if (b != 0) {
                return a / b;
            } else {
            return 0;
            }

        default:
            return 0;
    }

}

void reception_fichier(SOCKET clientSock, const char *nom_fichier, const char *type_fichier) { 
    FILE *fichier = fopen(nom_fichier, "wb");
    if (fichier == NULL) {
        printf("Erreur ouverture fichier %s.\n", nom_fichier);
        return;
    }

    char buffer_fichier[FILE_BUFFER_SIZE];
    int donnee_recu;
    while ((donnee_recu = recv(clientSock, buffer_fichier, FILE_BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer_fichier, 1, donnee_recu, fichier);
        if (donnee_recu < FILE_BUFFER_SIZE) {
            break;
            }
    }
    fclose(fichier);

    printf("\nFichier %s recu avec succes.\n", nom_fichier);
}


void compter_mots(SOCKET clientSock, char *texte) {
    
    int mots = 0;
    char *element = strtok(texte, " ");
    while (element) {
        mots++;
        element = strtok(NULL, " ");
    }
    char reponse[BUFFER_TAILLE];
    printf("Nombre de mots : %d", mots);
}


void supprimer_espaces_finaux(char *str) {
    int len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') {
        str[len - 1] = '\0';
        len--;
    }
}


//PARTIE RNA
typedef struct {
    float poids_entree_couche_cachee[2][2];  
    float poids_couche_cachee_sortie[2];     
    float biais_couche_cachee[2];     
    float biais_sortie;     
} ReseauNeurones;

//frmarre un reseau de neuronne avec des valeurs aleatoire
void demarre_reseau(ReseauNeurones *reseau) {
    for (int i_entree = 0; i_entree < 2; i_entree++) {  // 2 entrees
        for (int i_couche_cachee = 0; i_couche_cachee < 2; i_couche_cachee++) {
            reseau->poids_entree_couche_cachee[i_entree][i_couche_cachee] = ((float)rand() / RAND_MAX) * 2 - 1; // Poids entre [-1, 1]
        }
    }

    for (int i_couche_cachee = 0; i_couche_cachee < 2; i_couche_cachee++) {
        reseau->poids_couche_cachee_sortie[i_couche_cachee] = ((float)rand() / RAND_MAX) * 2 - 1; //entre [-1, 1]
        reseau->biais_couche_cachee[i_couche_cachee] = ((float)rand() / RAND_MAX) * 2 - 1; //entre [-1, 1]
    }

    reseau->biais_sortie = ((float)rand() / RAND_MAX) * 2 - 1; 
}

//propagation avant
float propagation_avant(ReseauNeurones *reseau, float entree1, float entree2, float couche_cachee[2]) {
    //valeurs pour la couche cachee
    for (int i_couche_cachee = 0; i_couche_cachee < 2; i_couche_cachee++) {
        couche_cachee[i_couche_cachee] = tanhf(entree1 * reseau->poids_entree_couche_cachee[0][i_couche_cachee] + entree2 * reseau->poids_entree_couche_cachee[1][i_couche_cachee] + reseau->biais_couche_cachee[i_couche_cachee]);
    }

    float sortie = 0;
    for (int i_couche_cachee = 0; i_couche_cachee < 2; i_couche_cachee++) {
        sortie += couche_cachee[i_couche_cachee] * reseau->poids_couche_cachee_sortie[i_couche_cachee];
    }
    sortie += reseau->biais_sortie;

    return tanhf(sortie);
}

//retropropagation
void retropropagation(ReseauNeurones *reseau, float entree1, float entree2, float couche_cachee[2], float sortie, float cible) {
    float erreur = cible - sortie;

    //delta pour la sortie
    float delta_sortie = erreur * (1 - sortie * sortie);

    //Calcul = delta pour la couche cachee
    float delta_couche_cachee[2];
    for (int i_couche_cachee = 0; i_couche_cachee < 2; i_couche_cachee++) {
        delta_couche_cachee[i_couche_cachee] = delta_sortie * reseau->poids_couche_cachee_sortie[i_couche_cachee] * (1 - couche_cachee[i_couche_cachee] * couche_cachee[i_couche_cachee]);
    }

    //maj des poids et biais pour la sortie
    for (int i_couche_cachee = 0; i_couche_cachee < 2; i_couche_cachee++) {
        reseau->poids_couche_cachee_sortie[i_couche_cachee] += delta_sortie * couche_cachee[i_couche_cachee];
    }
    reseau->biais_sortie += delta_sortie;

    //Maj des poids et biais pour la couche cachee
    for (int i_couche_cachee = 0; i_couche_cachee < 2; i_couche_cachee++) {
        reseau->poids_entree_couche_cachee[0][i_couche_cachee] += delta_couche_cachee[i_couche_cachee] * entree1;
        reseau->poids_entree_couche_cachee[1][i_couche_cachee] += delta_couche_cachee[i_couche_cachee] * entree2;
        reseau->biais_couche_cachee[i_couche_cachee] += delta_couche_cachee[i_couche_cachee];
    }
}

//Entrainement du reseau neuron
void train_reseau(ReseauNeurones *reseau, float entrees[4][2], float sorties[4]) {
    int iterations = 0;
    int max_iterations = 1000;  
    while (iterations < max_iterations) {
        iterations++; 
        int erreur_bonne = 0;
        for (int i = 0; i < 4; i++) {
            float couche_cachee[2];
            float sortie = propagation_avant(reseau, entrees[i][0], entrees[i][1], couche_cachee);

            if (fabs(sortie - sorties[i]) < 0.001) {
                erreur_bonne++;
            } else {
                retropropagation(reseau, entrees[i][0], entrees[i][1], couche_cachee, sortie, sorties[i]);
            }
        }
        if (erreur_bonne == 4) {
            break;  // Si erreurs sont correctes stop
        }
    }
}


// pour les portes logiques
void* train_porte_et(void* arg) {
    ReseauNeurones* reseau = (ReseauNeurones*) arg;
    float entrees[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    float sorties[4] = {0, 0, 0, 1}; //ET
    train_reseau(reseau, entrees, sorties);
    return NULL;
}

void* train_porte_ou(void* arg) {
    ReseauNeurones* reseau = (ReseauNeurones*) arg;
    float entrees[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    float sorties[4] = {0, 1, 1, 1}; //OU
    train_reseau(reseau, entrees, sorties);
    return NULL;
}

void* train_porte_xor(void* arg) {
    ReseauNeurones* reseau = (ReseauNeurones*) arg;
    float entrees[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    float sorties[4] = {0, 1, 1, 0}; //XOR
    train_reseau(reseau, entrees, sorties);
    return NULL;
}



int main() {
    WSADATA wsaData;
    SOCKET sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    int portno = PORT;
    int clilen, n;
    char buffer[BUFFER_TAILLE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Erreur initialisation Winsock.\n");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Erreur socket.\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("Erreur liaison socket\n");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == SOCKET_ERROR) {
        printf("Erreur ecoute\n");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Serveur lance sur le port %d...\n", portno);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd == INVALID_SOCKET) {
        printf("Erreur acceptation\n");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Connexion d'un client : %s\n", inet_ntoa(cli_addr.sin_addr));

    memset(buffer, 0, BUFFER_TAILLE);
    while ((n = recv(newsockfd, buffer, BUFFER_TAILLE, 0)) > 0) {
        buffer[strcspn(buffer, "\r\n")] = 0;

        if (strcmp(buffer, "QUITTER") == 0 || strcmp(buffer, "quitter") == 0) {
            printf("Client a envoyé 'QUITTER'. Fermeture.\n");
            break;
        }

        if (strcmp(buffer, "IMAGE") == 0) {
            memset(buffer, 0, BUFFER_TAILLE); //Vide le buffer
            reception_fichier(newsockfd, "image_recue.jpg", "Image");
            continue;
        }

        if (strcmp(buffer, "VIDEO") == 0) {
            memset(buffer, 0, BUFFER_TAILLE); 
            reception_fichier(newsockfd, "video_recue.mp4", "Video");
            continue;
        }

        if (strcmp(buffer, "CALCUL" ) == 0 ) {
            memset(buffer, 0, BUFFER_TAILLE);
            n = recv(newsockfd, buffer, BUFFER_TAILLE, 0);
            int result = calcul_client(buffer);
            printf("Calcul recu : %s, Resultat : %d\n", buffer, result);
            snprintf(buffer, BUFFER_TAILLE, "%d", result);
            send(newsockfd, buffer, strlen(buffer), 0);
            continue;
        }

        if (strcmp(buffer, "COMPTER") == 0) {
            memset(buffer, 0, BUFFER_TAILLE); 
            char texte[BUFFER_TAILLE];
            memset(texte, 0, BUFFER_TAILLE);
            recv(newsockfd, texte, BUFFER_TAILLE, 0);
            texte[strcspn(texte, "\r\n")] = 0;
            compter_mots(newsockfd, texte);
            continue;
        }


        if (strcmp(buffer, "RNA") == 0) {
            char type_porte[16];
            
            //Reception du type de porte logique dit par le client
            recv(newsockfd, type_porte, sizeof(type_porte), 0);

            ReseauNeurones reseau;
            demarre_reseau(&reseau);


            if (strcmp(type_porte, "ET") == 0) {
                printf("Entraînement pour la porte logique ET :\n");
                train_porte_et(&reseau);
            } else if (strcmp(type_porte, "OU") == 0) {
                printf("Entraînement pour la porte logique OU :\n");
                train_porte_ou(&reseau);
            } else if (strcmp(type_porte, "XOR") == 0) {
                printf("Entraînement pour la porte logique XOR :\n");
                train_porte_xor(&reseau);
            } else {
                printf("Type de porte inconnu reçu : %s\n", type_porte);
                continue;
            }

            printf("\nTests de prédiction pour la porte %s :\n", type_porte);
            float couche_cachee[2];
            for (int entree1 = 0; entree1 <= 1; entree1++) {
                for (int entree2 = 0; entree2 <= 1; entree2++) {
                    float resultat = propagation_avant(&reseau, entree1, entree2, couche_cachee);
                    if (resultat >= 0.5f) {
                        printf("%d  %s  %d = %.5f-> arrondi : 1 \n", 
                            entree1, type_porte, entree2, resultat);
                    } else {
                        printf("%d  %s %d = %.5f-> arrondi : 0 \n", 
                            entree1, type_porte, entree2, resultat);
                    }

                }
            }
        }

        printf("Message recu : %s\n", buffer);
        const char *reponse = "Message recu par le serveur ";
        send(newsockfd, reponse, strlen(reponse), 0);

        memset(buffer, 0, BUFFER_TAILLE);
    }

    closesocket(newsockfd);
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
