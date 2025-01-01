#ifndef __CYPHER_H__
#define __CYPHER_H__

#include <string>
#include <vector>

#define SYMMETRIC_KEY_SIZE 16

/**
 * Tạo cặp khóa RSA
 */
bool generate_key_pair(std::string &public_key, std::string &private_key);

/**
 * Tạo khóa đối xứng
 */
bool generate_symmetric_key(std::string &key);

/**
 * Mã hóa dữ liệu bằng khóa đối xứng
 * @param key khóa đối xứng
 * @param plaintext văn bản gốc
 * @param ciphertext văn bản đã mã hóa
 * @return độ dài của văn bản đã mã hóa nếu thành công, -1 nếu có lỗi
 */
int encrypt_data(const std::string &aesKey, const std::string &plaintext, std::string &ciphertext);

/**
 * Giải mã dữ liệu bằng khóa đối xứng
 * @param key khóa đối xứng
 * @param ciphertext văn bản đã mã hóa
 * @param plaintext văn bản gốc
 * @return độ dài của văn bản gốc nếu thành công, -1 nếu có lỗi
 */
int decrypt_data(const std::string &aesKey, const std::string &ciphertext, std::string &plaintext);

/**
 * Mã hóa khóa đối xứng bằng khóa công RSA
 * @param publicKeyPEM khóa công RSA
 * @param plaintext khóa đối xứng
 * @param encrypted khóa đối xứng đã mã hóa
 * @return true nếu thành công, false nếu có lỗi
 */
bool encrypt_symmetric_key(const std::string &publicKeyPEM, const std::string &plaintext,
                           std::string &encrypted);

/**
 * Giải mã khóa đối xứng bằng khóa riêng RSA
 * @param privateKeyPEM khóa riêng RSA
 * @param encrypted khóa đối xứng đã mã hóa
 * @param decrypted khóa đối xứng đã giải mã
 * @return true nếu thành công, false nếu có lỗi
 */
bool decrypt_symmetric_key(const std::string &privateKeyPEM, const std::string &encrypted,
                           std::string &decrypted);

#endif   // !__CYPHER_H__
