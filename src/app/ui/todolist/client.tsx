'use client'
import { useState,useRef } from "react";
import Header from './../../../components/todolist/Header';
export default function Client({
    a,
  }: {
    a: number,
  }) {
    const [age, setAge] = useState(a)
    const inputEl = useRef<HTMLInputElement>(null);
    
    const changeAge: React.MouseEventHandler<HTMLButtonElement> = (event)=>{
        console.log(event,inputEl);
        setAge(123)
    }
    

    return (
      <div>
        <div className="todolist">
            <Header />
            <div className="todolist-checklist">

            </div>
            <div className="todolist-footer">

            </div>
        </div>
      </div>
    );
}