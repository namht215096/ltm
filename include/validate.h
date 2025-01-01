#ifndef __VALIDATE_H__
#define __VALIDATE_H__

#define INVALID_IP -1

/**
 * Xác thực địa chỉ IP
 * @param ip địa chỉ IP
 * @return 0 nếu hợp lệ, -1 nếu không hợp lệ
 */
int validate_ip(const char *ip);

/**
 * Kiểm tra xem đường dẫn có phải là thư mục không
 * @param path đường dẫn cần kiểm tra
 * @return 1 nếu đường dẫn là thư mục, 0 nếu không phải
 */
int is_folder(const char *path);

/**
 * Xác thực đường dẫn là tệp hay thư mục
 * @param path đường dẫn cần xác thực
 * @return 0 nếu hợp lệ, -1 nếu không hợp lệ
 */
int validate_file_or_dir(const char *path);

#endif   // !
