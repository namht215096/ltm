#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "status.h"

#define PAYLOAD_SIZE 1024

typedef enum {
    MSG_TYPE_OK,
    MSG_TYPE_ERROR,
    MSG_TYPE_AUTHEN,
    MSG_TYPE_REGISTER,
    MSG_TYPE_CLEAR,
    MSG_TYPE_LS,
    MSG_TYPE_CD,
    MSG_TYPE_QUIT,
    MSG_TYPE_DOWNLOAD,
    MSG_TYPE_UPLOAD,
    MSG_TYPE_PWD,
    MSG_TYPE_FIND,
    MSG_TYPE_PIPE,
    MSG_TYPE_SHARE,
    MSG_TYPE_BASIC_COMMAND,   // mv, cp, rm, mkdir, touch
    MSG_TYPE_RELOAD,
    MSG_DATA_PUBKEY,
    MSG_DATA_FIND,
    MSG_DATA_LS,
    MSG_DATA_CD,
    MSG_DATA_CMD,
    MSG_TYPE_DOWNLOAD_FOLDER,
    MSG_TYPE_DOWNLOAD_FILE,
} MessageType;

struct Message_ {
    MessageType type;
    int length;
    char payload[PAYLOAD_SIZE];
};

typedef struct Message_ Message;
/**
 * Hàm tạo thông điệp
 * @param type loại thông điệp
 * @param payload nội dung thông điệp
 * @return thông điệp
 */
Message create_message(MessageType type, char* payload);

/**
 * Tạo thông điệp với trạng thái
 * @param type loại thông điệp
 * @param status trạng thái thông điệp
 * @return thông điệp
 */
Message create_status_message(MessageType type, Status status);

/**
 * Sao chép thông điệp từ temp sang mess
 * @param mess con trỏ tới thông điệp
 * @param temp thông điệp cần sao chép
 * @return 0 nếu thành công, -1 nếu có lỗi
 */
int messsagecpy(Message* mess, Message temp);

void print_message(Message mess);

#endif   // !
