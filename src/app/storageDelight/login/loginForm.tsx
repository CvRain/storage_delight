"use client";
import React, { use } from 'react';
import alovaInstance from '@ut/alova';
import { useState,useRef,useEffect } from "react";
import { LoginInfo } from '@type/loginInfo';
import { Card,Input,Button } from 'antd';
import Link from 'next/link';

import { useSelector, useDispatch } from 'react-redux';
import { RootState } from '@st/store';
import { changeHeaderShow,changeFooterShow,changeSiderShow } from '@st/reducers/layout';

const fetchData = async () => {
  const response = await alovaInstance.Get('/hello/say');
  return response
};

const LoginForm:React.FC = () => {
  
    const [loginInfo, setLoginInfo] = useState<LoginInfo>({

    })

    const dispatch = useDispatch();
    
    // // 模拟 componentDidMount 和 componentWillUnmount
    useEffect(() => {
        dispatch(changeHeaderShow(false));
        dispatch(changeFooterShow(false));
        dispatch(changeSiderShow(false));
        console.log('Component mounted');

        // 清理操作，模拟 componentWillUnmount
        return () => {
            dispatch(changeHeaderShow(true));
            dispatch(changeFooterShow(true));
            dispatch(changeSiderShow(true));
            console.log('Component will unmount');
        };

    }, []); 

    return (
        <div className="storageDelight-loginform">
            <div className="storageDelight-loginform-card">
            <Card title="登录" extra={<Link href="#">注册</Link>}>
                <div className="storageDelight-loginform-card-name">
                    <Input defaultValue="26888888" />
                </div>
                <div className="storageDelight-loginform-card-password">
                    <Input defaultValue="26888888" />
                </div>
                <div className="storageDelight-loginform-card-btn">
                    <Button type="primary">登录</Button>
                </div>
            </Card>
            </div>
        </div>
    );
};

export default LoginForm;