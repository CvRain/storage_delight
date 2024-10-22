'use client'
import { useState,useRe,useEffect } from "react";

export default function Client({
    a,
  }: {
    a: number;
  }) {
    const [age, setAge] = useState(a)
    const [count, setCount] = useState(0);
    
    const inputEl = useRef<HTMLInputElement>(null);
    
    const changeAge: React.MouseEventHandler<HTMLButtonElement> = (event)=>{
        console.log(event,inputEl);
        setAge(123)
    }

    // 模拟 componentDidMount 和 componentWillUnmount
    useEffect(() => {
      console.log('Component mounted');
  
      // 清理操作，模拟 componentWillUnmount
      return () => {
        console.log('Component will unmount');
      };
    }, []); // 空数组作为依赖项，确保只在组件挂载时执行一次
  
    // 模拟 componentDidUpdate
    useEffect(() => {
      console.log(`Count updated to ${count}`);
    }, [count]); // 当 count 改变时重新执行

    return (
      <div ref={inputEl} className="flex w-full flex-col md:col-span-4">
        <h2 className='main'>
          client {age}
          <button  onClick={changeAge}>改变</button>
        </h2>
      </div>
    );
}