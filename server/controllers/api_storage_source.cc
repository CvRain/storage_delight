#include "api_storage_source.h"

#include "service/log_service.hpp"
#include "service/storage_service.hpp"

using namespace api;

// Add definition of your processing function here
void StorageSource::add_source(model_delight::NlohmannJsonRequestPtr        &&request,
                               std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::add_source");

    const auto request_body = request->getNlohmannJsonBody();

    // 检查请求字段是否完整
    const auto field_integrity =
            request_body.contains(schema::key::name) and request_body.contains(schema::key::url) and
            request_body.contains(schema::key::access_key) and request_body.contains(schema::key::secret_key) and
            request_body.contains(schema::key::user_id);

    if (not field_integrity) {
        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "Invalid request body", .result = "Invalid request body"};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
        return;
    }

    schema::DbDataSource data_source;
    data_source.access_key  = request_body[schema::key::access_key].get<std::string>();
    data_source.name        = request_body[schema::key::name].get<std::string>();
    data_source.secret_key  = request_body[schema::key::secret_key].get<std::string>();
    data_source.url         = request_body[schema::key::url].get<std::string>();
    data_source.create_time = util_delight::Date::get_current_timestamp_32();

    // 检查插入情况
    if (const auto [result, error] = service_delight::StorageService::get_instance().append_storage(&data_source);
        not result.has_value() or result.value() == false)
    {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::info,
                                                    "StorageSource::add_source failed: {}",
                                                    error);
        auto response =
                model_delight::BasicResponse{
                        .code = k500InternalServerError, .message = "Invalid request body", .result = error.data()}
                        .to_json();
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

        // 记录插入失败日志
        schema::DbOperationLog operation_log;
        operation_log.action         = "add_source";
        operation_log.source_name    = data_source.name;
        operation_log.current_state  = "failed";
        operation_log.description    = error;
        operation_log.previous_state = "none";
        operation_log.request_ip     = request->getRequest().getPeerAddr().toIp();
        operation_log.user_id        = bsoncxx::oid{request_body[schema::key::user_id].get<std::string_view>()};
        service_delight::LogService::get_instance().record_operation(&operation_log);

        return;
    }

    auto response = model_delight::BasicResponse{.code = k200OK, .message = "Success", .result = "Success"};
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));

    // 记录插入成功日志
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::add_source success");
    schema::DbOperationLog operation_log;
    operation_log.action         = "add_source";
    operation_log.source_name    = data_source.name;
    operation_log.current_state  = "success";
    operation_log.description    = "add storage source success";
    operation_log.previous_state = "none";
    operation_log.request_ip     = request->getRequest().getPeerAddr().toIp();
    operation_log.user_id        = bsoncxx::oid{request_body[schema::key::user_id].get<std::string_view>()};
    service_delight::LogService::get_instance().record_operation(&operation_log);
}

void StorageSource::remove_source(model_delight::NlohmannJsonRequestPtr        &&request,
                                  std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::remove_source");

    const auto request_body = request->getNlohmannJsonBody();
    const auto field_integrity =
            request_body.contains(schema::key::user_id)
    and request_body.contains(model_delight::basic_value::request::source_id);

    if (not field_integrity) {
        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "Invalid request body", .result = "Invalid request body"};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
        return;
    }

    const auto bson_id = bsoncxx::oid{request_body[model_delight::basic_value::request::source_id].get<std::string_view>()};

    try {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
            spdlog::level::debug,
            "StorageSource::remove_source try to fetch storage source name");

        const auto [fst, snd] = service_delight::StorageService::get_instance().get_storage_by_id(bson_id);

        const auto source_name = fst.value().find(schema::key::name)->get_string().value;
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::info,
                                                    "StorageSource::remove_source: {}",
                                                    source_name);

        if (const auto [result, error] = service_delight::StorageService::get_instance().remove_storage(bson_id);
            not result.has_value() or result.value() == false)
        {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::info,
                                                        "StorageSource::remove_source failed: {}",
                                                        error);

            throw std::runtime_error(error.data());
        }

        auto response = model_delight::BasicResponse{.code = k200OK, .message = "Success", .result = "Success"};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));

        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::info,
                                                    "StorageSource::remove_source success");

        schema::DbOperationLog operation_log;
        operation_log.action         = "remove_source";
        operation_log.source_name    = source_name;
        operation_log.current_state  = "success";
        operation_log.description    = "remove storage source success";
        operation_log.previous_state = "none";
        operation_log.request_ip     = request->getRequest().getPeerAddr().toIp();
        operation_log.user_id        = bsoncxx::oid{request_body[schema::key::user_id].get<std::string_view>()};
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::err,
                                                    "StorageSource::remove_source failed: {}",
                                                    e.what());

        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "StorageSource::remove_source failed", .result = e.what()};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));

        // 记录删除失败日志
        schema::DbOperationLog operation_log;
        operation_log.action         = "remove_source";
        operation_log.current_state  = "failed";
        operation_log.description    = e.what();
        operation_log.previous_state = "none";
        operation_log.request_ip     = request->getRequest().getPeerAddr().toIp();
        operation_log.user_id        = bsoncxx::oid{request_body[schema::key::user_id].get<std::string_view>()};
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
}

