//
// Created by cvrain on 24-7-24.
//

#include "client.hpp"

#include <miniocpp/client.h>

namespace storage_delight::core {
    Client::Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider *provider)
            : client(base_url, provider),
              bucket_operation(std::make_unique<BucketOperation>(client)) {
    }

    BucketOperation &Client::getBucketOperation() {
        return *bucket_operation;
    }

    ObjectOperation &Client::getObjectOperation() {
        return *object_operation;
    }

    minio::creds::StaticProvider Client::make_provider(const std::string &access_key, const std::string &secret_key) {
        return {access_key, secret_key};
    }

    std::optional<std::shared_ptr<Client>> ClientGroup::get_client(const std::string_view &clientName) {
        if (const auto result = clients.find(clientName.data()); result == clients.end()) {
            return std::nullopt;
        }
        return clients.at(clientName.data());
    }

    std::optional<std::shared_ptr<Client>> ClientGroup::operator[](const std::string_view &clientName) {
        return get_client(clientName);
    }

    void ClientGroup::push_back(std::pair<std::string, std::shared_ptr<Client>>&& client) {
        clients.emplace(std::move(client));
    }

    void ClientGroup::push_back(std::string &&clientName, std::shared_ptr<Client> &&client) {
        clients.insert(std::make_pair(std::move(clientName), std::move(client)));
    }

    void ClientGroup::clear() {
        clients.clear();
    }

    std::size_t ClientGroup::size() {
        return clients.size();
    }

    std::vector<std::string> ClientGroup::get_client_names() {
        std::vector<std::string> names;
        names.reserve(clients.size());

        std::transform(clients.begin(), clients.end(), names.begin(),
                       [=](const std::pair<std::string, std::shared_ptr<Client>> &it) {
                           return it.first;
                       });
        return names;
    }

    void ClientGroup::remove(const std::string_view &clientName) {
        clients.erase(clientName.data());
    }
}