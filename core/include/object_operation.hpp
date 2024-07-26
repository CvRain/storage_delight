//
// Created by cvrain on 24-7-26.
//

#ifndef STORAGE_DELIGHT_OBJECT_OPERATION_HPP
#define STORAGE_DELIGHT_OBJECT_OPERATION_HPP

#include "base_operation.hpp"
#include <miniocpp/client.h>
#include <optional>
#include <string>
#include <string_view>

namespace storage_delight::core {
    class ObjectOperation : public BaseOperation {
    public:
        explicit ObjectOperation(minio::s3::Client &client);

        std::optional<minio::s3::Response>
        getObject(const std::string_view &bucketName, const std::string_view &objectName);

        std::optional<const minio::s3::ObjectLockConfig> getObjectLockConfig(const std::string_view &bucketName);

        std::optional<minio::s3::Response>
        getObjectProgress(const std::string_view &bucketName, const std::string_view &objectName);

        std::optional<minio::s3::GetObjectRetentionResponse>
        getObjectRetention(const std::string_view &bucketName, const std::string_view &objectName);

        std::optional<minio::s3::GetObjectTagsResponse>
        getObjectTags(const std::string_view &bucketName, const std::string_view &objectName);

    private:
        minio::s3::Client &client;
    };
}

#endif //STORAGE_DELIGHT_OBJECT_OPERATION_HPP
