#include <libgen.h>
#include <sys/socket.h>
#include <utils.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "color.h"
#include "command.h"
#include "common.h"
#include "connect.h"
#include "log.h"
#include "message.h"
#include "validate.h"

char *handle_path(char *dir) {
    char *home = getenv("HOME");
    char *path = (char *) malloc(SIZE);
    strcpy(path, home);
    strcat(path, "/");
    if (dir[0] == '~' || dir[0] == '.') {
        dir += 2;
    }
    strcat(path, dir);
    strcpy(dir, path);
    free(path);
    return dir;
}

void handle_upload(int sock_data, char *dir, int sock_control) {
    dir = handle_path(dir);
    printf("%s\n", dir);
    char compress_folder[SIZE];
    int fl = is_folder(dir);
    std::string str_log_message = fl ? "Uploading folder " : "Uploading file ";

    Message msg;
    recv_message(sock_control, &msg);
    if (msg.type == MSG_TYPE_ERROR) {
        printf(ANSI_COLOR_YELLOW "%s" ANSI_RESET "\n", msg.payload);
        std::string str_payload(msg.payload);
        log_message('e', str_payload.c_str());
        return;
    }

    if (is_folder(dir)) {
        strcpy(compress_folder, dir);
        char *last_past = basename(compress_folder);
        strcat(compress_folder, last_past);
        strcat(compress_folder, ".zip");
        zip(dir, compress_folder);
        strcpy(dir, compress_folder);

        send_message(sock_control, create_status_message(MSG_TYPE_DOWNLOAD_FOLDER, NO));
    } else
        send_message(sock_control, create_status_message(MSG_TYPE_DOWNLOAD_FILE, NO));

    std::string str_dir(dir);
    str_log_message += str_dir;
    FILE *fp = fopen(dir, "r");
    if (!fp) {
        send_message(sock_control, create_status_message(MSG_TYPE_ERROR, FILE_NOT_FOUND));
        perror(ANSI_COLOR_RED "error opening file" ANSI_RESET);
        str_log_message = str_dir + " not found";
        log_message('e', str_log_message.c_str());
    } else {
        send_message(sock_control, create_status_message(MSG_TYPE_OK, NO));
        printf("Uploading file %s to server!\n", dir);
        str_log_message = "Uploading file " + str_dir + " to server!";
        log_message('i', str_log_message.c_str());
        size_t byte_read;

        Message data;
        bool upload_success = true;
        do {
            byte_read = fread(data.payload, 1, PAYLOAD_SIZE, fp);
            data.type = MSG_TYPE_UPLOAD;
            data.length = byte_read;
            if (send_message(sock_data, data) < 0) {
                log_message('e', "Lỗi tải tệp lên máy chủ, dữ liệu quá lớn!");
                printf(ANSI_COLOR_RED "Lỗi tải tệp lên máy chủ, vui lòng thử lại sau!" ANSI_RESET
                                      "\n");
                send_message(sock_data,
                             create_status_message(MSG_TYPE_ERROR, STATUS_MESSAGE_TOO_LARGE));
                upload_success = false;
                break;
            }
            memset(data.payload, 0, PAYLOAD_SIZE);
        } while (byte_read > 0);

        send_message(sock_data, create_status_message(MSG_TYPE_OK, NO));
        str_log_message =
            upload_success ? "File " + str_dir + " uploaded" : "File " + str_dir + " not uploaded";
        log_message('i', str_log_message.c_str());
        fclose(fp);
        if (fl) {
            remove(compress_folder);
        }
    }
}

int _upload(int sock_control, int data_sock, char *arg, char *cur_dir) {
    // 1. Get file type from client
    Message msg;
    recv_message(sock_control, &msg);
    int is_file = msg.type == MSG_TYPE_DOWNLOAD_FILE ? 1 : 0;

    // 2. Handle initial checks
    recv_message(sock_control, &msg);
    if (msg.type == MSG_TYPE_ERROR) {
        return -1;
    }

    // 3. Prepare target path
    if (!is_file)
        strcat(arg, ".zip");
    char *path = construct_path(cur_dir, basename(arg));

    // 4. Open file for writing
    FILE *fp = fopen(path, "w");
    if (!fp) {
        return -1;
    }

    // 5. Receive and write file data
    bool upload_success = true;
    while (1) {
        recv_message(data_sock, &msg);
        if (msg.type == MSG_TYPE_UPLOAD) {
            fwrite(msg.payload, 1, msg.length, fp);
        } else if (msg.type == MSG_TYPE_ERROR) {
            upload_success = false;
            break;
        } else if (msg.type == MSG_TYPE_OK) {
            break;
        }
    }

    // 6. Cleanup and handle folder case
    fclose(fp);
    if (!upload_success) {
        remove(path);
    }
    if (!is_file && upload_success) {
        // Extract zip file for folders
        char *extracted_path = remove_zip_extension(path);
        unzip(path, extracted_path);
        remove(path);
    }
    return 0;
}

int server_upload(int sock_control, int data_sock, char *arg, char *cur_dir) {
    char *share_folder_path = (char *) malloc(sizeof(char) * SIZE);
    int share_mode = is_current_share_folder(cur_dir, share_folder_path);
    if (share_mode == -1) {
        char error_msg[] = "Bạn không nên tải tệp lên thư mục chia sẻ";
        std::string str_error_msg = "Trong thư mục chia sẻ, không nên tải tệp lên";
        server_log('w', str_error_msg.c_str());
        send_message(sock_control, create_message(MSG_TYPE_ERROR, error_msg));
        return -1;
    } else if (share_mode == 0) {
        char error_msg[] = "Lỗi: Bạn không có quyền truy cập vào thư mục này!";
        std::string str_error_msg = "Trong thư mục không có quyền truy cập, không nên tải tệp lên";
        server_log('e', str_error_msg.c_str());
        send_message(sock_control, create_message(MSG_TYPE_ERROR, error_msg));
        return -1;
    } else if (share_mode == 2) {
        send_message(sock_control, create_status_message(MSG_TYPE_OK, NO));
        _upload(sock_control, data_sock, arg, cur_dir);
    } else {
        send_message(sock_control, create_status_message(MSG_TYPE_OK, NO));
        _upload(sock_control, data_sock, arg, share_folder_path);
    }
    free(share_folder_path);
    return 0;
}
