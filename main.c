// Include STANDARD et define
// Made by http://rodolphe.barbanneau.free.fr/sub/projets/c/1/pro.php
// I will modify this code in Dancing Plastic

#include <cstdlib>
#include <iostream>

#define MAXCHARACTER 2048

using namespace std;

// Include SOCKETS (respecter l'ordre !!!)
#include <winsock2.h>                       // "PROJECT/Configuration/Linker/Input/Additional Dependencies" : ws2_32.lib (VS2010)
#include <windows.h>

DWORD WINAPI thread(LPVOID lpParameter);    // Thread pour rester en lecture
void sendLine(SOCKET S, char *text);        // Fonction pour envoyer une cha�ne de caract�res
void dataUse(char text[]);                  // Fonction pour traiter les donn�es re�ues

SOCKET mySocket;                            // Notre Socket
char cBuffer;                               // Caract�re buffer
char tBuffer[MAXCHARACTER];                 // Texte buffer

// Adresse et port du serveur IRC auquel on se connecte
char serverAddress[] = "uk.quakenet.org";
unsigned int serverPort = 6667;

// Param�tres
char channels[] = "JOIN "                   // Channels rejoints lors de la premi�re connection
                  "#robochBOT1,"
                  "#robochBOT2,"
                  "\r\n";



int main(int argc, char *argv[])
{
    // Initialisation winsock !!!
    WSADATA WSAData;
    WSAStartup(MAKEWORD (2,0), &WSAData );

    // Initialisation du socket
    mySocket = socket(AF_INET, SOCK_STREAM, 0);

    //
    HOSTENT *hostName;                      // Nom de domaine
    char *ipResult;                         // Adresse Ip
    sockaddr_in sock_in;
    sock_in.sin_family = AF_INET;           // Famille
    sock_in.sin_port = htons(serverPort);   // Port

    // Recherche de l'adresse IP � partir du nom de domaine sp�cifi�
    hostName = gethostbyname(serverAddress);
    ipResult = inet_ntoa(*((struct in_addr *)hostName->h_addr));
    sock_in.sin_addr.s_addr = inet_addr(ipResult);



    // Rapelle des informations serveur
    cout << "Server address : " << serverAddress << endl;
    cout << "IP : " << ipResult << endl;

    // Cr�action du lien
    cout << "Connecting...\n" << endl;
    connect(mySocket, (sockaddr *)&sock_in, sizeof(sockaddr));

    // Envoi des requ�tes (on est connect�)
    sendLine(mySocket,"NICK robochBOT\r\n");
    sendLine(mySocket,"USER robochBOT rodolphe.barbanneau.free.fr irc.quakenet.org RBarbanneau\r\n");

    // Cr�ation du thread de lecture
    DWORD threadID;
    CreateThread(NULL, 0, thread, NULL, 0, &threadID);

    // Pause
    int pause;
    cin >> pause;

    // Fermeture du socket
    closesocket(mySocket);
    WSACleanup();



    system("PAUSE");
    return EXIT_SUCCESS;
}



DWORD WINAPI thread(LPVOID lpParameter)
{
    int i = 0;

    while (1) {
        // Lecture du caract�re buffer
        recv(mySocket, &cBuffer, 1, 0);
        // Incrustation du caract�re buffer re�u dans tBuffer
        if (cBuffer == '\n') {
            *(tBuffer + i) = cBuffer;
            *(tBuffer + ++i) = '\0';
            dataUse(tBuffer);
            i = 0;
        }
        else {
            *(tBuffer + i) = cBuffer;
            i++;
        }
    }
}



void sendLine(SOCKET S, char *text)
{
    send(S, text, strlen(text), 0);
}



void dataUse(char text[])
{
    static int motd = 0;

    // Affichage de la ligne re�u en tBuffer
    cout << text;

    // R�ponse au PING du server IRC
    if (!strncmp(text, "PING", 4)) {
        memcpy(text, "PONG", 4);
        sendLine(mySocket, text);
        cout << text;
        return;
    }

    // D�tection du MOTD ("Message Of The Day")
    if (!motd && !strncmp(text, ":", 1)) {
        int i = 0;
        while (text[i] != ' ')
            i++;
        sendLine(mySocket, channels);
        motd = 1;
        return;
    }



    // =================================================================
    // Fonctions IRC bot
    // =================================================================

    // Ici on peut d�velopper des fonctionnalit�s du bot

    // ...
}
