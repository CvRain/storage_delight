"use server";

import React, { use } from 'react';

const fetchData = async () => {
  const response = await fetch(`${process.env.TEST_API_URL}/hello/say`);
  const data = await response.json();
  return data;
};

const fetchDataPost = async () => {
  const response = await fetch(`${process.env.TEST_API_URL}/hello/say`,);
  const data = await response.json();
  return data;
};

const Page:React.FC = () => {
  const data = use(fetchDataPost());

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