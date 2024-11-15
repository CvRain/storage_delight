// alova.ts
import { createAlova,StatesHook } from 'alova';
// import adapterFetch from 'alova/fetch';

import { axiosRequestAdapter } from '@alova/adapter-axios';
import ReactHook from 'alova/react';


const alovaInstance = createAlova({
  baseURL: process.env.TEST_API_URL || 'http://www.kursharp.cn:21654/api',
  requestAdapter: axiosRequestAdapter(),
  statesHook: ReactHook,
  // responded: response => response
  responded:{
    // response自动被推断为AxiosResponse类型
    onSuccess(response) {
      // response自动被推断为AxiosResponse类型
      return response.data;
    },
    onError(err) {
      return err;
    }
  }
});

export default alovaInstance;