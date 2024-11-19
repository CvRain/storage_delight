'use client'
import { useState,useRef,useEffect } from "react";
import { useSelector, useDispatch } from 'react-redux';
import { 
  SettingOutlined,
  HomeOutlined,
  LoginOutlined,
  PayCircleOutlined,
  CustomerServiceOutlined, 
  FileSearchOutlined } from '@ant-design/icons';
import type { MenuProps } from 'antd';
import { Menu,Image,Button } from 'antd';
import "./index.css"
import Link from "next/link";

const items: MenuItem[] = [
  {
    label: '主页',
    key: 'home',
    icon: <HomeOutlined />,
  },
  {
    label: '文档',
    key: 'document',
    icon: <FileSearchOutlined />,
  },
  {
    label: '定价',
    key: 'mail',
    icon: <PayCircleOutlined />,
  },
  {
    label: '支持与服务',
    key: 'service',
    icon: <CustomerServiceOutlined />,
    disabled: true,
    children: [
      {
        type: 'group',
        label: '',
        children: [
          { label: 'x64版本', key: 'sf' },
          { label: 'Option 2', key: 'setting:2' },
        ],
      },
      {
        type: 'group',
        label: 'Item 2',
        children: [
          { label: 'Option 3', key: 'setting:3' },
          { label: 'Option 4', key: 'setting:4' },
        ],
      },
    ],
  },
  // {
  //   key: 'alipay',
  //   label: (
  //     <a href="https://ant.design" target="_blank" rel="noopener noreferrer">
  //       Navigation Four - Link
  //     </a>
  //   ),
  // },
];

type MenuItem = Required<MenuProps>['items'][number];

export default function Client({
    show,
  }: {
    show?: boolean;
  }) {
    // // 模拟 componentDidMount 和 componentWillUnmount
    useEffect(() => {
      console.log('Component mounted');
  
      // 清理操作，模拟 componentWillUnmount
      return () => {
        console.log('Component will unmount');
      };
    }, []); 

    const [current, setCurrent] = useState('mail');

    const onClick: MenuProps['onClick'] = (e) => {
      console.log('click ', e);
      setCurrent(e.key);
    };
  
    return (
        <div className="header">
          <div className="header-frame">
            <div className="header-menu">
              <Menu 
              onClick={onClick} 
              selectedKeys={[current]} 
              mode="horizontal" 
              items={items} 
              expandIcon={<SettingOutlined />}
            />
            </div>
            <div className="header-operation">
              <Button type="primary" size="large" icon={<LoginOutlined />}>
                <Link href="/storageDelight/login">
                  登录
                </Link>
              </Button>
            </div>
          </div>
        </div>
    );
}