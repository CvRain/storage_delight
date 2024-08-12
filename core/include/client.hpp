//
// Created by cvrain on 24-7-24.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <miniocpp/client.h>
#include <memory>
#include <vector>
#include <map>

#include "bucket_operation.hpp"
#include "object_operation.hpp"

namespace storage_delight::core {
    class Client {
    public:
        explicit Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider *provider);

        static minio::creds::StaticProvider make_provider(const std::string &access_key, const std::string &secret_key);

        BucketOperation &getBucketOperation();

        ObjectOperation &getObjectOperation();


    private:
        minio::s3::Client client;
        std::unique_ptr<BucketOperation> bucket_operation;
        std::unique_ptr<ObjectOperation> object_operation;
    };

    class ClientGroup {
    public:
        ClientGroup() = default;

        std::optional<std::shared_ptr<Client>> get_client(const std::string_view &clientName);

        std::optional<std::shared_ptr<Client>> operator[](const std::string_view &clientName);

        void push_back(std::pair<std::string, std::shared_ptr<Client>>&& client);

        void push_back(std::string &&clientName, std::shared_ptr<Client> &&client);

        void remove(const std::string_view& clientName);

        void clear();

        std::size_t size();

        std::vector<std::string> get_client_names();

    private:
        std::unordered_map<std::string, std::shared_ptr<Client>> clients = {};
    };
}

#endif //CLIENT_H