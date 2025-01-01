#include "status.h"

#include <string>

std::string status_str(Status code) {
    switch (code) {
        case USERNAME_EXIST:
            return "Tên người dùng đã tồn tại!";
            break;
        // thông báo thành công
        case USER_NOT_FOUND:
            return "Không tìm thấy người dùng!";
            break;
        case USER_IS_BLOCKED:
            return "Tài khoản bị khóa!";
            break;
        case BLOCKED_USER:
            return "Người dùng đã bị khóa!";
            break;
        case PASSWORD_INVALID:
            return "Mật khẩu nhập vào không hợp lệ!";
            break;
            
        // thông báo lỗi
        case LOGIN_SUCCESS:
            return "Đăng nhập thành công!";
            break;

        // thông báo lỗi
        case LOGIN_FAIL:
            return "Tên người dùng hoặc mật khẩu sai, hoặc tài khoản của bạn đã bị khóa!";
            break;
        case USER_IS_ONLINE:
            return "Người dùng đang trực tuyến!";
            break;
        case ACCOUNT_IS_EXIST:
            return "Tài khoản đã tồn tại!";
            break;

        // thông báo không hợp lệ
        case REGISTER_SUCCESS:
            return "Đăng ký thành công!";
            break;
        case LOGOUT_SUCCESS:
            return "Đăng xuất thành công!";
            break;
        case FILE_NOT_FOUND:
            return "Không tìm thấy tệp!";
            break;
        case COMMAND_INVALID:
            return "Lệnh không hợp lệ!";
            break;
        case USERNAME_OR_PASSWORD_INVALID:
            return "Tên người dùng hoặc mật khẩu không hợp lệ!";
            break;

        // thông báo lỗi máy chủ
        case SERVER_ERROR:
            return "Có lỗi xảy ra!";
            break;
        case DIRECTORY_NOT_FOUND:
            return "cd: Không tìm thấy thư mục!";
            break;
        case STATUS_MV_ERROR:
            return "Cú pháp: mv <đường_dẫn_cũ> <đường_dẫn_mới>";
            break;
        case STATUS_SHARECMD_ERROR:
            return "Cú pháp: share <chế_độ> -u <tên_người_dùng> -f <tên_tệp>";
            break;
        case STATUS_MESSAGE_TOO_LARGE:
            return "Tin nhắn gửi hoặc nhận quá lớn! Có thể do mã hóa!";
            break;
        case NO:
            return "";
            break;
        default:
            break;
    }
    return "";
}
