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

        minio::s3::BucketExistsResponse
        bucket_exists(const std::string_view &bucketName);

        minio::s3::ListBucketsResponse
        listBuckets();

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

        minio::s3::ListenBucketNotificationResponse
        listen_bucket_notification(const std::string &bucketName);

        minio::s3::MakeBucketResponse
        make_bucket(const std::string &bucketName);

        minio::s3::RemoveBucketResponse
        remove_bucket(const std::string &bucketName);

        minio::s3::SetBucketEncryptionResponse
        set_bucket_encryption(const std::string &bucketName, const minio::s3::SseConfig &sseConfig);

        minio::s3::SetBucketLifecycleResponse
        set_bucket_lifecycle(const std::string &bucketName,
                             minio::s3::LifecycleConfig lifecycleConfig,
                             std::vector<minio::s3::LifecycleRule> rules = {});

        minio::s3::SetBucketNotificationResponse
        set_bucket_notification(const std::string &bucketName,
                                minio::s3::NotificationConfig config,
                                std::vector<minio::s3::QueueConfig> queueConfigs = {});

        minio::s3::SetBucketPolicyResponse
        set_bucket_policy(const std::string &bucketName, const std::string_view &policy);

        minio::s3::SetBucketReplicationResponse
        set_bucket_replication(const std::string &bucketName,
                               const minio::s3::ReplicationConfig &replicationConfig,
                               std::vector<minio::s3::ReplicationRule> rules = {});

        minio::s3::SetBucketTagsResponse
        set_bucket_tags(const std::string &bucketName,
                        const std::map<std::string, std::string> &tags);

        minio::s3::SetBucketVersioningResponse
        set_bucket_versioning(const std::string &bucketName, bool status = true);

    private:
        minio::s3::Client &client;
    };
} // namespace storage_delight::core

#endif // STORAGE_DELIGHT_BUCKET_OPERATION_HPP
