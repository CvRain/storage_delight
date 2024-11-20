//
// Created by cvrain on 24-7-24.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <map>
#include <memory>
#include <miniocpp/client.h>
#include <mutex>
#include <vector>

#include "bucket_operation.hpp"
#include "object_operation.hpp"

namespace storage_delight::core {
    class Client {
    public:
        explicit Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider *provider);

        static minio::creds::StaticProvider make_provider(const std::string &access_key, const std::string &secret_key);

        [[nodiscard]] BucketOperation &get_bucket_operation() const;

        [[nodiscard]] ObjectOperation &get_object_operation() const;


    private:
        minio::s3::Client                client;
        std::shared_ptr<BucketOperation> bucket_operation;
        std::shared_ptr<ObjectOperation> object_operation;
    };

    class ClientGroup {
    public:
        ClientGroup() = default;

        std::optional<std::shared_ptr<Client>> get_client(const std::string_view &clientName);

        std::optional<std::shared_ptr<Client>> operator[](const std::string_view &clientName);

        void push_back(std::pair<std::string, std::shared_ptr<Client>> &&client);

        void push_back(std::string &&clientName, std::shared_ptr<Client> &&client);

        void remove(const std::string_view &clientName);

        void clear();

        std::size_t size() const;

        std::vector<std::string> get_client_names();

    private:
        std::unordered_map<std::string, std::shared_ptr<Client>> clients = {};
        mutable std::mutex                                       mutex;
    };

    template<typename T>
    class ClientGroupV2 {
    public:
             ClientGroupV2() = default;
        auto get_client(const T& key) -> std::optional<std::shared_ptr<Client>>;
        auto operator[](const T& key) -> std::optional<std::shared_ptr<Client>>;
        void push_back(const T& key, std::shared_ptr<Client> client);
        void remove(T key);
        void clear();
        auto is_exist(const T& key) -> bool;
        auto size() const -> size_t;
        ~    ClientGroupV2() = default;

    private:
        std::map<T, std::shared_ptr<Client>> clients = {};
        mutable std::mutex                             mutex;
    };

    template<typename T>
    auto ClientGroupV2<T>::get_client(const T& key) -> std::optional<std::shared_ptr<Client>> {
        std::lock_guard<std::mutex> lock(mutex);
        const auto                  it = clients.find(key);
        return it != clients.end() ? std::make_optional(it->second) : std::nullopt;
    }

    template<typename T>
    auto ClientGroupV2<T>::operator[](const T& key) -> std::optional<std::shared_ptr<Client>> {
        return get_client(key);
    }

    template<typename T>
    void ClientGroupV2<T>::push_back(const T& key, std::shared_ptr<Client> client) {
        std::lock_guard<std::mutex> lock(mutex);
        clients.insert(std::make_pair(key, client));
    }
    template<typename T>
    void ClientGroupV2<T>::remove(T key) {
        std::lock_guard<std::mutex> lock(mutex);
        clients.erase(key);
    }
    template<typename T>
    void ClientGroupV2<T>::clear() {
        std::lock_guard<std::mutex> lock(mutex);
        clients.clear();
    }
    template<typename T>
    auto ClientGroupV2<T>::is_exist(const T& key) -> bool {
        return clients.contains(key);
    }

    template<typename T>
    std::size_t ClientGroupV2<T>::size() const {
        return clients.size();
    }
}  // namespace storage_delight::core

#endif  // CLIENT_H
