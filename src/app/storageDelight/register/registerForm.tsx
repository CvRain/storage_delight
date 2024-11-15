"use client";
import React, { use } from 'react';
import alovaInstance from '@ut/alova';
import { useRequest } from 'alova/client';
import { useState,useRef,useEffect } from "react";
import { RegisterInfo } from '@type/registerInfo';
import Link from 'next/link';

import { useSelector, useDispatch } from 'react-redux';
import { RootState } from '@st/store';
import { changeHeaderShow,changeFooterShow,changeSiderShow } from '@st/reducers/layout';

import { Card,Input,Button,Tooltip } from 'antd';
import {
    KeyOutlined,

  } from '@ant-design/icons';

const RegisterForm:React.FC = () => {
    const [registerName, setRegisterName] = useState<string>('')
    const [registerPassword, setRegisterPassword] = useState<string>('')
    const [registerPasswordConfirm, setRegisterPasswordConfirm] = useState<string>('')
    const [registerInfo, setRegisterInfo] = useState<RegisterInfo>({
        name:'',
        password: '',
        role:0,
    })

    const [passwordConditionWindow, setPasswordConditionWindow] = useState(false);
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
        (registerData: RegisterInfo) => alovaInstance.Put('/user/add', registerData),
        { immediate: false }
    );

    const registerRequest = useRequest(
        (registerData: RegisterInfo) => alovaInstance.Put('/user/register', registerData),
        { immediate: false }
    );

    addUserRequest.onSuccess((response) => {
        console.log('add successful:', response);
    });

    addUserRequest.onError((response) => {
        console.log('add failed:', response);
    });


    registerRequest.onSuccess((response) => {
        console.log('register successful:', response);
    });

    registerRequest.onError((error) => {
        console.error('register failed:', error);
    });

    const register: React.MouseEventHandler<HTMLButtonElement> = (event)=>{
        console.log(event);
        setRegisterInfo({
            name:registerName,
            password:registerPassword,
            role:registerInfo.role
        })
        // 发送请求
        registerRequest.send(registerInfo);
    }

    const nameInputChange = (event) => {
        console.log(event);
        setRegisterName(event.target.value);
    }
    const passwordInputChange = (event) => {
        console.log(event);
        setRegisterPassword(event.target.value);
        setPasswordConditionWindow(true)
    }
    const confirmPasswordInputChange = (event) => {
        console.log(event);
        setRegisterPasswordConfirm(event.target.value);
        setPasswordConditionWindow(true)
    }
    return (
        <div className="storageDelight-registerform">
            <Card title="注册" extra={<Link  href="/storageDelight/login">登录</Link>}>
                <div className="storageDelight-registerform-card">
                    <div className="storageDelight-registerform-card-name">
                        <Input
                            id="registerName"
                            value={registerName}
                            onChange={nameInputChange}
                            placeholder="请输入用户名"
                        />
                    </div>
                    <div className="storageDelight-registerform-card-password">
                        <Input.Password
                            id="registerPassword"
                            value={registerPassword}
                            placeholder="请输入密码"
                            onChange={passwordInputChange}
                            suffix={
                                <Tooltip  
                                    title="密码复杂度"
                                    open={passwordConditionWindow}
                                >
                                
                                </Tooltip>
                            }
                        />
                    </div>
                    <div className="storageDelight-registerform-card-password">
                        <Input
                            id="registerConfirmPassword"
                            onChange={confirmPasswordInputChange}
                            value={registerPasswordConfirm}
                            placeholder="请输入密码"
                        />
                    </div>
                    <div className="storageDelight-registerform-card-btn">
                        <Button type="primary" size="large" onMouseUp={register} block>注册</Button>
                    </div>
                </div>
            </Card>
        </div>
    );
};

export default RegisterForm;