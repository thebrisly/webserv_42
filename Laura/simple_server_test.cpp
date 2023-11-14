#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

void handleRequest(int clientSocket, const char* request) {
    // Traiter la requête ici
    // (Note: Vous devrez implémenter la logique pour gérer différentes méthodes HTTP, URI, etc.)
    
    // Pour cet exemple, nous renvoyons une réponse simple

    (void) request;
    
    const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
    send(clientSocket, response, strlen(response), 0);
}

int main() {
    // Créer un socket serveur
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8002);

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, SOMAXCONN);

    std::cout << "[2023-11-14 12:40:54] Initializing Servers..." << std::endl;
    std::cout << "[2023-11-14 12:40:54] Server Created: ServerName[localhost] Host[127.0.0.1] Port[8002]" << std::endl;

    while (true) {
        // Accepter une connexion entrante
        int clientSocket = accept(serverSocket, NULL, NULL);

        // Vous pouvez récupérer l'adresse IP du client avec getpeername()
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        getpeername(clientSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        std::cout << "[2023-11-14 12:41:03] New Connection From " << inet_ntoa(clientAddr.sin_addr)
                  << ", Assigned Socket " << clientSocket << std::endl;

        // Lire la requête du client
        char buffer[1024];
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "[2023-11-14 12:41:03] Request Received From Socket " << clientSocket
                  << ", Content:\n" << buffer << std::endl;

        // Traiter la requête et envoyer la réponse
        handleRequest(clientSocket, buffer);

        // Fermer la connexion client
        close(clientSocket);
    }

    // Fermer le socket serveur
    close(serverSocket);

    return 0;
}
