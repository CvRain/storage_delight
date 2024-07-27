#include "client.hpp"

#include <miniocpp/client.h>

int main() {
    auto base_url = minio::s3::BaseUrl{"http://127.0.0.1:9000", false};
    minio::creds::StaticProvider provider{"exjJK7R9buUmW46lNpwx", "aDGaikn07kPMIUKnVY3v58BDrsnh2uf0Q9TNmfig"};

    storage_delight::core::Client client{base_url, provider};
    client.getBucketOperation().set_enable_log_to_console(true);
    client.getBucketOperation().listBuckets();
    client.getBucketOperation().bucket_exists("aaa");

}