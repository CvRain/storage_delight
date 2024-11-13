"use server";
import React, { use } from 'react';
import alovaInstance from '@ut/alova';
import "./page.css"
import RegisterForm from './registerForm';

const fetchData = async () => {
  const response = await alovaInstance.Get('/hello/say');
  return response
};

const Page:React.FC = () => {

  return (
    <div className="storageDelight-login">
        <RegisterForm></RegisterForm>
    </div>
  );
};

export default Page;