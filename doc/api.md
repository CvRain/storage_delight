---
title: Cloudvl
language_tabs:
  - shell: Shell
  - http: HTTP
  - javascript: JavaScript
  - ruby: Ruby
  - python: Python
  - php: PHP
  - java: Java
  - go: Go
toc_footers: []
includes: []
search: true
code_clipboard: true
highlight_theme: darkula
headingLevel: 2
generator: "@tarslib/widdershins v4.0.23"

---

# Cloudvl

Base URLs:

# Authentication

# storage_delight

## POST user::add

POST /api/user/add

> Body 请求参数

```json
{
  "name": "string",
  "password": "string",
  "role": 1
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» name|body|string| 是 |none|
|» password|body|string| 是 |none|
|» role|body|integer| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "data": {
    "_id": "67458c4515134c6f7e0cfda8",
    "create_time": 1732611141,
    "group_id": "67458c4515134c6f7e0cfdaa",
    "name": "cvrain",
    "password": "2c835ba8966d902120fb4504037fad34effa4b9461e988e4c4da073ad50dae82",
    "role": 0,
    "update_time": 1732611141
  },
  "message": "add user success",
  "result": "ok"
}
```

> 400 Response

```json
{
  "code": 0,
  "message": "string",
  "result": "string"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|
|400|[Bad Request](https://tools.ietf.org/html/rfc7231#section-6.5.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|object|true|none||none|
|»» _id|string|true|none||none|
|»» create_time|integer|true|none||none|
|»» group_id|string|true|none||none|
|»» name|string|true|none||none|
|»» password|string|true|none||none|
|»» role|integer|true|none||none|
|»» update_time|integer|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

状态码 **400**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## GET user::find_by_id

GET /api/user/id/

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|user_id|query|string| 是 |none|
|Authorization|header|string| 否 |none|

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

## POST user::login

POST /api/user/login

> Body 请求参数

```json
{
  "name": "cvrain",
  "password": "qwe123!@#"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» name|body|string| 是 |none|
|» password|body|string| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "message": "Login successfully",
  "token": "ewoJImFsZyIgOiAiSFMyNTYiLAoJInR5cCIgOiAiSldUIgp9Cg==.ewoJImF1ZCIgOiAwLAoJImV4cCIgOiAxNzI0MzM2Mjc0LAoJImlhdCIgOiAxNzI0MzMyNjc0LAoJImlzcyIgOiAic3RvcmFnZV9kZWxpZ2h0IiwKCSJzdWIiIDogImxvZ2luIiwKCSJ1c2VyX2lkIiA6IDIKfQo=.ZDJiYzkxMzZmZWRmYTdiZWZjNTg5MjNlNjA5OWVmOGYyODdjZjk4NzIzYmQzYzJkMWI4ZWFhYzI3Njk3OTYyOA==",
  "user_id": 2,
  "user_name": "cvrain"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» message|string|true|none||none|
|» token|string|true|none||none|
|» user_id|integer|true|none||none|
|» user_name|string|true|none||none|

## PATCH group::add_member

PATCH /api/group/add_members

> Body 请求参数

```json
{
  "user_id": "string",
  "group_id": "string",
  "member_id": [
    "string"
  ]
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|
|» group_id|body|string| 是 |none|
|» member_id|body|[string]| 是 |none|

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

## DELETE group::remove_member

DELETE /api/group/members/remove

> Body 请求参数

```json
{
  "user_id": "string",
  "group_id": "string",
  "members_id": [
    "string"
  ]
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|
|» group_id|body|string| 是 |none|
|» members_id|body|[string]| 是 |none|

> 返回示例

> 200 Response

```json
{
  "code": 0,
  "data": null,
  "message": "string",
  "result": "string"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|null|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## PUT storage_source::add

PUT /api/StorageSource/add

> Body 请求参数

```json
{
  "name": "cloudavl_test",
  "url": "http://server.cloudvl.cn:10569",
  "access_key": "yg2my5KTBQnQuCWDhqS1",
  "secret_key": "4B4awK7FDViDenrs5qGoAfsmkhWvuzPYtgbZfK8C",
  "user_id": "670d399eb8679e9edc0a90c3",
  "is_https": false
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» name|body|string| 是 |none|
|» url|body|string| 是 |none|
|» access_key|body|string| 是 |none|
|» secret_key|body|string| 是 |none|
|» user_id|body|string| 是 |none|
|» is_https|body|boolean| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "data": null,
  "message": "Success",
  "result": "Success"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|null|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## GET storage_source::list

GET /api/StorageSource/list

> Body 请求参数

```json
{
  "user_id": "67345be792fb1c912d0cbdb3"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "data": [
    {
      "_id": "67345be792fb1c912d0cbdb3",
      "access_key": "uiJ2kXR4V1ceWJPkHNfT",
      "create_time": 1731484647,
      "name": "cloudvl",
      "secret_key": "7KBobqxCWyLQKhARhs6paIsmI4rwx1kx8Zpjghhd",
      "url": "http://server.cloudvl.cn:10569"
    },
    {
      "_id": "6734670d840bd161d30c9799",
      "access_key": "uiJ2kXR4V1ceWJPkHNfT",
      "create_time": 1731487501,
      "name": "cloudavl_test",
      "secret_key": "7KBobqxCWyLQKhARhs6paIsmI4rwx1kx8Zpjghhd",
      "url": "http://server.cloudvl.cn:10569"
    }
  ],
  "message": "Success",
  "result": "Success"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|[object]|true|none||none|
|»» _id|string|true|none||none|
|»» access_key|string|true|none||none|
|»» create_time|integer|true|none||none|
|»» name|string|true|none||none|
|»» secret_key|string|true|none||none|
|»» url|string|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## GET storage_source::ids

GET /api/StorageSource/ids

> Body 请求参数

```json
{
  "user_id": "string"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "data": [
    "67345be792fb1c912d0cbdb3",
    "6734670d840bd161d30c9799"
  ],
  "message": "Success",
  "result": "Success"
}
```

> 500 Response

```json
{
  "code": 0,
  "data": "string",
  "error": "string"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|
|500|[Internal Server Error](https://tools.ietf.org/html/rfc7231#section-6.6.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|[string]|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

状态码 **500**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|string|true|none||none|
|» error|string|true|none||none|

## GET storage_source::one

GET /api/StorageSource/one

> Body 请求参数

```json
{
  "user_id": "670d399eb8679e9edc0a90c3",
  "source_id": "67345be792fb1c912d0cbdb3"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|
|» source_id|body|string| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "data": {
    "_id": "67345be792fb1c912d0cbdb3",
    "access_key": "uiJ2kXR4V1ceWJPkHNfT",
    "create_time": 1731484647,
    "name": "cloudvl",
    "secret_key": "7KBobqxCWyLQKhARhs6paIsmI4rwx1kx8Zpjghhd",
    "url": "http://server.cloudvl.cn:10569"
  },
  "message": "Success",
  "result": "Success"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

## DELETE storage_source::delete

DELETE /api/StorageSource/remove

> Body 请求参数

```json
{
  "user_id": "670d399eb8679e9edc0a90c3",
  "source_id": "6734670d840bd161d30c9799"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|
|» source_id|body|string| 是 |none|

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

## PUT bucket::add

PUT /api/bucket/add

> Body 请求参数

```json
{
  "user_id": "67398201a65ca166090d7e52",
  "bucket_name": "test",
  "source_id": "67398276a65ca166090d7e5e"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|
|» bucket_name|body|string| 是 |none|
|» source_id|body|string| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "data": {
    "bucket_id": "67407b20b32048e59b0857b5",
    "bucket_name": "test",
    "source_name": "67398276a65ca166090d7e5e"
  },
  "message": "k200OK",
  "result": "success"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|object|true|none||none|
|»» bucket_id|string|true|none||none|
|»» bucket_name|string|true|none||none|
|»» source_name|string|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## GET bucket::list

GET /api/bucket/list

> Body 请求参数

```json
{
  "user_id": "string",
  "source_id": "string"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|
|» source_id|body|string| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "data": [
    "another-bucket"
  ],
  "message": "k200OK",
  "result": "Success"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|[string]|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## PATCH group::buckets::add

PATCH /api/Group/buckets/add

> Body 请求参数

```json
{
  "user_id": "string",
  "group_id": "string",
  "source_id": "string",
  "bucket_name": "string"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|
|» group_id|body|string| 是 |none|
|» source_id|body|string| 是 |none|
|» bucket_name|body|string| 是 |none|

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

## DELETE group::bucket::remove

DELETE /api/Group/buckets/remove

> Body 请求参数

```json
{
  "user_id": "67459983948109dbd1079573",
  "group_id": "67459983948109dbd1079575",
  "source_id": "673ea940b4f544a2c2092b3d",
  "bucket_name": "another-bucket"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» user_id|body|string| 是 |none|
|» group_id|body|string| 是 |none|
|» source_id|body|string| 是 |none|
|» bucket_name|body|string| 是 |none|

> 返回示例

```json
{
  "code": 200,
  "data": null,
  "message": "k200OK",
  "result": "Success to remove bucket"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» data|null|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

# storage_delight/test

## GET hello::say

GET /api/hello/say

> 返回示例

```json
{
  "code": 200,
  "message": "1726932354",
  "result": "ok"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## GET hello::echo

GET /api/hello/echo

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|message|query|string| 否 |none|

> 返回示例

```json
{
  "code": 200,
  "message": "Jeffrey Hall",
  "result": "ok"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## POST hello::hello

POST /api/hello/hello

> Body 请求参数

```yaml
text: "123"
file: file:///home/cvrain/图片/209_1710921879_hd.jpeg

```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» text|body|string| 是 |none|
|» file|body|string(binary)| 是 |none|

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

## POST hello::test_json_body

POST /api/hello/test_json_body

> Body 请求参数

```json
{
  "name": "string",
  "id": "string",
  "email": "string"
}
```

### 请求参数

|名称|位置|类型|必选|说明|
|---|---|---|---|---|
|body|body|object| 否 |none|
|» name|body|string| 是 |none|
|» id|body|string| 是 |none|
|» email|body|string| 是 |none|

> 返回示例

> 200 Response

```json
{
  "code": 0,
  "message": "string",
  "result": "string"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» code|integer|true|none||none|
|» message|string|true|none||none|
|» result|string|true|none||none|

## POST hello::test_requets

POST /api/hello/test_request

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

## GET hello::schema::user

GET /api/hello/schema/user

> 返回示例

```json
{
  "_id": "672234154f9dce055e0de395",
  "create_time": 1730294805,
  "group_id": "672234154f9dce055e0de394",
  "name": "test",
  "password": "test",
  "role": 1,
  "update_time": 0
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» _id|string|true|none||none|
|» create_time|integer|true|none||none|
|» group_id|string|true|none||none|
|» name|string|true|none||none|
|» password|string|true|none||none|
|» role|integer|true|none||none|
|» update_time|integer|true|none||none|

## GET hello:schema::bucket

GET /api/hello/schema/bucket

> 返回示例

```json
{
  "_id": "6722347a4f9dce055e0de39d",
  "bucket_name": "test bucket",
  "create_time": 1730294906,
  "data_source": "6722347a4f9dce055e0de39b",
  "group_id": "6722347a4f9dce055e0de39c",
  "permission_id": "6722347a4f9dce055e0de39e",
  "tags": [
    "tag1",
    "tag2",
    "tag3"
  ],
  "update_time": 635406336
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» _id|string|true|none||none|
|» bucket_name|string|true|none||none|
|» create_time|integer|true|none||none|
|» data_source|string|true|none||none|
|» group_id|string|true|none||none|
|» permission_id|string|true|none||none|
|» tags|[string]|true|none||none|
|» update_time|integer|true|none||none|

## GET hello::schema::group

GET /api/hello/schema/group

> 返回示例

```json
{
  "_id": "672237e14f9dce055e0de3a1",
  "bucket_group_id": [
    "672237e14f9dce055e0de3a3",
    "672237e14f9dce055e0de3a4"
  ],
  "create_time": 635406336,
  "members_id": [
    "672237e14f9dce055e0de3a5",
    "672237e14f9dce055e0de3a6"
  ],
  "name": "test group",
  "owner_id": "672237e14f9dce055e0de3a7",
  "update_time": 30173
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» _id|string|true|none||none|
|» bucket_group_id|[string]|true|none||none|
|» create_time|integer|true|none||none|
|» members_id|[string]|true|none||none|
|» name|string|true|none||none|
|» owner_id|string|true|none||none|
|» update_time|integer|true|none||none|

## GET hello::schema::permission

GET /api/hello/schema/permission

> 返回示例

```json
{
  "_id": "672238bb4f9dce055e0de3a9",
  "allow_actions": {
    "allow_create_bucket": [
      "672238bb4f9dce055e0de3ad",
      "672238bb4f9dce055e0de3ae"
    ],
    "allow_delete": [
      "672238bb4f9dce055e0de3b3",
      "672238bb4f9dce055e0de3b4"
    ],
    "allow_delete_bucket": [
      "672238bb4f9dce055e0de3af",
      "672238bb4f9dce055e0de3b0"
    ],
    "allow_grant": [
      "672238bb4f9dce055e0de3b5",
      "672238bb4f9dce055e0de3b6"
    ],
    "allow_list": [
      "672238bb4f9dce055e0de3b7",
      "672238bb4f9dce055e0de3b8"
    ],
    "allow_read": [
      "672238bb4f9dce055e0de3ab",
      "672238bb4f9dce055e0de3ac"
    ],
    "allow_revoke": [
      "672238bb4f9dce055e0de3bb",
      "672238bb4f9dce055e0de3bc"
    ],
    "allow_update_bucket": [
      "672238bb4f9dce055e0de3b1",
      "672238bb4f9dce055e0de3b2"
    ],
    "allow_write": [
      "672238bb4f9dce055e0de3b9",
      "672238bb4f9dce055e0de3ba"
    ]
  },
  "bucket_id": "672238bb4f9dce055e0de3aa",
  "create_time": "",
  "description": "test permission description",
  "name": "test permission",
  "update_time": ""
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» _id|string|true|none||none|
|» allow_actions|object|true|none||none|
|»» allow_create_bucket|[string]|true|none||none|
|»» allow_delete|[string]|true|none||none|
|»» allow_delete_bucket|[string]|true|none||none|
|»» allow_grant|[string]|true|none||none|
|»» allow_list|[string]|true|none||none|
|»» allow_read|[string]|true|none||none|
|»» allow_revoke|[string]|true|none||none|
|»» allow_update_bucket|[string]|true|none||none|
|»» allow_write|[string]|true|none||none|
|» bucket_id|string|true|none||none|
|» create_time|string|true|none||none|
|» description|string|true|none||none|
|» name|string|true|none||none|
|» update_time|string|true|none||none|

## GET hello::schema::operation

GET /api/hello/schema/operation

> 返回示例

```json
{
  "_id": "672250044f9dce055e0de3d3",
  "action": "test action",
  "bucket_name": "test bucket",
  "current_state": "test current state",
  "description": "test description",
  "object_name": "test object name",
  "previous_state": "test previous state",
  "request_ip": "127.0.0.1",
  "timestamp": 1730301956,
  "user_id": "672250044f9dce055e0de3d4"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» _id|string|true|none||none|
|» action|string|true|none||none|
|» bucket_name|string|true|none||none|
|» current_state|string|true|none||none|
|» description|string|true|none||none|
|» object_name|string|true|none||none|
|» previous_state|string|true|none||none|
|» request_ip|string|true|none||none|
|» timestamp|integer|true|none||none|
|» user_id|string|true|none||none|

## GET hello::schema::data_source

GET /api/hello/schema/data_source

> 返回示例

```json
{
  "_id": "67225099cd4e3029ba0e3524",
  "access_key": "test access key",
  "create_time": "test create time",
  "name": "test name",
  "secret_key": "test secret key",
  "url": "test url"
}
```

### 返回结果

|状态码|状态码含义|说明|数据模型|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|none|Inline|

### 返回数据结构

状态码 **200**

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|» _id|string|true|none||none|
|» access_key|string|true|none||none|
|» create_time|string|true|none||none|
|» name|string|true|none||none|
|» secret_key|string|true|none||none|
|» url|string|true|none||none|

# 数据模型

<h2 id="tocS_Pet">Pet</h2>

<a id="schemapet"></a>
<a id="schema_Pet"></a>
<a id="tocSpet"></a>
<a id="tocspet"></a>

```json
{
  "id": 1,
  "category": {
    "id": 1,
    "name": "string"
  },
  "name": "doggie",
  "photoUrls": [
    "string"
  ],
  "tags": [
    {
      "id": 1,
      "name": "string"
    }
  ],
  "status": "available"
}

```

### 属性

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|id|integer(int64)|true|none||宠物ID编号|
|category|[Category](#schemacategory)|true|none||分组|
|name|string|true|none||名称|
|photoUrls|[string]|true|none||照片URL|
|tags|[[Tag](#schematag)]|true|none||标签|
|status|string|true|none||宠物销售状态|

#### 枚举值

|属性|值|
|---|---|
|status|available|
|status|pending|
|status|sold|

<h2 id="tocS_Category">Category</h2>

<a id="schemacategory"></a>
<a id="schema_Category"></a>
<a id="tocScategory"></a>
<a id="tocscategory"></a>

```json
{
  "id": 1,
  "name": "string"
}

```

### 属性

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|id|integer(int64)|false|none||分组ID编号|
|name|string|false|none||分组名称|

<h2 id="tocS_Tag">Tag</h2>

<a id="schematag"></a>
<a id="schema_Tag"></a>
<a id="tocStag"></a>
<a id="tocstag"></a>

```json
{
  "id": 1,
  "name": "string"
}

```

### 属性

|名称|类型|必选|约束|中文名|说明|
|---|---|---|---|---|---|
|id|integer(int64)|false|none||标签ID编号|
|name|string|false|none||标签名称|

