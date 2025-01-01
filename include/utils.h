#ifndef __UTILS_H__
#define __UTILS_H__

#include <sstream>
#include <string>
#include <vector>

#include "color.h"

#define UP_ARROW   "\033[A"
#define DOWN_ARROW "\033[B"

/**
 * Kiểm tra xem một tiến trình có phải là client không
 * @param process tên tiến trình
 * @return 1 nếu đúng, 0 nếu sai
 */
int is_client(const char *process);

void read_input(char *user_input, int size);

/**
 * Lấy tên người dùng từ đường dẫn
 * @param path đường dẫn đến thư mục người dùng
 * @return con trỏ tới tên người dùng
 */
char *get_username(char *path);

/**
 * Tạo thư mục người dùng
 * @param path đường dẫn đến thư mục người dùng
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int create_dir(const char *path);

/**
 * Tạo tệp nếu chưa tồn tại
 * @param path đường dẫn đến tệp
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int create_file(const char *path);

// Hàm khóa hoặc mở khóa người dùng trong tệp .auth (0 để mở khóa, 1 để khóa)
void toggle_lock(const char *username, int lockStatus);

/**
 * In chuỗi vào giữa màn hình
 * @param text chuỗi cần in
 */
void print_centered(const char *text);

/**
 * In menu và lấy lựa chọn của người dùng
 * @param menu_items mảng các tùy chọn để hiển thị
 * @param num_items số lượng tùy chọn
 */
int process_menu(const char *menu_items[], int num_items);

/**
 * Lấy chuỗi prompt từ người dùng hiện tại và thư mục hiện tại
 * @param cur_user người dùng hiện tại
 * @param user_dir cây thư mục hiện tại
 * @return chuỗi prompt "[user@cppdrive ~/documents/]"$"
 */
char *handle_prompt(char *cur_user, char *user_dir);

/**
 * Cắt bỏ khoảng trắng và ký tự kết thúc dòng trong một chuỗi
 */
void trimstr(char *str, int n);

/**
 * Tách chuỗi theo dấu phân cách
 * @param s chuỗi cần tách
 * @param delim dấu phân cách
 * @return vector các chuỗi
 */
std::vector<std::string> split(const std::string &s, char delim);

/**
 * Nén thư mục
 * @param folder đường dẫn đến thư mục
 * @param compress_path đường dẫn đến tệp nén
 */
void zip(const char *folder, const char *compress_path);

/**
 * Giải nén thư mục
 * @param compressed_path đường dẫn đến thư mục nén
 * @param extract_path đường dẫn đến thư mục giải nén
 */
void unzip(const char *compressed_path, const char *extract_path);

#endif   // !__UTILS