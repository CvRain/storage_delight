"use server";
import React, { use } from 'react';
import alovaInstance from '@ut/alova';
import LoginForm from './loginForm';
import "./page.css"

const Page:React.FC = () => {

  return (
    <div className="storageDelight-login">
        <LoginForm></LoginForm>
    </div>
  );
};

export default Page;