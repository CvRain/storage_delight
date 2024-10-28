// alova.ts
import { createAlova } from 'alova';

import adapterFetch from 'alova/fetch';

// import { axiosRequestAdapter } from '@alova/adapter-axios';

const alovaInstance = createAlova({
  baseURL: process.env.TEST_API_URL || 'http://www.kursharp.cn:21654/api',
  requestAdapter: adapterFetch(),
  responded: response => response.json()
});

export default alovaInstance;