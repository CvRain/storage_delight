'use client'
import { useState,useRe,useEffect } from "react";
import { Input } from "antd";
export default function Client() {
    const [inputVal, setinputVal] = useState('')

    return (
        <div className="todolist-header">
            {inputVal}
            <Input placeholder="Basic usage" size="large" value={inputVal} />
        </div>
    );
}