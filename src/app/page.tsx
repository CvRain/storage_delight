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
      <div className="storageDelight-index">
        <div className="storageDelight-index-"> 

          

        </div>
      </div>
    );
}

export default App;
