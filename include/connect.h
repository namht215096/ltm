#ifndef __CONNECT_H__
#define __CONNECT_H__

#include "message.h"
/**
 * Gửi thông điệp qua socket
 * @param sockfd socket để gửi thông điệp
 * @param msg thông điệp cần gửi
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int send_message(int sockfd, Message msg);

/**
 * Nhận thông điệp qua socket
 * @param sockfd socket để nhận thông điệp
 * @param msg thông điệp nhận được
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int recv_message(int sockfd, Message *msg);

/**
 * Tạo socket lắng nghe và gắn nó vào cổng chỉ định.
 *
 * @param port Số cổng để gắn socket vào.
 * @return Mô tả tệp của socket đã tạo, hoặc -1 nếu có lỗi xảy ra.
 */
int socket_create(int port);

/**
 * Chấp nhận kết nối trên socket đang nghe.
 *
 * @param sock_listen Mô tả tệp của socket đang nghe.
 * @return Mô tả tệp của kết nối đã chấp nhận, hoặc -1 nếu có lỗi xảy ra.
 */
int socket_accept(int sock_listen);
int client_start_conn(int sock_con);

int socket_connect(int port, char *host);
int server_start_conn(int sock_control);

#endif