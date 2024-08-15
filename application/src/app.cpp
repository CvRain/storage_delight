#include "client.hpp"
#include <miniocpp/client.h>

using namespace storage_delight;

int main() {
//    auto provider = core::Client::make_provider("H4UMCiwa8ZgE0OorsFxX", "PyhL7fFAkVPnT1esNTIbBUuhp2ZztEz2qD8muv86");
//    minio::s3::BaseUrl url{"http://127.0.0.1:10567", false};

    auto provider = core::Client::make_provider("uiJ2kXR4V1ceWJPkHNfT", "7KBobqxCWyLQKhARhs6paIsmI4rwx1kx8Zpjghhd");
    minio::s3::BaseUrl url{"http://server.cloudvl.cn:10569", false};

    auto client = core::Client{url, &provider};

    const auto response = client.get_bucket_operation().listBuckets();
    for (const auto &it: response.buckets) {
        std::cout << it.name << std::endl;
    }

    return 0;
}