#ifndef __LOG_H__
#define __LOG_H__

/**
 * Ghi thông điệp vào tệp nhật ký
 * @param level mức độ nhật ký [i]nfo, [w]arning, [e]rror
 * @param message thông điệp nhật ký
 */
void log_message(char level, const char* message);

/**
 * Ghi thông điệp vào tệp nhật ký cho server
 * @param level mức độ nhật ký [i]nfo, [w]arning, [e]rror
 * @param message thông điệp nhật ký
 */
void server_log(char level, const char* message);

#endif   // !__LOG_H__