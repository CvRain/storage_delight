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

        minio::s3::Response
        CopyObject(const std::string_view &buckName, const std::string_view &objectName,
                   const std::string_view &distBuck, const std::string_view &distObject);

        minio::s3::GetObjectResponse
        getObject(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::GetObjectLockConfigResponse
        getObjectLockConfig(const std::string_view &bucketName);

        minio::s3::GetObjectResponse
        getObjectProgress(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::GetObjectRetentionResponse
        getObjectRetention(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::GetObjectTagsResponse
        getObjectTags(const std::string_view &bucketName, const std::string_view &objectName);

    private:
        minio::s3::Client &client;
    };
}

#endif //STORAGE_DELIGHT_OBJECT_OPERATION_HPP
