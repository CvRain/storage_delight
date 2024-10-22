'use client'
import { useState,useRef } from "react";
export default function Client({
    inputServerFetchData,
  }: {
    inputServerFetchData:number,
  }) {
    const [inputData, setinputServerFetchData] = useState(inputServerFetchData)
    const inputEl = useRef<HTMLInputElement>(null);
    
    const changeAge: React.MouseEventHandler<HTMLButtonElement> = (event)=>{
        console.log(event,inputEl);
        setinputServerFetchData(123)
    }
    

    return (
      <div ref={inputEl} className="flex w-full flex-col md:col-span-4">
        <h2 className='main'>
          client {inputData}
          <button  onClick={changeAge}>改变</button>
        </h2>
      </div>
    );
}