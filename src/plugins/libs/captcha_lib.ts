import { App } from 'vue'

export const CaptchaBuild = (captchaLength:number)=>{
    return Math.random().toFixed(captchaLength).slice(-captchaLength)
}
export const RegHaveUppercase = (str:string)=>{
    let regHaveUppercase=/[A-Z]+/;
    return regHaveUppercase.test(str)
}
export const RegHaveSymbol = (str:string)=>{
    let regHaveSymbol=/[.@!%*#_~?&$]+/;
    return regHaveSymbol.test(str)
}
export const RegHaveNumber = (str:string)=>{
    let regHaveNumber=/[0-9]+/;
    return regHaveNumber.test(str)
}
export const RegHaveLowercase = (str:string)=>{
    let regHaveLowercase=/[a-z]+/;
    return str.length>0?regHaveLowercase.test(str):false
}
export const RegHaveChinese = (str:string)=>{
    let regHaveChinese=/[\u4e00-\u9fa5]+/;
    return regHaveChinese.test(str)
}
export const RegHaveLetter = (str:string)=>{
    let regHaveLetter=/^[A-Za-z]+/
    return regHaveLetter.test(str)
}
export const RegHaveLetterAndNumber = (str:string)=>{
    let regHaveLetterAndNumber=/^[A-Za-z0-9]+/
    return regHaveLetterAndNumber.test(str)
}

export const RegIsLetter = (str:string)=>{
    let regIsLetter=/^[a-zA-Z]+$/
    return regIsLetter.test(str)
}
export const RegIsNumber = (str:string)=>{
    let regIsNumber=/^[0-9]+$/
    return regIsNumber.test(str)
}
export const RegIsLetterAndNumber = (str:string)=>{
    let regHaveLetterAndNumber=/^[a-zA-Z0-9]+$/
    return regHaveLetterAndNumber.test(str)
}
export const RegIsPhoneNumber = (str:string)=>{
    let regIsPhoneNumber=/^1(3[0-9]|5[0-3,5-9]|7[1-3,5-8]|8[0-9])\d{8}$/
    return regIsPhoneNumber.test(str)
}
export const RegIsNumberAndLetterAndChinese = (str:string)=>{
    let regIsPhoneNumber=/^1(3[0-9]|5[0-3,5-9]|7[1-3,5-8]|8[0-9])\d{8}$/
    return regIsPhoneNumber.test(str)
}
