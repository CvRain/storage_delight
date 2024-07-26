//
// Created by cvrain on 24-7-24.
//

#include "bucket_operation.hpp"

#include <spdlog/spdlog.h>

namespace storage_delight::core {

    template<typename Func>
    void responseCallback(minio::s3::Response *response, Func func) {

    }

    BucketOperation::BucketOperation(minio::s3::Client &client)
            : client{client} {

    }

    std::optional<std::list<minio::s3::Bucket>> BucketOperation::listBuckets() {
        auto result = executeOperation([this]() {
            return client.ListBuckets();
        }, "List buckets");

        if (result) {
            for (const auto &bucket: result.buckets) {
                log(spdlog::level::info, "Bucket: " + bucket.name);
            }
            return result.buckets;
        }
        return std::nullopt;
    }

    bool BucketOperation::bucketExists(const std::string &bucketName) {
        minio::s3::BucketExistsArgs args;
        args.bucket = bucketName;

        auto result = executeOperation([this, args]() {
            return client.BucketExists(args);
        }, "Bucket exists");

        if (result) {
            log(spdlog::level::info,
                fmt::format("Bucket {} {}", bucketName, result.exist ? "exists" : "does not exist"));
            return result.exist;
        }
        return false;
    }

    void BucketOperation::deleteBucketEncryption(const std::string &bucketName) {
        if (!bucketExists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketEncryptionArgs args;
        args.bucket = bucketName;
        executeOperation([this, args]() {
            return client.DeleteBucketEncryption(args);
        }, "Delete bucket encryption");
        log(spdlog::level::info, fmt::format("Bucket {} encryption deleted", bucketName));
    }

    void BucketOperation::deleteBucketLifecycle(const std::string &bucketName) {
        if (!bucketExists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketLifecycleArgs args;
        args.bucket = bucketName;
        executeOperation([this, args]() {
            return client.DeleteBucketLifecycle(args);
        }, "Delete bucket lifecycle");
        log(spdlog::level::info, fmt::format("Bucket {} lifecycle deleted", bucketName));
    }

    void BucketOperation::deleteBucketNotifications(const std::string &bucketName) {
        if (!bucketExists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketNotificationArgs args;
        args.bucket = bucketName;

        auto response = executeOperation([this, args]() {
            return client.DeleteBucketNotification(args);
        }, "Delete bucket notifications");

        if (response) {
            log(spdlog::level::info, fmt::format("Bucket {} notifications deleted", bucketName));
        } else {
            log(spdlog::level::info, fmt::format("Bucket {} notifications delete failed", bucketName));
        }

    }

    void BucketOperation::deleteBucketPolicy(const std::string &bucketName) {
        if (!bucketExists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketPolicyArgs args;
        args.bucket = bucketName;

        auto response = executeOperation([this, args]() {
            return client.DeleteBucketPolicy(args);
        }, "Delete bucket policy");
        if (response) {
            log(spdlog::level::info, fmt::format("Bucket {} policy deleted", bucketName));
        } else {
            log(spdlog::level::info, fmt::format("Bucket {} policy delete failed", bucketName));
        }
    }

    void BucketOperation::deleteBucketReplication(const std::string &bucketName) {
        if (!bucketExists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketReplicationArgs args;
        args.bucket = bucketName;

        auto response = executeOperation([this, args]() {
            return client.DeleteBucketReplication(args);
        }, "Delete bucket replication");
        if (response) {
            log(spdlog::level::info, fmt::format("Bucket {} replication deleted", bucketName));
        } else {
            log(spdlog::level::info, fmt::format("Bucket {} replication delete failed", bucketName));
        }
    }

    void BucketOperation::deleteBucketTags(const std::string &bucketName) {
        if (!bucketExists(bucketName)) {
            log(spdlog::level::info, fmt::format("Bucket {} does not exist", bucketName));
            return;
        }

        minio::s3::DeleteBucketTagsArgs args;
        args.bucket = bucketName;
        executeOperation([this, args]() {
            return client.DeleteBucketTags(args);
        }, "Delete bucket tags");
        log(spdlog::level::info, fmt::format("Bucket {} tags deleted", bucketName));
    }

    std::optional<minio::s3::SseConfig> BucketOperation::getBucketEncryption(const std::string &bucketName) {
        minio::s3::GetBucketEncryptionArgs args;
        args.bucket = bucketName;

        const auto response = executeOperation([this, args]() {
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

    std::optional<minio::s3::LifecycleConfig> BucketOperation::getBucketLiftCycle(const std::string &bucketName) {
        minio::s3::GetBucketLifecycleArgs args;
        args.bucket = bucketName;

        const auto response = executeOperation([this, args]() {
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

    std::optional<minio::s3::NotificationConfig> BucketOperation::getBucketNotification(const std::string &bucketName) {
        minio::s3::GetBucketNotificationArgs args;
        args.bucket = bucketName;

        const auto response = executeOperation([this, args]() {
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

    std::optional<std::basic_string<char>> BucketOperation::getBucketPolicy(const std::string &bucketName) {
        minio::s3::GetBucketPolicyArgs args;
        args.bucket = bucketName;

        const auto response = executeOperation([this, args]() {
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

    std::optional<std::string_view> BucketOperation::getBucketReplication(const std::string &bucketName) {
        const auto response = executeOperation([&]() {
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

    std::optional<std::map<std::string, std::string>> BucketOperation::getBucketTags(const std::string &bucketName) {
        const auto response = executeOperation([&]() {
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

    std::optional<std::string> BucketOperation::getBucketVersioning(const std::string &bucketName) {
        const auto response = executeOperation([&]() {
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