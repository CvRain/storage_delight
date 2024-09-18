
import { App } from 'vue'
import * as base from 'js-base64'

import { nanoid } from 'nanoid';
import { createAlova, useRequest } from 'alova'
import GlobalFetch from 'alova/GlobalFetch';
import VueHook from 'alova/vue';

export const alovaInstance = createAlova({
    baseURL: import.meta.env.VITE_API_URL,
    statesHook: VueHook,
    requestAdapter: GlobalFetch(),
    async beforeRequest(method) {
      method.config.headers.token = `tk-${nanoid()}`;
    },
  
    responded: {
          // 请求成功的拦截器
          // 当使用GlobalFetch请求适配器时，第一个参数接收Response对象
          // 第二个参数为当前请求的method实例，你可以用它同步请求前后的配置信息
          onSuccess: async (response, method) => {
            if (response.status >= 400) {
                throw new Error(response.statusText);
            }
            const json = await response.json();
            if (!json.code) {
                // 抛出错误或返回reject状态的Promise实例时，此请求将抛出错误
                throw new Error(json.message);
            }
    
            // 解析的响应数据将传给method实例的transformData钩子函数，这些函数将在后续讲解
            return json;
          },

  
          // 请求失败的拦截器
          // 请求错误时将会进入该拦截器。
          // 第二个参数为当前请求的method实例，你可以用它同步请求前后的配置信息
          onError: (err, method) => {
            console.log(err.message);
          },
      }
  })


// const TemplateReq = (postData:any) => {
//     return alovaInstance.Post('/path',postData,{
//         headers: {
//         'Content-Type': 'application/json;charset=UTF-8'
//         },
//         params: {
//             //Get请求用的，在url后用?加的参数
//         },
//         timeout: 10000,//请求超时时间
//     });
// }
// const {
//     send:PostSend,
//     onSuccess,
//     onError,
//     onComplete
// } = useRequest(LoginReq, {
//     // 默认发送请求
//     immediate:false,
//     // 请求响应前，data的初始值
//     initialData: []
// });

// onSuccess(req => {
//     console.log('请求成功，响应数据为:', req.data);
//     console.log('本次请求的method实例为:', req.method);
//     console.log('响应数据是否来自缓存:', req.fromCache);
// });

// onError(req => {
//     console.log(req.data);
// });

// PostSend({
//     data:'test',
// })