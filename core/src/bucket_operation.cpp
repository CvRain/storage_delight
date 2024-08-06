//
// Created by cvrain on 24-7-24.
//

#include "bucket_operation.hpp"

#include <spdlog/spdlog.h>

namespace storage_delight::core {
    BucketOperation::BucketOperation(minio::s3::Client &client)
            : client{client} {

    }

    minio::s3::ListBucketsResponse
    BucketOperation::listBuckets() {
        return execute_operation([&]() {
            return client.ListBuckets();
        }, "List buckets");
    }

    minio::s3::BucketExistsResponse
    BucketOperation::bucket_exists(const std::string_view &bucketName) {
        auto response = execute_operation([&]() {
            minio::s3::BucketExistsArgs args;
            args.bucket = bucketName.data();
            return client.BucketExists(args);
        }, "Bucket exists");
        return response;
    }

    void BucketOperation::delete_bucket_encryption(const std::string &bucketName) {
        if (!bucket_exists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketEncryptionArgs args;
        args.bucket = bucketName;
        execute_operation([this, args]() {
            return client.DeleteBucketEncryption(args);
        }, "Delete bucket encryption");
        log(spdlog::level::info, fmt::format("Bucket {} encryption deleted", bucketName));
    }

    void BucketOperation::delete_bucket_lifecycle(const std::string &bucketName) {
        if (!bucket_exists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketLifecycleArgs args;
        args.bucket = bucketName;
        execute_operation([this, args]() {
            return client.DeleteBucketLifecycle(args);
        }, "Delete bucket lifecycle");
        log(spdlog::level::info, fmt::format("Bucket {} lifecycle deleted", bucketName));
    }

    void BucketOperation::delete_bucket_notifications(const std::string &bucketName) {
        if (!bucket_exists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketNotificationArgs args;
        args.bucket = bucketName;

        auto response = execute_operation([this, args]() {
            return client.DeleteBucketNotification(args);
        }, "Delete bucket notifications");

        if (response) {
            log(spdlog::level::info, fmt::format("Bucket {} notifications deleted", bucketName));
        } else {
            log(spdlog::level::info, fmt::format("Bucket {} notifications delete failed", bucketName));
        }

    }

    void BucketOperation::delete_bucket_policy(const std::string &bucketName) {
        if (!bucket_exists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketPolicyArgs args;
        args.bucket = bucketName;

        auto response = execute_operation([this, args]() {
            return client.DeleteBucketPolicy(args);
        }, "Delete bucket policy");
        if (response) {
            log(spdlog::level::info, fmt::format("Bucket {} policy deleted", bucketName));
        } else {
            log(spdlog::level::info, fmt::format("Bucket {} policy delete failed", bucketName));
        }
    }

    void BucketOperation::delete_bucket_replication(const std::string &bucketName) {
        if (!bucket_exists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketReplicationArgs args;
        args.bucket = bucketName;

        auto response = execute_operation([this, args]() {
            return client.DeleteBucketReplication(args);
        }, "Delete bucket replication");
        if (response) {
            log(spdlog::level::info, fmt::format("Bucket {} replication deleted", bucketName));
        } else {
            log(spdlog::level::info, fmt::format("Bucket {} replication delete failed", bucketName));
        }
    }

    void BucketOperation::delete_bucket_tags(const std::string &bucketName) {
        if (!bucket_exists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketTagsArgs args;
        args.bucket = bucketName;
        execute_operation([this, args]() {
            return client.DeleteBucketTags(args);
        }, "Delete bucket tags");
        log(spdlog::level::info, fmt::format("Bucket {} tags deleted", bucketName));
    }

    std::optional<minio::s3::SseConfig> BucketOperation::get_bucket_encryption(const std::string &bucketName) {
        minio::s3::GetBucketEncryptionArgs args;
        args.bucket = bucketName;

        const auto response = execute_operation([this, args]() {
            return client.GetBucketEncryption(args);
        }, "Get bucket encryption");

        if (response) {
            auto config = response.config;
            log(spdlog::level::info, fmt::format("SSE Algorithm: {}", config.sse_algorithm));
            log(spdlog::level::info, fmt::format("KMS Master Key ID: {}", config.kms_master_key_id));
            return config;
        }
        log(spdlog::level::info, fmt::format("Bucket {} encryption not found", bucketName));
        return std::nullopt;
    }

    std::optional<minio::s3::LifecycleConfig> BucketOperation::get_bucket_lifecycle(const std::string &bucketName) {
        minio::s3::GetBucketLifecycleArgs args;
        args.bucket = bucketName;

        const auto response = execute_operation([this, args]() {
            return client.GetBucketLifecycle(args);
        }, "Get bucket lifecycle");

        if (response) {
            auto lifecycle = response.config;
            log(spdlog::level::info, fmt::format("Lifecycle configuration: {}", lifecycle.ToXML()));
            return lifecycle;
        }
        log(spdlog::level::info,
            fmt::format("Bucket {} lifecycle not found: {}", bucketName, response.Error().String()));
        return std::nullopt;
    }

    std::optional<minio::s3::NotificationConfig>
    BucketOperation::get_bucket_notification(const std::string &bucketName) {
        minio::s3::GetBucketNotificationArgs args;
        args.bucket = bucketName;

        const auto response = execute_operation([this, args]() {
            return client.GetBucketNotification(args);
        }, "Get bucket notification");

        if (response) {
            auto notification = response.config;
            log(spdlog::level::info, fmt::format("Notification configuration: {}", notification.ToXML()));
            return notification;
        }
        log(spdlog::level::info,
            fmt::format("Bucket {} notification not found: {}", bucketName, response.Error().String()));
        return std::nullopt;
    }

    std::optional<std::basic_string<char>> BucketOperation::get_bucket_policy(const std::string &bucketName) {
        minio::s3::GetBucketPolicyArgs args;
        args.bucket = bucketName;

        const auto response = execute_operation([this, args]() {
            return client.GetBucketPolicy(args);
        }, "Get bucket policy");

        if (response) {
            auto policy = response.policy;
            log(spdlog::level::info, fmt::format("Policy: {}", policy));
            return policy;
        }
        log(spdlog::level::info, fmt::format("Bucket {} policy not found: {}", bucketName, response.Error().String()));
        return std::nullopt;
    }

    std::optional<std::string_view> BucketOperation::get_bucket_replication(const std::string &bucketName) {
        const auto response = execute_operation([&]() {
            minio::s3::GetBucketReplicationArgs args;
            args.bucket = bucketName;
            return client.GetBucketReplication(args);
        }, "Get bucket replication");

        if (response) {
            auto replication = response.config.ToXML();
            log(spdlog::level::info, fmt::format("Replication: {}", replication));
            return replication;
        }
        log(spdlog::level::info,
            fmt::format("Bucket {} replication not found: {}", bucketName, response.Error().String()));
        return std::nullopt;
    }

    std::optional<std::map<std::string, std::string>> BucketOperation::get_bucket_tags(const std::string &bucketName) {
        const auto response = execute_operation([&]() {
            minio::s3::GetBucketTagsArgs args;
            args.bucket = bucketName;
            return client.GetBucketTags(args);
        }, "Get bucket tags");

        if (response) {
            auto tags = response.tags;
            for (const auto &[key, value]: tags) {
                log(spdlog::level::info, fmt::format("Tag: {} = {}", key, value));
            }
            return tags;
        }
        log(spdlog::level::info, fmt::format("Bucket {} tags not found: {}", bucketName, response.Error().String()));
        return std::nullopt;
    }

    std::optional<std::string> BucketOperation::get_bucket_versioning(const std::string &bucketName) {
        const auto response = execute_operation([&]() {
            minio::s3::GetBucketVersioningArgs args;
            args.bucket = bucketName;
            return client.GetBucketVersioning(args);
        }, "Get bucket versioning");

        if (response) {
            auto versioning = response.Status();
            log(spdlog::level::info, fmt::format("Versioning: {}", versioning));
            return versioning;
        }
        log(spdlog::level::info,
            fmt::format("Bucket {} versioning not found: {}", bucketName, response.Error().String())
        );
        return std::nullopt;
    }
}