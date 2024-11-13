'use client';
import React from 'react';
import { Layout } from 'antd';

import WebHeader from '@com/storageDelight/WebHeader';
import WebFooter from '@com/storageDelight/WebFooter';
import WebSider from '@com/storageDelight/WebSider';

import { useSelector, useDispatch } from 'react-redux';
import { RootState } from '@st/store';

const { Header, Content, Footer, Sider } = Layout;

const WebLayout = ({ children }: React.PropsWithChildren) => {
  const {headerShow,footerShow,siderShow} = useSelector((state: RootState) => state.layoutReducer);

  const headerStyle: React.CSSProperties = {
    textAlign: 'center',
    backgroundColor:'#fff',
  };

  const contentStyle: React.CSSProperties = {
    padding: '20px',
    backgroundColor:'#fff',
    height:'100%',
  };

  const siderStyle: React.CSSProperties = {
    textAlign: 'center',
    color: '#fff',
    backgroundColor:'#fff',
    height:'100%',
  };

  const footerStyle: React.CSSProperties = {
    textAlign: 'center',
    backgroundColor:'#fff',
  };

  const layoutStyle = {
    overflow: 'hidden',
    width: '100vw',
  };

  return (
    <Layout style={layoutStyle}>
        {siderShow && (
        <Sider style={siderStyle}>
            <WebSider />
        </Sider>
        )}
        <Layout>
        {headerShow && (
        <Header style={headerStyle}>
            <WebHeader />
        </Header>
        )}
        <Content style={contentStyle}>
            {children}
        </Content>
        {footerShow && (
        <Footer style={footerStyle}>
            <WebFooter />
        </Footer>
        )}
        </Layout>
    </Layout>
  );
};

export default WebLayout;