//
// Created by cvrain on 24-7-24.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <miniocpp/client.h>
#include <list>
#include <optional>
#include <spdlog/spdlog.h>

namespace storage_delight::core {
    class Client {
    public:
        explicit Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider &provider);

        std::optional<std::list<minio::s3::Bucket>> listBuckets();

        void setLogEnable(bool enable = false);

    protected:
        template<typename Func>
        auto logDecorator(Func f, const std::string_view &funcName) ;

    private:
        minio::s3::Client client;
        bool isEnableLog = false;
    };
}

#endif //CLIENT_H