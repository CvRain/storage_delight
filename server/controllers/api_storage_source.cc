#include "api_storage_source.h"
#include "service/log_service.hpp"

using namespace api;

// Add definition of your processing function here
void StorageSource::add_source(model_delight::NlohmannJsonRequestPtr &&request,
                               std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger, spdlog::level::info,
        "StorageSource::add_source");
}
