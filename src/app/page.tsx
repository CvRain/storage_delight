import React, { use } from 'react';
import alovaInstance from '@ut/alova';

const getData = () => {
  const response = alovaInstance.Get('/hello/say');
  console.log(response)
  return response
};

const App: React.FC = () => {
    const data = use(getData());

  
    return (
      <div>
        <div className="storageDelight-index">
          <div className="storageDelight-index-"> 
  
          </div>
        </div>
        <h2>Alova Test</h2>
        <pre>{JSON.stringify(data, null, 2)}</pre>
      </div>
    );
}

export default App;
