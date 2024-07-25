//
// Created by cvrain on 24-7-24.
//

#ifndef STORAGE_DELIGHT_BUCKET_OPERATION_HPP
#define STORAGE_DELIGHT_BUCKET_OPERATION_HPP

#include "base_operation.hpp"
#include <miniocpp/client.h>
#include <list>
#include <optional>

namespace storage_delight::core {
    class BucketOperation : public BaseOperation {
    public:
        explicit BucketOperation(minio::s3::Client &client);

        bool bucketExists(const std::string &bucketName);

        std::optional<std::list<minio::s3::Bucket>> listBuckets();

        void deleteBucketEncryption(const std::string &bucketName);

        void deleteBucketLifecycle(const std::string &bucketName);

        void deleteBucketNotifications(const std::string &bucketName);

        void deleteBucketPolicy(const std::string &bucketName);

        void deleteBucketReplication(const std::string &bucketName);

        void deleteBucketTags(const std::string &bucketName);
    private:
        minio::s3::Client &client;
    };
}

#endif //STORAGE_DELIGHT_BUCKET_OPERATION_HPP
