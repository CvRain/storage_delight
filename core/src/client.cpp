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
}