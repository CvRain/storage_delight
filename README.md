![storage_delight_placeholder](img/storage_delight_placeholder.png)

# 存储乐事
一个简单的个人文件存储工具，旨在能否方便的存储个人文件并简单分享。  
能够构建一个私人的云存储服务，也可以实现一个小圈子文件分享站。

# 项目结构
- storage_delight:
  - application: 桌面客户端
  - core: 存储核心
  - server: 存储服务器
  - doc: 项目文档
  - vcpkg: 第三方库(sub module)

# 功能实现
## storage_delight::core
  - [x] BucketExists
  - [ ] ComposeObject
  - [ ] CopyObject
  - [x] DeleteBucketEncryption
  - [x] DeleteBucketLifecycle
  - [x] DeleteBucketNotification
  - [x] DeleteBucketPolicy
  - [x] DeleteBucketReplication
  - [x] DeleteBucketTags
  - [ ] DeleteObjectLockConfig
  - [ ] DeleteObjectTags
  - [ ] DisableObjectLegalHold
  - [ ] DownloadObject
  - [ ] EnableObjectLegalHold
  - [x] GetBucketEncryption
  - [x] GetBucketLifecycle
  - [x] GetBucketNotification
  - [x] GetBucketPolicy
  - [x] GetBucketReplication
  - [x] GetBucketTags
  - [ ] GetBucketVersioning
  - [ ] GetObject
  - [ ] GetObjectLockConfig
  - [ ] GetObjectProgress
  - [ ] GetObjectRetention
  - [ ] GetObjectTags
  - [ ] GetPresignedObjectUrl
  - [ ] GetPresignedPostFormData
  - [ ] IsObjectLegalHoldEnabled
  - [x] ListBuckets
  - [ ] ListenBucketNotification
  - [ ] ListObjects
  - [ ] MakeBucket
  - [ ] PutObject
  - [ ] PutObjectProgress
  - [ ] RemoveBucket
  - [ ] RemoveObject
  - [ ] RemoveObjects
  - [ ] SelectObjectContent
  - [ ] SetBucketEncryption
  - [ ] SetBucketLifecycle
  - [ ] SetBucketNotification
  - [ ] SetBucketPolicy
  - [ ] SetBucketReplication
  - [ ] SetBucketTags
  - [ ] SetBucketVersioning
  - [ ] SetObjectLockConfig
  - [ ] SetObjectRetention
  - [ ] SetObjectTags
  - [ ] StatObject
  - [ ] UploadObject
