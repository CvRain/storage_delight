//
// Created by cvrain on 24-7-24.
//

#include "client.hpp"

#include <miniocpp/client.h>
#include <spdlog/spdlog.h>

namespace storage_delight::core {
    Client::Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider &provider)
            : client(base_url, &provider),
              bucket_operation(std::make_unique<BucketOperation>(client)) {
    }

    BucketOperation &Client::getBucketOperation() {
        return *bucket_operation;
    }
}