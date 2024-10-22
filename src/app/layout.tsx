import React from 'react';
import { AntdRegistry } from '@ant-design/nextjs-registry';
import { Layout } from "antd";

const { Header, Content, Footer } = Layout;

const RootLayout = ({ children }: React.PropsWithChildren) => (
  <html lang="en">
    <body>
      <AntdRegistry>
          <Layout>
          <Header>
            header
          </Header>
          <Content>
            {children}
          </Content>
          <Footer>
            footer
          </Footer>
        </Layout>
      </AntdRegistry>
    </body>
  </html>
);

export default RootLayout;