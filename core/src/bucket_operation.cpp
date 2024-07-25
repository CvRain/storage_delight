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
}