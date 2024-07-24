//
// Created by cvrain on 24-7-24.
//

#include "client.hpp"

#include <miniocpp/client.h>
#include <spdlog/spdlog.h>

namespace storage_delight::core {
    Client::Client(minio::s3::BaseUrl base_url, minio::creds::StaticProvider &provider)
            : client(base_url, &provider) {
    }

    /**
     * @brief 创建一个日志装饰器函数
     *
     * 该函数通过lambda表达式捕获传入的函数f和函数名funcName，并返回一个新的函数。
     * 新函数在调用原始函数f前后会根据日志启用状态打印进入和退出消息。
     * 如果原始函数f返回void，则只在调用后打印退出消息；否则，同时在调用前后的打印消息。
     *
     * @tparam Func 原始函数f的类型
     * @param f 要装饰的函数
     * @param funcName 函数名，用于日志打印
     * @return 返回一个新的函数，该函数在调用原始函数前后添加了日志功能
     */
    template<typename Func>
    auto Client::logDecorator(Func f, const std::string_view &funcName) {
        // 返回一个lambda函数
        return [this, f, funcName](auto &&... args) {
            // 如果日志启用，打印进入函数的消息
            if (isEnableLog) {
                spdlog::info("Entering {}", funcName);
            }
            // 判断原始函数的返回类型是否为void
            if constexpr (std::is_void_v<std::invoke_result_t<Func, decltype(args)...>>) {
                // 如果是void函数，调用原始函数
                f(std::forward<decltype(args)>(args)...);
                // 如果日志启用，打印退出函数的消息
                if (isEnableLog) {
                    spdlog::info("Exiting {}", funcName);
                }
            } else {
                // 如果不是void函数，调用原始函数并保存结果
                auto result = f(std::forward<decltype(args)>(args)...);
                // 如果日志启用，打印退出函数的消息
                if (isEnableLog) {
                    spdlog::info("Exiting {}", funcName);
                }
                // 返回原始函数的结果
                return result;
            }
        };
    }


    std::optional<std::list<minio::s3::Bucket>> Client::listBuckets() {
        const auto response = client.ListBuckets();
        if (!response) {
            return std::nullopt;
        }
        auto decoratedFunc = logDecorator([&]() {
            for (const auto &it: response.buckets) {
                spdlog::info("bucket: {}", it.name);
            }
        }, std::string_view("listBuckets"));

        decoratedFunc();

        return response.buckets;
    }

    void Client::setLogEnable(bool enable) {
        isEnableLog = enable;
    }


}