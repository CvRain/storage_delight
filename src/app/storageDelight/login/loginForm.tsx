"use client";
import React, { use } from 'react';
import alovaInstance from '@ut/alova';
import { useState,useRef,useEffect } from "react";
import { LoginInfo } from './../../../types/loginInfo';

const fetchData = async () => {
  const response = await alovaInstance.Get('/hello/say');
  return response
};

const Page:React.FC = () => {
  
    const [loginInfo, setLoginInfo] = useState<LoginInfo>({
        
    })

    return (
        <div className="storageDelight-login">
            <div className="storageDelight-login-frame"> 
                
            </div>
        </div>
    );
};

export default Page;