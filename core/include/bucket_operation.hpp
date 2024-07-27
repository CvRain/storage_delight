//
// Created by cvrain on 24-7-24.
//

#ifndef STORAGE_DELIGHT_BUCKET_OPERATION_HPP
#define STORAGE_DELIGHT_BUCKET_OPERATION_HPP

#include "base_operation.hpp"
#include <list>
#include <map>
#include <miniocpp/client.h>
#include <optional>

namespace storage_delight::core {
    class BucketOperation : public BaseOperation {
    public:
        explicit BucketOperation(minio::s3::Client &client);

        bool bucket_exists(const std::string &bucketName);

        std::optional<std::list<minio::s3::Bucket>> listBuckets();

        void delete_bucket_encryption(const std::string &bucketName);

        void delete_bucket_lifecycle(const std::string &bucketName);

        void delete_bucket_notifications(const std::string &bucketName);

        void delete_bucket_policy(const std::string &bucketName);

        void delete_bucket_replication(const std::string &bucketName);

        void delete_bucket_tags(const std::string &bucketName);

        std::optional<minio::s3::SseConfig>
        get_bucket_encryption(const std::string &bucketName);

        std::optional<minio::s3::LifecycleConfig>
        get_bucket_lifecycle(const std::string &bucketName);

        std::optional<minio::s3::NotificationConfig>
        get_bucket_notification(const std::string &bucketName);

        std::optional<std::basic_string<char>>
        get_bucket_policy(const std::string &bucketName);

        std::optional<std::string_view>
        get_bucket_replication(const std::string &bucketName);

        std::optional<std::map<std::string, std::string>>
        get_bucket_tags(const std::string &bucketName);

        std::optional<std::string>
        get_bucket_versioning(const std::string &bucketName);

    private:
        minio::s3::Client &client;
    };
} // namespace storage_delight::core

#endif // STORAGE_DELIGHT_BUCKET_OPERATION_HPP
