'use client'
import { useState,useRef,useEffect } from "react";
import store from '@st/store'; // 确保路径正确

export default function Client({
    show,
  }: {
    show: boolean;
  }) {
    // const [showFooter, setShowFooter] = useState(show)

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
    //   console.log(`Count updated to ${showFooter}`);
    // }, [show]);

    return ( 
        <div className="footer">
            {
                show && (
                    <div className="footer-frame">
                        footer
                    </div>
                )
            }
            {show}
        </div>
    );
}