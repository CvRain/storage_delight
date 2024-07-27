//
// Created by cvrain on 24-7-24.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <miniocpp/client.h>
#include <memory>

#include "bucket_operation.hpp"
#include "object_operation.hpp"

namespace storage_delight::core {
    class Client {
    public:
        explicit Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider &provider);

        BucketOperation& getBucketOperation();
        ObjectOperation& getObjectOperation();


    private:
        minio::s3::Client client;
        std::unique_ptr<BucketOperation> bucket_operation;
        std::unique_ptr<ObjectOperation> object_operation;
    };
}

#endif //CLIENT_H