"use client";
import React, { use } from 'react';
import alovaInstance from '@ut/alova';
import { useRequest } from 'alova/client';
import { useState,useRef,useEffect } from "react";
import { LoginInfo } from '@type/loginInfo';
import { Card,Input,Button } from 'antd';
import Link from 'next/link';

import { useSelector, useDispatch } from 'react-redux';
import { RootState } from '@st/store';
import { changeHeaderShow,changeFooterShow,changeSiderShow } from '@st/reducers/layout';

const LoginForm:React.FC = () => {
    const [loginName, setLoginName] = useState<string>('')
    const [loginPassword, setLoginPassword] = useState<string>('')
    const [loginInfo, setLoginInfo] = useState<LoginInfo>({
        name:'',
        password: '',
        role:0,
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

    
    const addUserRequest = useRequest(
        (loginData: LoginInfo) => alovaInstance.Put('/user/add', loginData),
        { immediate: false }
    );

    const loginRequest = useRequest(
        (loginData: LoginInfo) => alovaInstance.Put('/user/login', loginData),
        { immediate: false }
    );

    addUserRequest.onSuccess((response) => {
        console.log('add successful:', response);
    });

    addUserRequest.onError((response) => {
        console.log('add failed:', response);
    });


    loginRequest.onSuccess((response) => {
        console.log('Login successful:', response);
    });

    loginRequest.onError((error) => {
        console.error('Login failed:', error);
    });

    const login: React.MouseEventHandler<HTMLButtonElement> = (event)=>{
        console.log(event);
        setLoginInfo({
            name:loginName,
            password:loginPassword,
            role:loginInfo.role
        })
        // 发送请求
        loginRequest.send(loginInfo);
    }
    
    const nameInputChange = (event) => {
        console.log(event);
        setLoginName(event.target.value);
    }
    const passwordInputChange = (event) => {
        console.log(event);
        setLoginPassword(event.target.value);
    }

    return (
        <div className="storageDelight-loginform">
            <Card title="登录" extra={<Link  href="/storageDelight/register">注册</Link>}>
                <div className="storageDelight-loginform-card">
                    <div className="storageDelight-loginform-card-name">
                        <Input
                            id="name"
                            value={loginName}
                            onChange={nameInputChange}
                            placeholder="请输入用户名"
                        />
                    </div>
                    <div className="storageDelight-loginform-card-password">
                        <Input.Password
                            id="password"
                            value={loginPassword}
                            onChange={passwordInputChange}
                            placeholder="请输入密码"
                        />
                    </div>
                    <div className="storageDelight-loginform-card-btn">
                        <Button type="primary" size="large" onMouseUp={login} block>登录</Button>
                    </div>
                </div>
            </Card>
        </div>
    );
};

export default LoginForm;