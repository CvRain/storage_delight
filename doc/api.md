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
  "user_name": "string",
  "user_password": "string",
  "role": 0
}
```

### 请求参数

| 名称              | 位置   | 类型      | 必选 | 说明   |
|-----------------|------|---------|----|------|
| body            | body | object  | 否  | none |
| » user_name     | body | string  | 是  | none |
| » user_password | body | string  | 是  | none |
| » role          | body | integer | 是  | none |

> 返回示例

> 200 Response

```json
{
  "code": 0,
  "message": "string",
  "result": "string",
  "users": [
    {
      "create_time": "string",
      "id": 0,
      "password": "string",
      "role": 0,
      "user_name": "string"
    }
  ]
}
```

### 返回结果

| 状态码 | 状态码含义                                                   | 说明   | 数据模型   |
|-----|---------------------------------------------------------|------|--------|
| 200 | [OK](https://tools.ietf.org/html/rfc7231#section-6.3.1) | none | Inline |

### 返回数据结构

状态码 **200**

| 名称             | 类型       | 必选    | 约束   | 中文名 | 说明   |
|----------------|----------|-------|------|-----|------|
| » code         | integer  | true  | none |     | none |
| » message      | string   | true  | none |     | none |
| » result       | string   | true  | none |     | none |
| » users        | [object] | true  | none |     | none |
| »» create_time | string   | false | none |     | none |
| »» id          | integer  | false | none |     | none |
| »» password    | string   | false | none |     | none |
| »» role        | integer  | false | none |     | none |
| »» user_name   | string   | false | none |     | none |

## GET user::find_by_id

GET /api/user/id/

### 请求参数

| 名称            | 位置     | 类型     | 必选 | 说明   |
|---------------|--------|--------|----|------|
| user_id       | query  | string | 是  | none |
| Authorization | header | string | 否  | none |

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

| 状态码 | 状态码含义                                                   | 说明   | 数据模型   |
|-----|---------------------------------------------------------|------|--------|
| 200 | [OK](https://tools.ietf.org/html/rfc7231#section-6.3.1) | none | Inline |

### 返回数据结构

## POST user::login

POST /api/user/login

> Body 请求参数

```json
{}
```

### 请求参数

| 名称   | 位置   | 类型     | 必选 | 说明   |
|------|------|--------|----|------|
| body | body | object | 否  | none |

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

| 状态码 | 状态码含义                                                   | 说明   | 数据模型   |
|-----|---------------------------------------------------------|------|--------|
| 200 | [OK](https://tools.ietf.org/html/rfc7231#section-6.3.1) | none | Inline |

### 返回数据结构

状态码 **200**

| 名称          | 类型      | 必选   | 约束   | 中文名 | 说明   |
|-------------|---------|------|------|-----|------|
| » code      | integer | true | none |     | none |
| » message   | string  | true | none |     | none |
| » token     | string  | true | none |     | none |
| » user_id   | integer | true | none |     | none |
| » user_name | string  | true | none |     | none |

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

| 状态码 | 状态码含义                                                   | 说明   | 数据模型   |
|-----|---------------------------------------------------------|------|--------|
| 200 | [OK](https://tools.ietf.org/html/rfc7231#section-6.3.1) | none | Inline |

### 返回数据结构

状态码 **200**

| 名称        | 类型      | 必选   | 约束   | 中文名 | 说明   |
|-----------|---------|------|------|-----|------|
| » code    | integer | true | none |     | none |
| » message | string  | true | none |     | none |
| » result  | string  | true | none |     | none |

## GET hello::echo

GET /api/hello/echo

### 请求参数

| 名称      | 位置    | 类型     | 必选 | 说明   |
|---------|-------|--------|----|------|
| message | query | string | 否  | none |

> 返回示例

```json
{
  "code": 200,
  "message": "Jeffrey Hall",
  "result": "ok"
}
```

### 返回结果

| 状态码 | 状态码含义                                                   | 说明   | 数据模型   |
|-----|---------------------------------------------------------|------|--------|
| 200 | [OK](https://tools.ietf.org/html/rfc7231#section-6.3.1) | none | Inline |

### 返回数据结构

状态码 **200**

| 名称        | 类型      | 必选   | 约束   | 中文名 | 说明   |
|-----------|---------|------|------|-----|------|
| » code    | integer | true | none |     | none |
| » message | string  | true | none |     | none |
| » result  | string  | true | none |     | none |

## POST hello::hello

POST /api/hello/hello

> Body 请求参数

```yaml
text: "123"
file: file:///home/cvrain/图片/209_1710921879_hd.jpeg

```

### 请求参数

| 名称     | 位置   | 类型             | 必选 | 说明   |
|--------|------|----------------|----|------|
| body   | body | object         | 否  | none |
| » text | body | string         | 是  | none |
| » file | body | string(binary) | 是  | none |

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

| 状态码 | 状态码含义                                                   | 说明   | 数据模型   |
|-----|---------------------------------------------------------|------|--------|
| 200 | [OK](https://tools.ietf.org/html/rfc7231#section-6.3.1) | none | Inline |

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

| 名称      | 位置   | 类型     | 必选 | 说明   |
|---------|------|--------|----|------|
| body    | body | object | 否  | none |
| » name  | body | string | 是  | none |
| » id    | body | string | 是  | none |
| » email | body | string | 是  | none |

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

| 状态码 | 状态码含义                                                   | 说明   | 数据模型   |
|-----|---------------------------------------------------------|------|--------|
| 200 | [OK](https://tools.ietf.org/html/rfc7231#section-6.3.1) | none | Inline |

### 返回数据结构

状态码 **200**

| 名称        | 类型      | 必选   | 约束   | 中文名 | 说明   |
|-----------|---------|------|------|-----|------|
| » code    | integer | true | none |     | none |
| » message | string  | true | none |     | none |
| » result  | string  | true | none |     | none |

## POST hello::test_requets

POST /api/hello/test_request

> 返回示例

> 200 Response

```json
{}
```

### 返回结果

| 状态码 | 状态码含义                                                   | 说明   | 数据模型   |
|-----|---------------------------------------------------------|------|--------|
| 200 | [OK](https://tools.ietf.org/html/rfc7231#section-6.3.1) | none | Inline |

### 返回数据结构

# 数据模型

