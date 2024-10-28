'use client';
import React from 'react';
import { AntdRegistry } from '@ant-design/nextjs-registry';
import { Provider } from 'react-redux';

import '@ys/global.css';

import WebLayout from '@com/storageDelight/WebLayout';

import store from '@st/store'; // 确保路径正确

const RootLayout = ({ children }: React.PropsWithChildren) => {

  return (
    <Provider store={store}>
    <html lang="en">
      <body>
          <AntdRegistry>
            <WebLayout>{children}</WebLayout>
          </AntdRegistry>
      </body>
    </html>
    </Provider>
  );
};

export default RootLayout;