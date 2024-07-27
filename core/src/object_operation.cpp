//
// Created by cvrain on 24-7-26.
//

#include "object_operation.hpp"

namespace storage_delight::core {

    ObjectOperation::ObjectOperation(minio::s3::Client &client)
            : client(client) {

    }

    minio::s3::Response
    ObjectOperation::CopyObject(const std::string_view &buckName, const std::string_view &objectName,
                                const std::string_view &distBuck, const std::string_view &distObject) {
        return executeOperation([&]() {
            minio::s3::CopyObjectArgs args;
            args.bucket = buckName;
            args.object = objectName;

            minio::s3::CopySource source;
            source.bucket = distBuck;
            source.object = distObject;
            args.source = source;
            return client.CopyObject(args);
        }, "CopyObject");
    }

    minio::s3::GetObjectResponse
    ObjectOperation::getObject(const std::string_view &bucketName, const std::string_view &objectName) {
        return executeOperation([&]() {
            minio::s3::GetObjectArgs args;
            args.bucket = bucketName;
            args.object = objectName;
            args.datafunc = [](const minio::http::DataFunctionArgs &args) {
                //do something
                return true;
            };

            return client.GetObject(args);
        }, "getObject");
    }

    minio::s3::GetObjectLockConfigResponse
    ObjectOperation::getObjectLockConfig(const std::string_view &bucketName) {
        return executeOperation([&]() {
            minio::s3::GetObjectLockConfigArgs args;
            args.bucket = bucketName;
            return client.GetObjectLockConfig(args);
        }, "getObjectLockConfig");
    }

    minio::s3::GetObjectResponse
    ObjectOperation::getObjectProgress(const std::string_view &bucketName, const std::string_view &objectName) {
        minio::s3::GetObjectArgs args;
        args.bucket = bucketName;
        args.object = objectName;
        args.datafunc = [&](auto dataArgs) {
            log(spdlog::level::info, fmt::format("receive data: {} bytes", dataArgs.datachunk.length()));
            return true;
        };

        args.progressfunc = [&](auto progressArgs) {
            if (progressArgs.download_speed > 0) {
                log(spdlog::level::info, fmt::format("download speed: {} bytes/s", progressArgs.download_speed));
            } else {
                log(spdlog::level::info, fmt::format("bytes of {} bytes", progressArgs.download_total_bytes));
            }
            return true;
        };

        return client.GetObject(args);
    }

    minio::s3::GetObjectRetentionResponse
    ObjectOperation::getObjectRetention(const std::string_view &bucketName, const std::string_view &objectName) {
        return executeOperation([&]() {
            minio::s3::GetObjectRetentionArgs args;
            args.bucket = bucketName;
            args.object = objectName;
            return client.GetObjectRetention(args);
        }, "getObjectRetention");
    }

    minio::s3::GetObjectTagsResponse
    ObjectOperation::getObjectTags(const std::string_view &bucketName, const std::string_view &objectName) {
        return executeOperation([&]() {
            minio::s3::GetObjectTagsArgs args;
            args.bucket = bucketName;
            args.object = objectName;
            return client.GetObjectTags(args);
        }, "getObjectTags");
    }
}