//
// Created by cvrain on 24-7-26.
//

#ifndef STORAGE_DELIGHT_OBJECT_OPERATION_HPP
#define STORAGE_DELIGHT_OBJECT_OPERATION_HPP

#include "base_operation.hpp"
#include "basic_types.hpp"

#include <miniocpp/client.h>
#include <optional>
#include <string>
#include <string_view>
#include <list>
#include <initializer_list>
#include <functional>

namespace storage_delight::core {
    class ObjectOperation : public BaseOperation {
    public:
        explicit ObjectOperation(minio::s3::Client &client);

        template<typename T, typename... Args>
        static std::list<T> make_list(T first, Args... args) {
            return {first, args...};
        }

        template<typename T>
        static std::list<T> make_list_from_var_args(int count, ...) {
            va_list args;
            va_start(args, count);
            std::list<T> lst;
            for (int i = 0; i < count; ++i) {
                lst.push_back(va_arg(args, T));
            }
            va_end(args);
            return lst;
        }

        minio::s3::ComposeObjectResponse
        compose_object(const minio::s3::ComposeObjectArgs &composeObjectArgs,
                       const std::list<minio::s3::ComposeSource> &sources);

        minio::s3::ComposeObjectResponse
        compose_object(const minio::s3::ComposeObjectArgs &composeObjectArgs,
                       const std::initializer_list<minio::s3::ComposeSource> &sources);

        minio::s3::Response
        copy_object(const std::string_view &buckName, const std::string_view &objectName,
                    const std::string_view &distBuck, const std::string_view &distObject);

        minio::s3::Response
        copy_object(const minio::s3::CopyObjectArgs &copyArgs, const minio::s3::CopySource &source);

        minio::s3::GetObjectResponse
        get_object(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::GetObjectLockConfigResponse
        get_object_config(const std::string_view &bucketName);

        minio::s3::GetObjectResponse
        get_object_progress(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::GetObjectRetentionResponse
        get_object_retention(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::GetObjectTagsResponse
        get_object_tags(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::DeleteObjectLockConfigResponse
        delete_object_config(const std::string_view &bucketName);

        minio::s3::DisableObjectLegalHoldResponse
        disable_object_legal_hold(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::DownloadObjectResponse
        download_object(const minio::s3::DownloadObjectArgs &args);

        minio::s3::DownloadObjectResponse
        download_object(const std::string_view &bucketName, const std::string_view &objectName,
                        const std::string_view &fileName);

        minio::s3::EnableObjectLegalHoldResponse
        enable_object_legal_hold(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::ListObjectsResult
        list_objects(const std::string_view &bucketName);

        minio::s3::GetPresignedObjectUrlResponse
        get_presigned_object_url(const std::string_view &bucketName, const std::string_view &objectName);

        //todo
//        minio::s3::GetPresignedPostFormDataResponse
//        get_presigned_post_form_data(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::IsObjectLegalHoldEnabledResponse
        is_object_legal_hold_enabled(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::PutObjectResponse
        put_object(const std::string_view &bucketName, const std::string_view &objectName, std::istream file);

        minio::s3::PutObjectResponse
        put_object(const std::string_view &bucketName, const std::string_view &objectName,
                   std::basic_string_view<char> fileContent);

        minio::s3::PutObjectResponse
        put_object(const std::string_view &bucketName, const std::string_view &objectName, types::file_content file);

        minio::s3::PutObjectResponse
        put_object_response(const std::string_view &bucketName, const std::string_view &objectName,
                            std::basic_string_view<char> fileContent,
                            std::function<bool(minio::http::ProgressFunctionArgs)> progressCallback);

        minio::s3::PutObjectResponse
        put_object_response(const std::string_view &bucketName, const std::string_view &objectName,
                            types::file_content file,
                            std::function<bool(minio::http::ProgressFunctionArgs)> progressCallback);

        minio::s3::RemoveObjectResponse
        remove_object(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::RemoveObjectsResult
        remove_object(const std::string_view &bucketName, const std::vector<std::string_view> &objectName);

        minio::s3::SetObjectLockConfigResponse
        set_object_lock_config(const std::string_view &bucketName, minio::s3::SetObjectLockConfigArgs config);

        minio::s3::SetObjectTagsResponse
        set_object_tags(const std::string_view &bucketName, const std::string_view &objectName,
                        const std::map<std::string, std::string> &tags);

        minio::s3::StatObjectResponse
        stat_object(const std::string_view &bucketName, const std::string_view &objectName);

        minio::s3::UploadObjectResponse
        upload_object(const std::string_view &bucketName, const std::string_view &objectName,
                      types::file_content fileContent);
    private:
        minio::s3::Client &client;
    };
}

#endif //STORAGE_DELIGHT_OBJECT_OPERATION_HPP
