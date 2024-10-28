'use client'
import { useState,useRef,useEffect } from "react";
import { useSelector, useDispatch } from 'react-redux';
import {RootState} from '@st/store';

const useSiderValue = () => {
  return useSelector((state: RootState) => state.globalReducer.siderShow)
};
export default function Client({
    show,
  }: {
    show?: boolean;
  }) {
    
    const siderShow = useSiderValue()
    // const [showSider, setShowSider] = useState(show)

    // // 模拟 componentDidMount 和 componentWillUnmount
    // useEffect(() => {
    //   console.log('Component mounted');
  
    //   // 清理操作，模拟 componentWillUnmount
    //   return () => {
    //     console.log('Component will unmount');
    //   };
    // }, []); // 空数组作为依赖项，确保只在组件挂载时执行一次
  
    // // 模拟 componentDidUpdate
    // useEffect(() => {
    //   console.log(`Count updated to ${showSider}`);
    // }, [showSider]); // 当 count 改变时重新执行

    return (
        <div className="sider" style={{ display: siderShow ? 'block' : 'none' }}>
          {
              siderShow && (
              <div className="sider-frame">
                  sider
              </div>
              )
          }
        </div>
    );
}