'use client';
import React from 'react';
import { AntdRegistry } from '@ant-design/nextjs-registry';
import { Provider } from 'react-redux';
// import { useSelector, useDispatch } from 'react-redux';
import { Layout } from "antd";

import "@ys/global.css"

import WebHeader from "@com/storageDelight/WebHeader"
import WebFooter from "@com/storageDelight/WebFooter"
import WebSider from "@com/storageDelight/WebSider"

import store from '@st/store'; // 确保路径正确
import {RootState} from '@st/store';

const { Header, Content, Footer, Sider } = Layout;

const useGlobalValue = () => {
  const globalVal = {
    // headshow:useSelector((state: RootState) => state.globalReducer.headerShow),
    // siderShow:useSelector((state: RootState) => state.globalReducer.siderShow),
    // footerShow:useSelector((state: RootState) => state.globalReducer.footerShow),
    headshow:true,
    siderShow:true,
    footerShow:true,
  }
  return globalVal;
};

const RootLayout = ({ children }: React.PropsWithChildren) => {
  const {headshow,siderShow,footerShow} = useGlobalValue()

  const headerStyle: React.CSSProperties = {
    textAlign: 'center',
  };
  
  const contentStyle: React.CSSProperties = {
    padding:'20px'
  };
  
  const siderStyle: React.CSSProperties = {
    textAlign: 'center',
    color: '#fff',
  };
  
  const footerStyle: React.CSSProperties = {
    textAlign: 'center',
  };
  
  const layoutStyle = {
    overflow: 'hidden',
    width: '100vw',
  };

  return (
    <html lang="en">
      <body>
        <Provider store={store}>
          <AntdRegistry>
              <Layout style={layoutStyle}>
                <Header style={headerStyle}>
                  <WebHeader show={headshow?headshow:false} />
                </Header>
                <Layout>
                  <Sider width="15%" style={siderStyle}>
                    <WebSider show={siderShow?siderShow:false} />
                  </Sider>
                  <Content style={contentStyle}>
                    {children}
                  </Content>
                </Layout>
                <Footer style={footerStyle}>
                  <WebFooter show={footerShow?footerShow:false} />
                </Footer>
              </Layout>
          </AntdRegistry>
        </Provider>
      </body>
    </html>
  )
};

export default RootLayout;