void StorageSource::list_all_source(model_delight::NlohmannJsonRequestPtr        &&request,
                                    std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::list_all_source");

    const auto request_body = request->getNlohmannJsonBody();

    if (const auto field_integrity = request_body.contains(schema::key::user_id); not field_integrity) {
        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "Invalid request body", .result = "Invalid request body"};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
        return;
    }

    const auto user_id = bsoncxx::oid{request_body[schema::key::user_id].get<std::string_view>()};

    const auto [result, error] = service_delight::StorageService::get_instance().list_all_storages();
    if (not result.has_value()) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::info,
                                                    "StorageSource::list_all_source failed: {}",
                                                    error);

        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "StorageSource::list_all_source failed", .result = error.data()};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
        return;
    }
    std::vector<nlohmann::json> data_sources;
    for (const auto &document: result.value()) {
        auto data_source = schema::DbDataSource::from_bson(document);
        data_sources.emplace_back(data_source.to_json());
    }
    auto response = model_delight::BasicResponse{
            .code = k200OK, .message = "Success", .result = "Success", .data = data_sources};

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));

    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::list_all_source success");

    schema::DbOperationLog operation_log;
    operation_log.action         = "list_all_source";
    operation_log.current_state  = "success";
    operation_log.description    = "list all storage source success";
    operation_log.previous_state = "none";
    operation_log.request_ip     = request->getRequest().getPeerAddr().toIp();
    operation_log.user_id        = user_id;
    service_delight::LogService::get_instance().record_operation(&operation_log);
}

void StorageSource::list_all_ids(model_delight::NlohmannJsonRequestPtr        &&request,
                                 std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::list_all_ids");

    const auto request_body    = request->getNlohmannJsonBody();
    const auto field_integrity = request_body.contains(schema::key::user_id);

    if (not field_integrity) {
        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "Invalid request body", .result = "Invalid request body"};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
        return;
    }
    const auto user_id         = bsoncxx::oid{request_body[schema::key::user_id].get<std::string_view>()};
    const auto [result, error] = service_delight::StorageService::get_instance().list_all_storage_ids();
    if (not result.has_value()) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::info,
                                                    "StorageSource::list_all_ids failed: {}",
                                                    error);

        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "StorageSource::list_all_ids failed", .result = error.data()};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
        return;
    }
    std::vector<std::string> data_source_ids;
    for (const auto &id: result.value()) {
        data_source_ids.emplace_back(id.to_string());
    }

    auto response = model_delight::BasicResponse{
            .code    = k200OK,
            .message = std::string{"Success"},
            .result  = "Success",
            .data    = data_source_ids,
    };

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::list_all_ids success");

    schema::DbOperationLog operation_log;
    operation_log.action         = "list_all_ids";
    operation_log.current_state  = "success";
    operation_log.description    = "list all storage source ids success";
    operation_log.previous_state = "none";
    operation_log.request_ip     = request->getRequest().getPeerAddr().toIp();
    operation_log.user_id        = user_id;
    service_delight::LogService::get_instance().record_operation(&operation_log);
}

void StorageSource::get_one_source(model_delight::NlohmannJsonRequestPtr        &&request,
                                   std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::get_one_source");

    const auto request_body    = request->getNlohmannJsonBody();
    const auto field_integrity = request_body.contains(schema::key::user_id) and
                                 request_body.contains(model_delight::basic_value::request::source_id);

    if (not field_integrity) {
        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "Invalid request body", .result = "Invalid request body"};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
        return;
    }

    const auto user_id = bsoncxx::oid{request_body[schema::key::user_id].get<std::string_view>()};
    const auto data_source_id =
            bsoncxx::oid{request_body[model_delight::basic_value::request::source_id].get<std::string_view>()};
    const auto [result, error] = service_delight::StorageService::get_instance().get_storage_by_id(data_source_id);

    if (not result.has_value()) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::info,
                                                    "StorageSource::get_one_source failed: {}",
                                                    error);
        auto response = model_delight::BasicResponse{
                .code = k400BadRequest, .message = "StorageSource::get_one_source failed", .result = error.data()};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
        return;
    }
    auto response = model_delight::BasicResponse{
            .code    = k200OK,
            .message = "Success",
            .result  = "Success",
            .data    = schema::DbDataSource::from_bson((result.value())).to_json(),
    };

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                spdlog::level::info,
                                                "StorageSource::get_one_source success");

    schema::DbOperationLog operation_log;
    operation_log.action         = "get_one_source";
    operation_log.current_state  = "success";
    operation_log.description    = "get one storage source success";
    operation_log.previous_state = "none";
    operation_log.request_ip     = request->getRequest().getPeerAddr().toIp();
    operation_log.user_id        = user_id;
    service_delight::LogService::get_instance().record_operation(&operation_log);
}
