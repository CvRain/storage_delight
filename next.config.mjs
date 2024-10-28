/** @type {import('next').NextConfig} */
const nextConfig = {
    reactStrictMode: true,
    // env: {
    //     API_URL: 'http://www.kursharp.cn:21654/api', // 环境变量
    // },
    async rewrites() {
      return [
        {
          source: '/api/:path*', // 本地 API 路由
          destination: 'http://www.kursharp.cn:21654/api/:path*', // 代理到外部 API
        },
      ];
    },
    // images: {
    //   domains: ['example.com'], // 允许的图片域名
    // },
    // i18n: {
    //   locales: ['en-US', 'fr', 'es'], // 支持的语言
    //   defaultLocale: 'en-US', // 默认语言
    // },
};

export default nextConfig;
