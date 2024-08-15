//
// Created by cvrain on 24-8-13.
//

#ifndef STORAGE_DELIGHT_STRING_ENCRYPTION_H
#define STORAGE_DELIGHT_STRING_ENCRYPTION_H

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <iomanip>

namespace util_delight
{
	template <typename Num>
	concept is_integral = std::is_integral_v<Num>
		|| std::is_same_v<Num, uint8_t>
		|| std::is_same_v<Num, uint16_t>
		|| std::is_same_v<Num, uint32_t>
		|| std::is_same_v<Num, uint64_t>
		|| std::is_same_v<Num, int8_t>
		|| std::is_same_v<Num, int16_t>
		|| std::is_same_v<Num, int32_t>;

	class StringEncryption {
	public:
		template <is_integral T>
		static T to_number(const std::string_view& str) {
			std::istringstream iss(str.data());
			T num;
			iss >> num;
			if (iss.fail()) {
				throw std::invalid_argument("Invalid number format");
			}
			return num;
		}

		static std::string sha256(const std::string& str) {
			unsigned char hash[SHA256_DIGEST_LENGTH];
			EVP_MD_CTX* ctx = EVP_MD_CTX_new();
			EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
			EVP_DigestUpdate(ctx, str.c_str(), str.size());
			unsigned int len;
			EVP_DigestFinal_ex(ctx, hash, &len);
			EVP_MD_CTX_free(ctx);

			std::stringstream ss;
			for (const unsigned char i : hash) {
				ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
			}

			return ss.str();
		}

		static std::string sha512(const std::string& str) {
			unsigned char hash[SHA512_DIGEST_LENGTH];
			EVP_MD_CTX* ctx = EVP_MD_CTX_new();
			EVP_DigestInit_ex(ctx, EVP_sha512(), nullptr);
			EVP_DigestUpdate(ctx, str.c_str(), str.size());
			unsigned int len;
			EVP_DigestFinal_ex(ctx, hash, &len);
			EVP_MD_CTX_free(ctx);
			std::stringstream ss;
			for (const unsigned char i : hash) {
				ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
			}
			return ss.str();
		}
	};
}

#endif //STORAGE_DELIGHT_STRING_ENCRYPTION_H
