//
// Created by cvrain on 24-10-22.
//

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <curl/curl.h>

namespace util_delight {
    class Item {
    public:
        static auto check_minio_server_connection(const std::string_view& url) -> bool {
            CURL* curl = curl_easy_init();
            if (!curl) {
                return false;
            }
            curl_easy_setopt(curl, CURLOPT_URL, url.data());
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            curl_easy_cleanup(curl);

            const CURLcode res = curl_easy_perform(curl);
            return res == CURLE_OK;
        }
    };
} // namespace util_delight

#endif // ITEM_H
