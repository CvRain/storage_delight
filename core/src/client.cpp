//
// Created by cvrain on 24-7-24.
//

#include "client.hpp"

#include <algorithm>
#include <miniocpp/client.h>

namespace storage_delight::core {
    Client::Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider *provider)
            : client(base_url, provider),
              bucket_operation(std::make_shared<BucketOperation>(client)),
              object_operation(std::make_shared<ObjectOperation>(client)) {
        if(!provider){
            throw std::invalid_argument("Provider cannot be null");
        }
    }

    BucketOperation &Client::get_bucket_operation() const
    {
        return *bucket_operation;
    }

    ObjectOperation &Client::get_object_operation() const
    {
        return *object_operation;
    }

    minio::creds::StaticProvider Client::make_provider(const std::string &access_key, const std::string &secret_key) {
        return {access_key, secret_key};
    }

    std::optional<std::shared_ptr<Client>> ClientGroup::get_client(const std::string_view &clientName) {
        std::lock_guard<std::mutex> lock(mutex);
        const auto it = clients.find(clientName.data());
        return it != clients.end() ? std::make_optional(it->second) : std::nullopt;
    }

    std::optional<std::shared_ptr<Client>> ClientGroup::operator[](const std::string_view &clientName) {
        return get_client(clientName);
    }

    void ClientGroup::push_back(std::pair<std::string, std::shared_ptr<Client>> &&client) {
        std::lock_guard<std::mutex> lock(mutex);
        clients.emplace(std::move(client));
    }

    void ClientGroup::push_back(std::string &&clientName, std::shared_ptr<Client> &&client) {
        std::lock_guard<std::mutex> lock(mutex);
        clients.insert(std::make_pair(std::move(clientName), std::move(client)));
    }

    void ClientGroup::clear() {
        std::lock_guard<std::mutex> lock(mutex);
        clients.clear();
    }

    std::size_t ClientGroup::size() const
    {
        return clients.size();
    }

    std::vector<std::string> ClientGroup::get_client_names() {
        std::lock_guard<std::mutex> lock(mutex);
        std::vector<std::string> names;
        names.reserve(clients.size());

        std::ranges::transform(clients, names.begin(),
                               [=](const std::pair<std::string, std::shared_ptr<Client>> &it) {
	                               return it.first;
                               });
        return names;
    }

    void ClientGroup::remove(const std::string_view &clientName) {
        std::lock_guard<std::mutex> lock(mutex);
        clients.erase(clientName.data());
    }
}