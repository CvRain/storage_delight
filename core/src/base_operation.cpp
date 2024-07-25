//
// Created by cvrain on 24-7-25.
//

#include "base_operation.hpp"

namespace storage_delight::core {

    void BaseOperation::log(spdlog::level::level_enum level, const std::string_view &message) const {
        if(enableLogOutputToConsole){
            spdlog::log(level, message);
        }
        if(enableLogOutputToFile){
            //todo
        }
    }

    void BaseOperation::setEnableLogOutputToConsole(bool enable) {
        enableLogOutputToConsole = enable;
    }

    void BaseOperation::setEnableLogOutputToFile(bool enable) {
        enableLogOutputToFile = enable;
    }
}