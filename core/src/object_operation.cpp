//
// Created by cvrain on 24-7-26.
//

#include "object_operation.hpp"

namespace storage_delight::core {

    ObjectOperation::ObjectOperation(minio::s3::Client &client)
            : client(client) {

    }

    std::optional<minio::s3::Response>
    ObjectOperation::getObject(const std::string_view &bucketName, const std::string_view &objectName) {
        auto response = executeOperation([&]() {
            minio::s3::GetObjectArgs args;
            args.bucket = bucketName;
            args.object = objectName;
            args.datafunc = [](const minio::http::DataFunctionArgs &args) {
                //do something
                return true;
            };

            return client.GetObject(args);
        }, "getObject");

        if (response) {
            log(spdlog::level::info, "getObject success");
            return response;
        }
        return std::nullopt;
    }

    std::optional<const minio::s3::ObjectLockConfig>
    ObjectOperation::getObjectLockConfig(const std::string_view &bucketName) {
        auto response = executeOperation([&]() {
            minio::s3::GetObjectLockConfigArgs args;
            args.bucket = bucketName;
            return client.GetObjectLockConfig(args);
        }, "getObjectLockConfig");

        if (!response) {
            log(spdlog::level::info,
                fmt::format("unable to get object lock configuration {} ", response.Error().String()));
            return std::nullopt;
        }

        log(spdlog::level::info, fmt::format("get object lock configuration success"));

        const auto &config = response.config;
        if (minio::s3::IsRetentionModeValid(config.retention_mode)) {
            log(spdlog::level::info,
                fmt::format("Retention Mode: {}", minio::s3::RetentionModeToString(config.retention_mode)));

            if (config.retention_duration_days) {
                log(spdlog::level::info,
                    fmt::format("Retention Days: {}", config.retention_duration_days.Get()));
            }

            if (config.retention_duration_years) {
                log(spdlog::level::info,
                    fmt::format("Retention Years: {}", config.retention_duration_years.Get()));
            }
        }
        return config;
    }

    std::optional<minio::s3::Response>
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

        minio::s3::GetObjectResponse response = client.GetObject(args);
        if (response) {
            log(spdlog::level::info, "getObjectProgress success");
            return response;
        }
        log(spdlog::level::info, fmt::format("getObjectProgress failed: {}", response.Error().String()));
        return std::nullopt;
    }

    std::optional<minio::s3::GetObjectRetentionResponse>
    ObjectOperation::getObjectRetention(const std::string_view &bucketName, const std::string_view &objectName) {
        minio::s3::GetObjectRetentionArgs args;
        args.bucket = bucketName;
        args.object = objectName;

        auto response = client.GetObjectRetention(args);
        if (!response) {
            log(spdlog::level::info, response.Error().String());
            return std::nullopt;
        }
        log(spdlog::level::info, minio::s3::RetentionModeToString(response.retention_mode));
        log(spdlog::level::info, response.retain_until_date.ToHttpHeaderValue());
        return response;
    }

    std::optional<minio::s3::GetObjectTagsResponse>
    ObjectOperation::getObjectTags(const std::string_view &bucketName, const std::string_view &objectName) {
        auto response = executeOperation([&]() {
            minio::s3::GetObjectTagsArgs args;
            args.bucket = bucketName;
            args.object = objectName;
            return client.GetObjectTags(args);
        }, "getObjectTags");

        if (!response) {
            log(spdlog::level::info, fmt::format("unable to get object tags {} ", response.Error().String()));
            return std::nullopt;
        }
        log(spdlog::level::info, fmt::format("object tags"));
        for (const auto &[key, value]: response.tags) {
            log(spdlog::level::info, fmt::format("key: {}, value: {}", key, value));
        }
        return response;
    }
}