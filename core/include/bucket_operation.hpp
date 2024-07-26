//
// Created by cvrain on 24-7-24.
//

#ifndef STORAGE_DELIGHT_BUCKET_OPERATION_HPP
#define STORAGE_DELIGHT_BUCKET_OPERATION_HPP

#include "base_operation.hpp"
#include <miniocpp/client.h>
#include <list>
#include <optional>
#include <map>

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

        std::optional<minio::s3::SseConfig> getBucketEncryption(const std::string &bucketName);

        std::optional<minio::s3::LifecycleConfig> getBucketLiftCycle(const std::string &bucketName);

        std::optional<minio::s3::NotificationConfig> getBucketNotification(const std::string &bucketName);

        std::optional<std::basic_string<char>> getBucketPolicy(const std::string &bucketName);

        std::optional<std::string_view> getBucketReplication(const std::string &bucketName);

        std::optional<std::map<std::string , std::string>> getBucketTags(const std::string &bucketName);

        std::optional<std::string> getBucketVersioning(const std::string &bucketName);
    private:
        minio::s3::Client &client;
    };
}

#endif //STORAGE_DELIGHT_BUCKET_OPERATION_HPP
