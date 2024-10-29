"use server";
import React, { use } from 'react';
import alovaInstance from '@ut/alova';
import LoginForm from './loginForm';
import "./page.css"

const fetchData = async () => {
  const response = await alovaInstance.Get('/hello/say');
  return response
};

const Page:React.FC = () => {

  return (
    <div className="storageDelight-login">
        <div className="storageDelight-login-frame"> 
            <LoginForm></LoginForm>
        </div>
    </div>
  );
};

export default Page;