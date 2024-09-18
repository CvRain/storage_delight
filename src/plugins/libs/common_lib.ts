import { App } from 'vue'
import * as base from 'js-base64'

export const DeepClone = (value:any)=>{
    return JSON.parse(JSON.stringify(value))
}
export const PageRefresh = ()=>{
    location.reload();
}
export const ToUtf8 = (str:string) => {
    let out, i, len, c;
    out = "";
    len = str.length;
    for(i = 0; i < len; i++) {
        c = str.charCodeAt(i);
        if ((c >= 0x0001) && (c <= 0x007F)) {
            out += str.charAt(i);
        } else if (c > 0x07FF) {
            out += String.fromCharCode(0xE0 | ((c >> 12) & 0x0F));
            out += String.fromCharCode(0x80 | ((c >>  6) & 0x3F));
            out += String.fromCharCode(0x80 | ((c >>  0) & 0x3F));
        } else {
            out += String.fromCharCode(0xC0 | ((c >>  6) & 0x1F));
            out += String.fromCharCode(0x80 | ((c >>  0) & 0x3F));
        }
    }
    return out;
}
export const CheckMail = (mailstr:string)=>{
    let regEmail = /^[A-Za-z0-9\u4e00-\u9fa5]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$/;
    return regEmail.test(mailstr);
}
export const CheckPhone = (phonestr:string)=>{
    let regPhone = /^((1[3,5,8,7,9][0-9])|(14[5,7])|(17[0,6,7,8])|(19[7]))\d{8}$/;
    return regPhone.test(phonestr);
}
export const CheckReg = (reg:string,str:string)=>{
    let regexpobj = new RegExp(reg);
    return regexpobj.test(str);
}
export const NumberStringInt = (num_x:number,num_y:number)=>{
    return parseInt(num_x.toString() + num_y.toString())
}