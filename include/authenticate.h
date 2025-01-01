#ifndef __AUTHENTICATE_H__
#define __AUTHENTICATE_H__

#include <cstdio>

#include "message.h"

void sha256(const char *input, char *output);

/**
 * Đăng nhập vào cppdrive
 * @param sockfd socket cho kết nối điều khiển
 * @param cur_user người dùng hiện tại đã đăng nhập
 * @return 1 nếu xác thực thành công, 0 nếu không
 */
int handle_login(int sockfd, char *cur_user);

/**
 * Đăng ký tài khoản mới
 * @param sockfd socket cho kết nối điều khiển
 * @return 1 nếu thành công, 0 nếu không
 */
int register_acc(int sockfd);

/**
 * Xác thực thông tin người dùng
 * @param user tên người dùng
 * @param pass mật khẩu
 * @param user_dir lưu đường dẫn đến thư mục gốc của người dùng
 * @return 1 nếu xác thực thành công, 0 nếu không
 */
int check_user_acc(char *user, char *pass, char *user_dir);

/**
 * Kiểm tra xem tên người dùng có tồn tại trong cơ sở dữ liệu không
 * @param user tên người dùng
 * @return 1 nếu tồn tại, 0 nếu không
 */
int check_username(char *user);

/**
 * Xử lý đăng nhập của server
 * @param msg thông điệp nhận được
 * @param user_dir đường dẫn đến thư mục gốc của người dùng
 * @return 1 nếu xác thực thành công, 0 nếu không
 */
int server_login(Message msg, char *user_dir);

/**
 * Xử lý đăng ký tài khoản mới của server
 * @param msg thông điệp nhận được
 * @return 1 nếu thành công, 0 nếu không
 */
int server_register(Message msg);

#endif
