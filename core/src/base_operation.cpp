//
// Created by cvrain on 24-7-25.
//

#include "base_operation.hpp"

namespace storage_delight::core {

    void BaseOperation::log(spdlog::level::level_enum level, const std::string_view &message) const {
        if (enable_log_to_console) {
            spdlog::log(level, message);
        }
        if (enable_log_to_file) {
            //todo
        }
    }

    void BaseOperation::set_enable_log_to_console(bool enable) {
        enable_log_to_console = enable;
    }

    void BaseOperation::set_enable_log_file(bool enable) {
        enable_log_to_file = enable;
    }
}