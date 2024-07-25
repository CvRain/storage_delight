//
// Created by cvrain on 24-7-24.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <miniocpp/client.h>
#include <memory>

#include "bucket_operation.hpp"


namespace storage_delight::core {
    class Client {
    public:
        explicit Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider &provider);

        BucketOperation& getBucketOperation();


    private:
        minio::s3::Client client;
        std::unique_ptr<BucketOperation> bucket_operation;
    };
}

#endif //CLIENT_H