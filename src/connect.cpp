#include "connect.h"

#include <arpa/inet.h>
#include <errno.h>
#include <log.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "common.h"
#include "crypto.h"
#include "utils.h"

int send_message(int sockfd, Message msg) {
    // printf("\n===============SENDING============\n");
    // print_message(msg);
    // printf("\n============END SENDING=========\n");
    std::string encrypted_payload = "";
    if (!SYMMETRIC_KEY.empty() && strlen(msg.payload) > 0) {
        std::string payload(msg.payload, msg.length);
        int len = encrypt_data(SYMMETRIC_KEY, payload, encrypted_payload);
        if (len < 0) {
            printf("Error encrypting data\n");
            return -1;
        }
        if (len > PAYLOAD_SIZE) {
            std::string message = "Payload too large: " + std::to_string(len);
            print_message(msg);
            log_message('e', message.c_str());
            server_log('e', message.c_str());
            return -1;
        }

        memcpy(msg.payload, encrypted_payload.data(), len);
        msg.length = len;
    }
    int dataLength, nLeft, idx;
    nLeft = sizeof(Message);
    idx = 0;
    while (nLeft > 0) {
        dataLength = send(sockfd, (char *) &msg + idx, nLeft, 0);
        if (dataLength <= 0) {
            close(sockfd);
            return -1;
        }
        nLeft -= dataLength;
        idx += dataLength;
    }
    return 1;
}

int recv_message(int socket, Message *msg) {
    char recvBuff[PAYLOAD_SIZE];
    int ret, nLeft, idx, bytes_recv;
    Message recvMessage;
    ret = 0;
    idx = 0;
    nLeft = sizeof(Message);
    while (nLeft > 0) {
        bytes_recv = nLeft > PAYLOAD_SIZE ? PAYLOAD_SIZE : nLeft;
        ret = recv(socket, recvBuff, bytes_recv, 0);
        if (ret <= 0) {
            close(socket);
            return -1;
        }
        memcpy(&(((char *) &recvMessage)[idx]), recvBuff, ret);
        idx += ret;
        nLeft -= ret;
    }
    msg->type = recvMessage.type;
    memcpy(msg->payload, recvMessage.payload, recvMessage.length + 1);
    msg->length = recvMessage.length;

    if (!SYMMETRIC_KEY.empty() && strlen(msg->payload) > 0) {
        std::string encrypted_payload(msg->payload, msg->length);
        std::string decrypted_payload;
        int len = decrypt_data(SYMMETRIC_KEY, encrypted_payload, decrypted_payload);
        if (len < 0) {
            printf(ANSI_BOLD ANSI_COLOR_RED "Error decrypting data" ANSI_RESET "\n");
            printf("Decrypted payload: %s\n", decrypted_payload.c_str());
            return -1;
        }
        if (len > PAYLOAD_SIZE) {
            printf("Payload too large: %d\n", len);
            return -1;
        }

        memcpy(msg->payload, decrypted_payload.data(), len);
        msg->length = len;
    }
    return 1;
}

int socket_create(int port) {
    int sockfd;
    struct sockaddr_in sock_addr;

    // create new socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        return -1;
    }

    // set local address info
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // socket sẽ lắng nghe trên tất cả các địa chỉ IP của máy tính.

    // bind
    int flag = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag));
    if (bind(sockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
        close(sockfd);
        perror("bind() error");
        return -1;
    }

    // begin listening for incoming TCP requests
    if (listen(sockfd, MAX_CLIENTS) < 0) {
        perror("listen() error");
        return -1;
    }
    return sockfd;
}

int socket_accept(int sock_listen) {
    int sockfd;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    sockfd = accept(sock_listen, (struct sockaddr *) &client_addr, &len);

    if (sockfd < 0) {
        perror("accept() error");
        return -1;
    }
    return sockfd;
}

/**
 * Tạo một kế nối chỉ sử dụng để truyền dữ liệu 
 */
int client_start_conn(int sock_con) {

    int sock_listen = socket_create(DEFAULT_PORT);

    // send an ACK on control conn
    int ack = 1;
    if ((send(sock_con, &ack, sizeof(ack), 0)) < 0) {
        printf("client: ack write error :%d\n", errno);
        exit(1);
    }

    int sock_conn = socket_accept(sock_listen);
    close(sock_listen);
    return sock_conn;
}

/**
 * tạo socket mới kết nối đến bên còn lại tại cổng và địa chỉ IP của bên kia 
 * Trả về:	định danh socket nếu thành công, -1 nếu có lỗi
 */
int socket_connect(int port, char *host) {
    int sockfd;
    struct sockaddr_in dest_addr;

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error creating socket");
        return -1;
    }

    // create server address
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(host);

    // Connect on socket
    if (connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) < 0) {
        perror("error connecting to server");
        return -1;
    }
    return sockfd;
}

/**
 * Tạo kết nối chỉ truyền dữ liệu 
 * Trả về: socket cho kết nối dữ liệu
 * hoặc -1 nếu có lỗi
 */
int server_start_conn(int sock_control) {
    char buf[1024];
    int wait, sock_data;

    // Wait for go-ahead on control conn
    if (recv(sock_control, &wait, sizeof wait, 0) < 0) {
        perror("Error while waiting");
        return -1;
    }

    // Lấy thông tin địa chỉ client 
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);  
    getpeername(sock_control, reinterpret_cast<struct sockaddr *>(&client_addr),
                &len);   

    // lưu địa chỉ IP chủa client vào buf
    inet_ntop(AF_INET, &client_addr.sin_addr, buf, sizeof(buf));

    // Initiate data connection with client
    if ((sock_data = socket_connect(DEFAULT_PORT, buf)) < 0)
        return -1;

    return sock_data;
}
