//
// Created by cvrain on 24-7-26.
//

#include "object_operation.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

namespace storage_delight::core {

    ObjectOperation::ObjectOperation(const std::shared_ptr<minio::s3::Client> &client) : client(client) {}

    minio::s3::ComposeObjectResponse ObjectOperation::compose_object(
            const minio::s3::ComposeObjectArgs &composeObjectArgs, const std::list<minio::s3::ComposeSource> &sources) {
        return execute_operation(
                [&]() {
                    auto args    = minio::s3::ComposeObjectArgs(composeObjectArgs);
                    args.sources = sources;
                    return client->ComposeObject(args);
                },
                "ComposeObject");
    }

    minio::s3::ComposeObjectResponse ObjectOperation::compose_object(
            const minio::s3::ComposeObjectArgs                    &composeObjectArgs,
            const std::initializer_list<minio::s3::ComposeSource> &sources) {
        return execute_operation(
                [&]() {
                    std::list<minio::s3::ComposeSource> list{sources};
                    return compose_object(composeObjectArgs, list);
                },
                "ComposeObject");
    }

    minio::s3::Response ObjectOperation::copy_object(const std::string_view &buckName,
                                                     const std::string_view &objectName,
                                                     const std::string_view &distBuck,
                                                     const std::string_view &distObject) {
        return execute_operation(
                [&]() {
                    minio::s3::CopyObjectArgs args;
                    args.bucket = buckName;
                    args.object = objectName;

                    minio::s3::CopySource source;
                    source.bucket = distBuck;
                    source.object = distObject;
                    args.source   = source;

                    return copy_object(args, source);
                },
                "CopyObject");
    }

    minio::s3::Response ObjectOperation::copy_object(const minio::s3::CopyObjectArgs &copyArgs,
                                                     const minio::s3::CopySource     &source) {
        return static_cast<minio::s3::Response>(execute_operation(
                [&]() {
                    auto cloneArgs   = copyArgs;
                    cloneArgs.source = source;
                    return client->CopyObject(cloneArgs);
                },
                "copy_object"));
    }

