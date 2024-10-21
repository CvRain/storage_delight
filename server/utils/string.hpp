//
// Created by cvrain on 24-8-13.
//

#ifndef STORAGE_DELIGHT_STRING_ENCRYPTION_H
#define STORAGE_DELIGHT_STRING_ENCRYPTION_H

#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <chrono>
#include <random>
#include <nlohmann/json.hpp>

#include "drogon_specialization.hpp"
#include "models/type.hpp"

namespace util_delight {
    template<typename Num>
    concept is_integral = std::is_integral_v<Num>
                          || std::is_same_v<Num, uint8_t>
                          || std::is_same_v<Num, uint16_t>
                          || std::is_same_v<Num, uint32_t>
                          || std::is_same_v<Num, uint64_t>
                          || std::is_same_v<Num, int8_t>
                          || std::is_same_v<Num, int16_t>
                          || std::is_same_v<Num, int32_t>;

    // Ensure OpenSSL is initialized.
    static void ensure_openssl_initialized()
    {
        static bool initialized = false;
        if (!initialized) {
            OPENSSL_init_crypto(0, nullptr);
            initialized = true;
        }
    }

    class StringEncryption {
    public:
        template<is_integral T>
        static T to_number(const std::string_view &str) {
            std::istringstream iss(str.data());
            T num;
            iss >> num;
            if (iss.fail()) {
                throw std::invalid_argument("Invalid number format");
            }
            return num;
        }

        static std::string sha256(const std::string &str);

        static std::string sha512(const std::string &str);

        static std::string base64_encode(const std::string &in);

        static std::string base64_decode(const std::string &in);

        static std::string hmac_sha256(const std::string &data, const std::string &key) ;

        static std::string generate_jwt(const std::string &header, const std::string &payload, const std::string &secret);

        static std::string generate_random_string(size_t length);

        static std::string generate_secret();

        static std::optional<schema::Jwt> parse_jwt(const std::string& jwt, const std::string &secret);

        static std::string secret_string;
    };
}

#endif //STORAGE_DELIGHT_STRING_ENCRYPTION_H
