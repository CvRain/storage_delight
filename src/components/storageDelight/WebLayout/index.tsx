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
  const {headerShow,footerShow,siderShow} = useSelector((state: RootState) => state.globalReducer);

  const headerStyle: React.CSSProperties = {
    textAlign: 'center',
  };

  const contentStyle: React.CSSProperties = {
    padding: '20px',
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
    <Layout style={layoutStyle}>
        {headerShow && (
        <Header style={headerStyle}>
            <WebHeader />
        </Header>
        )}
        <Layout>
        {siderShow && (
        <Sider width="15%" style={siderStyle}>
            <WebSider />
        </Sider>
        )}
        <Content style={contentStyle}>
            {children}
        </Content>
        </Layout>
        {footerShow && (
        <Footer style={footerStyle}>
            <WebFooter />
        </Footer>
        )}
    </Layout>
  );
};

export default WebLayout;