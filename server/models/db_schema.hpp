//
// Created by cvrain on 24-8-13.
//

#ifndef STORAGE_DELIGHT_DB_SCHEMA_HPP
#define STORAGE_DELIGHT_DB_SCHEMA_HPP

#include <string>
#include <sqlite_orm/sqlite_orm.h>

namespace schema {
    enum UserRole : int {
        TypeNone = -1,
        TypeAdmin = 0,
        TypeUser = 1,
    };

    struct BaseUser {
        UserRole role = UserRole::TypeNone;
        std::string user_name = "unknown";
        std::string password = "unknown";
    };

    struct User {
        int id = -1;
        int role = UserRole::TypeUser;
        std::string user_name;
        std::string password;
        std::string create_time;
    };


    struct MinioClient {
        int id;
        bool is_http;
        std::string name;
        std::string access_key;
        std::string secret_key;
        std::string link_url;
        std::string create_time;
    };

    inline auto db_user_table = sqlite_orm::make_table(
            "user",
            sqlite_orm::make_column("id", &User::id, sqlite_orm::primary_key()),
            sqlite_orm::make_column("role", &User::role),
            sqlite_orm::make_column("password", &User::password),
            sqlite_orm::make_column("user_name", &User::user_name),
            sqlite_orm::make_column("create_time", &User::create_time)
    );

    inline auto db_minio_client_table = sqlite_orm::make_table(
            "minio_client",
            sqlite_orm::make_column("id", &MinioClient::id, sqlite_orm::primary_key()),
            sqlite_orm::make_column("is_http", &MinioClient::is_http),
            sqlite_orm::make_column("name", &MinioClient::name),
            sqlite_orm::make_column("access_key", &MinioClient::access_key),
            sqlite_orm::make_column("secret_key", &MinioClient::secret_key),
            sqlite_orm::make_column("link_url", &MinioClient::link_url)
    );


}

#endif //STORAGE_DELIGHT_DB_SCHEMA_HPP
