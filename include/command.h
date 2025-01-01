#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

/**
 * Chạy lệnh trên server, gửi kết quả cho client qua sockfd
 * @param sockfd: socket để gửi kết quả đến
 * @param cmd: lệnh để chạy
 * @param cur_dir: thư mục hiện tại của người dùng
 */
int process_command(int sockfd, char* cmd, char* cur_dir);

/**
 * Kiểm tra xem thư mục có phải là thư mục con của base_dir không
 * @param base_dir: thư mục cơ sở
 * @param dir: thư mục cần kiểm tra
 * @return 1 nếu đúng, 0 nếu sai
 */
int is_subdir(const char* base_dir, const char* dir);

/**
 * Thay đổi thư mục
 * Trả về -1 nếu có lỗi, 0 nếu thành công
 */
int server_cd(int sock_control, char* folderName, char* user_dir, char* cur_user_dir);

/**
 * @brief Liệt kê các tệp và thư mục trong thư mục làm việc hiện tại
 * @param sockfd Socket để nhận dữ liệu
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int handle_list(int sockfd);

/**
 * @brief Server xử lý liệt kê các tệp và thư mục trong thư mục làm việc hiện tại
 * @param sockfd Socket để gửi dữ liệu
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int server_list(int sockfd);

/**
 * Lấy thư mục hiện tại
 * @param sockfd socket điều khiển
 */
const char* handle_pwd(char* cur_user, char* user_dir);

/**
 * Thoát khỏi server
 * @param sockfd socket
 * @param cur_user người dùng hiện tại
 */
void server_quit(int sockfd, char* cur_user);

/**
 * Liệt kê các tệp trong thư mục hiện tại
 * @param sockfd: socket để gửi kết quả đến
 * @param arg: đối số của lệnh
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int server_find(int sockfd, char* arg);

/**
 * Chia sẻ tệp với người dùng khác bằng cách gửi link
 * @param sockfd: socket để gửi kết quả đến
 * @param arg: đối số của lệnh
 * @param user_dir: thư mục hiện tại của người dùng
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int server_share(int sockfd, char* arg, char* user_dir);

/**
 * Client tải tệp hoặc thư mục từ server
 * @param data_sock socket dữ liệu
 * @param sock_control socket điều khiển
 * @param arg tệp hoặc thư mục để tải về
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int handle_download(int data_sock, int sock_control, char* arg);

/**
 * Tải tệp hoặc thư mục từ server
 * @param sock_control socket để gửi trạng thái 
 * @param sock_data socket để gửi file/thư mục cho client
 * @param dir thư mục để tải về
 */
void server_download(int sock_control, int sock_data, char* path);

/**
 * Tải danh sách các tệp từ server
 */
int handle_pipe_download(int sockfd, std::string files);

void server_pipe_download(int sockfd, char* output);

void handle_upload(int data_sock, char* filename, int sock_control);
int server_upload(int sock_control, int sock_data, char* filename, char* user_dir);

/**
 * Tải file/folder được chia sẻ. Thông tin chia sẻ được lưu trong file .share
 * Lưu vào trong thư mục share
 */
int load_shared_file(char* user_dir);

int check_permision(const char* dir, const char* share_path, char* share_folder_path);

int is_current_share_folder(char* dir, char* share_folder_path);
#endif   // !__COMMAND_H__