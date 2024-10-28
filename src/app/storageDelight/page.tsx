"use server";

import React, { use } from 'react';
import alovaInstance from '@ut/alova';

const fetchData = async () => {
  const response = await alovaInstance.Get('/hello/say');
  return response
};

const fetchDataPost = async () => {
  const response = await fetch(`${process.env.TEST_API_URL}/hello/say`,);
  const data = await response.json();
  return data;
};

const Page:React.FC = () => {
  const data = use(fetchData());

  console.log(data)

  return (
    <div>
      <div className="storageDelight-index">
        <div className="storageDelight-index-"> 

        </div>
      </div>
      <h2>Data from Server Component</h2>
      <pre>{JSON.stringify(data, null, 2)}</pre>
    </div>
  );
};

export default Page;