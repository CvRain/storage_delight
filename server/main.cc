#include <drogon/drogon.h>

#include "client.hpp"

using namespace storage_delight;

int main() {
    auto provider = core::Client::make_provider("uiJ2kXR4V1ceWJPkHNfT", "7KBobqxCWyLQKhARhs6paIsmI4rwx1kx8Zpjghhd");
    minio::s3::BaseUrl url{"http://server.cloudvl.cn:10569", false};

    auto client = core::Client{url, &provider};

    const auto response = client.getBucketOperation().listBuckets();
    for (const auto &it: response.buckets) {
        std::cout << it.name << std::endl;
    }

    //Set HTTP listener address and port
    drogon::app().loadConfigFile("config.json");
    drogon::app().run();

    return 0;
}