    minio::s3::GetObjectResponse ObjectOperation::get_object(const std::string_view &bucketName,
                                                             const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::GetObjectArgs args;
                    args.bucket   = bucketName;
                    args.object   = objectName;
                    args.datafunc = [](const minio::http::DataFunctionArgs &args) {
                        // do something
                        return true;
                    };

                    return client->GetObject(args);
                },
                "get_object");
    }

    minio::s3::GetObjectLockConfigResponse ObjectOperation::get_object_config(const std::string_view &bucketName) {
        return execute_operation(
                [&]() {
                    minio::s3::GetObjectLockConfigArgs args;
                    args.bucket = bucketName;
                    return client->GetObjectLockConfig(args);
                },
                "get_object_config");
    }

    minio::s3::GetObjectResponse ObjectOperation::get_object_progress(const std::string_view &bucketName,
                                                                      const std::string_view &objectName) {
        minio::s3::GetObjectArgs args;
        args.bucket   = bucketName;
        args.object   = objectName;
        args.datafunc = [&](const auto &dataArgs) {
            log(spdlog::level::info, fmt::format("receive data: {} bytes", dataArgs.datachunk.length()));
            return true;
        };

        args.progressfunc = [&](auto progressArgs) {
            if (progressArgs.download_speed > 0) {
                log(spdlog::level::info, fmt::format("download speed: {} bytes/s", progressArgs.download_speed));
            }
            else {
                log(spdlog::level::info, fmt::format("bytes of {} bytes", progressArgs.download_total_bytes));
            }
            return true;
        };

        return client->GetObject(args);
    }

    minio::s3::GetObjectRetentionResponse ObjectOperation::get_object_retention(const std::string_view &bucketName,
                                                                                const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::GetObjectRetentionArgs args;
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->GetObjectRetention(args);
                },
                "get_object_retention");
    }

    minio::s3::GetObjectTagsResponse ObjectOperation::get_object_tags(const std::string_view &bucketName,
                                                                      const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::GetObjectTagsArgs args;
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->GetObjectTags(args);
                },
                "get_object_tags");
    }

    minio::s3::DeleteObjectLockConfigResponse ObjectOperation::delete_object_config(
            const std::string_view &bucketName) {
        return execute_operation(
                [&]() {
                    minio::s3::DeleteObjectLockConfigArgs args;
                    args.bucket = bucketName;
                    return client->DeleteObjectLockConfig(args);
                },
                "delete_object_config");
    }

    minio::s3::DisableObjectLegalHoldResponse ObjectOperation::disable_object_legal_hold(
            const std::string_view &bucketName, const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::DisableObjectLegalHoldArgs args;
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->DisableObjectLegalHold(args);
                },
                "disable_object_legal_hold");
    }

    minio::s3::DownloadObjectResponse ObjectOperation::download_object(const minio::s3::DownloadObjectArgs &args) {
        return execute_operation([&]() { return client->DownloadObject(args); }, "download_object");
    }

    minio::s3::DownloadObjectResponse ObjectOperation::download_object(const std::string_view &bucketName,
                                                                       const std::string_view &objectName,
                                                                       const std::string_view &fileName) {
        return download_object([&]() {
            minio::s3::DownloadObjectArgs args;
            args.bucket   = bucketName;
            args.object   = objectName;
            args.filename = fileName;
            return args;
        }());
    }

    minio::s3::EnableObjectLegalHoldResponse ObjectOperation::enable_object_legal_hold(
            const std::string_view &bucketName, const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::EnableObjectLegalHoldArgs args;
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->EnableObjectLegalHold(args);
                },
                "enable_object_legal_hold");
    }

    minio::s3::ListObjectsResult ObjectOperation::list_objects(const std::string_view &bucketName) {
        return execute_operation(
                [&]() {
                    minio::s3::ListObjectsArgs args;
                    args.bucket = bucketName;
                    return client->ListObjects(args);
                },
                "list_objects");
    }

    minio::s3::GetPresignedObjectUrlResponse ObjectOperation::get_presigned_object_url(
            const std::string_view &bucketName, const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::GetPresignedObjectUrlArgs args;
                    args.bucket         = bucketName;
                    args.object         = objectName;
                    args.method         = minio::http::Method::kGet;
                    args.expiry_seconds = 60 * 60 * 24;  // 24 hours

                    return client->GetPresignedObjectUrl(args);
                },
                "get_presigned_object_url");
    }

    minio::s3::IsObjectLegalHoldEnabledResponse ObjectOperation::is_object_legal_hold_enabled(
            const std::string_view &bucketName, const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::IsObjectLegalHoldEnabledArgs args;
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->IsObjectLegalHoldEnabled(args);
                },
                "is_object_legal_hold_enabled");
    }

    minio::s3::PutObjectResponse ObjectOperation::put_object(const std::string_view &bucketName,
                                                             const std::string_view &objectName,
                                                             std::istream            file) {
        return execute_operation(
                [&]() {
                    const auto               file_size = file.seekg(0, std::ios::end).tellg();
                    minio::s3::PutObjectArgs args(file, file_size, 0);
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->PutObject(args);
                },
                "put_object");
    }

    minio::s3::PutObjectResponse ObjectOperation::put_object(const std::string_view      &bucketName,
                                                             const std::string_view      &objectName,
                                                             std::basic_string_view<char> fileContent) {
        return execute_operation(
                [&]() {
                    const auto         file_size = fileContent.size();
                    std::string        str(fileContent.data(), fileContent.size());
                    std::istringstream data(str);

                    minio::s3::PutObjectArgs args(data, static_cast<long>(file_size), 0);
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->PutObject(args);
                },
                "put_object");
    }

    minio::s3::PutObjectResponse ObjectOperation::put_object(const std::string_view    &bucketName,
                                                             const std::string_view    &objectName,
                                                             const types::file_content &file) {
        return execute_operation(
                [&]() {
                    std::istringstream       data(file.content);
                    minio::s3::PutObjectArgs args(data, static_cast<long>(file.size), 0);
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->PutObject(args);
                },
                "put_object");
    }

    minio::s3::PutObjectResponse ObjectOperation::put_object_response(
            const std::string_view                                       &bucketName,
            const std::string_view                                       &objectName,
            std::basic_string_view<char>                                  fileContent,
            const std::function<bool(minio::http::ProgressFunctionArgs)> &progressCallback) {
        return execute_operation(
                [&]() {
                    const auto         file_size = fileContent.size();
                    std::string        str(fileContent.data(), fileContent.size());
                    std::istringstream data(str);

                    minio::s3::PutObjectArgs args(data, static_cast<long>(file_size), 0);
                    args.bucket = bucketName;
                    args.object = objectName;

                    if (progressCallback != nullptr) {
                        args.progressfunc = progressCallback;
                    }

                    return client->PutObject(args);
                },
                "put_object_response");
    }

    minio::s3::PutObjectResponse ObjectOperation::put_object_response(
            const std::string_view                                       &bucketName,
            const std::string_view                                       &objectName,
            const types::file_content                                    &file,
            const std::function<bool(minio::http::ProgressFunctionArgs)> &progressCallback) {
        return execute_operation(
                [&]() {
                    std::istringstream data(file.content);

                    minio::s3::PutObjectArgs args(data, static_cast<long>(file.size), 0);
                    args.bucket = bucketName;
                    args.object = objectName;
                    if (progressCallback != nullptr) {
                        args.progressfunc = progressCallback;
                    }

                    return client->PutObject(args);
                },
                "put_object_response");
    }

    minio::s3::RemoveObjectResponse ObjectOperation::remove_object(const std::string_view &bucketName,
                                                                   const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::RemoveObjectArgs args;
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->RemoveObject(args);
                },
                "remove_object");
    }

    minio::s3::RemoveObjectsResult ObjectOperation::remove_object(const std::string_view              &bucketName,
                                                                  const std::vector<std::string_view> &objectName) {
        return execute_operation(
                [&]() {
                    std::list<minio::s3::DeleteObject> objects;
                    for (const auto &name: objectName) {
                        minio::s3::DeleteObject object;
                        object.name = name;
                        objects.push_back(object);
                    }
                    auto i = objects.begin();

                    minio::s3::RemoveObjectsArgs args;
                    args.bucket = bucketName;
                    args.func   = [&](minio::s3::DeleteObject &obj) {
                        if (i == objects.end()) {
                            return false;
                        }
                        obj = *i;
                        ++i;
                        return true;
                    };

                    return client->RemoveObjects(args);
                },
                "remove_object");
    }

    minio::s3::SetObjectLockConfigResponse ObjectOperation::set_object_lock_config(
            const std::string_view &bucketName, const minio::s3::SetObjectLockConfigArgs &config) {
        return execute_operation(
                [&]() {
                    minio::s3::SetObjectLockConfigArgs args;
                    args.bucket = bucketName;
                    return client->SetObjectLockConfig(args);
                },
                "set_object_config");
    }

    minio::s3::SetObjectTagsResponse ObjectOperation::set_object_tags(const std::string_view &bucketName,
                                                                      const std::string_view &objectName,
                                                                      const std::map<std::string, std::string> &tags) {
        return execute_operation(
                [&]() {
                    minio::s3::SetObjectTagsArgs args;
                    args.bucket = bucketName;
                    args.object = objectName;
                    args.tags   = tags;
                    return client->SetObjectTags(args);
                },
                "set_object_tags");
    }

    minio::s3::StatObjectResponse ObjectOperation::stat_object(const std::string_view &bucketName,
                                                               const std::string_view &objectName) {
        return execute_operation(
                [&]() {
                    minio::s3::StatObjectArgs args;
                    args.bucket = bucketName;
                    args.object = objectName;
                    return client->StatObject(args);
                },
                "stat_object");
    }

    minio::s3::UploadObjectResponse ObjectOperation::upload_object(const std::string_view    &bucketName,
                                                                   const std::string_view    &objectName,
                                                                   const types::file_content &fileContent) {
        return execute_operation(
                [&]() {
                    minio::s3::UploadObjectArgs args;
                    args.bucket   = bucketName;
                    args.object   = objectName;
                    args.filename = fileContent.name;

                    return client->UploadObject(args);
                },
                "upload_object");
    }

}  // namespace storage_delight::core
