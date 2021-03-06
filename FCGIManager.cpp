//
// Created by ola on 17.02.17.
//

#include <cstdio>
#include <unistd.h>
#include <string>
#include <cstring>
#include "FCGIManager.h"
#include "constants.h"

using namespace std;

FCGIManager::FCGIManager(const char *host, int port) : ConnectionManager(host, port) {}

int FCGIManager::connectSocket() {
    int rc = connect(descriptor, (sockaddr *) &socketStruct, sizeof(socketStruct));
    if (rc == -1) {
        perror("Error connecting to socket");
        return -1;
    }
    return 0;
}

int FCGIManager::createConnection() {
    if (createSocket() == -1) return -1;
    createSockaddr();
    if (connectSocket() == -1) return -1;
    return 0;
}

void FCGIManager::sendMessage(int clientSocketFd) {
    std::cout << "\n***MESSAGE FROM FCGI TO CLIENT\n";
    unsigned char message_buf[8];
    ssize_t readBytes = 0;
    unsigned char end_header[8];
    end_header[0] = FCGI_VERSION;
    end_header[1] = FCGI_END_REQUEST;
    end_header[4] = ZERO;               // content length - always eight
    end_header[5] = 8;                  // content length - always eight
    end_header[6] = ZERO;               // padding length - always zero
    end_header[7] = ZERO;               // reserved - always zero
    // Copying the two bytes of requestId
    if (recv(descriptor, &message_buf, 8, 0) == 0) {
        cout << "FCGI returned empty message!\n";
        return;
    }
    end_header[2] = message_buf[2];
    end_header[3] = message_buf[3];

    bool stop_reading = false;
    while (1) {
        // Reading 8 bytes at a time
        readBytes = recv(descriptor, &message_buf, sizeof(message_buf), 0);
        if (readBytes == 0) {
            const char fcgiErr[43] = "An error with a fcgi connection occured. \n";
            write(clientSocketFd, fcgiErr, sizeof(fcgiErr) - 1);
            cout << "An error with a fcgi connection occured. \n" << endl;
            break;
        }
        if (strcmp((const char *) message_buf, (const char *) end_header) == 0) {
            // cout << "END HEADER FOUND \n";
            stop_reading = true;
        } else {
            if (stop_reading) {
                // get the appStatus here
                int appStatus = int(message_buf[0] << 24 |
                                    message_buf[1] << 16 |
                                    message_buf[2] << 8 |
                                    message_buf[3]);
                std::cout << "***END OF MESSAGE FROM FCGI TO CLIENT WITH STATUS " << appStatus << std::endl;
                break;
            }
            try {
                send(clientSocketFd, &message_buf, (size_t) readBytes, MSG_NOSIGNAL);
            }
            catch (std::exception &e) {
                std::cout << e.what();
                perror("Connection with client canceled.");
                return;
            }
        }
    }
}

FCGIManager::~FCGIManager() {}
