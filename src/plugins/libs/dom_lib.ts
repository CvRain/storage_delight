import {ref} from 'vue'
import { App } from 'vue'

export const ButtonBlur = (event:any)=>{
    event.target.blur();
}
export const IsPhoneWindow = (vue_obj:any) => {
    let obj = vue_obj
    if(obj.currentStyle){
        return parseFloat(obj.currentStyle.width.replace('px',''))<600?true:false;
    }else{
        return parseFloat(getComputedStyle(obj).width.replace('px',''))<600?true:false;
    }
}
export const DomHeightGet = (vue_obj:any) => {
    let obj = vue_obj
    if(obj.currentStyle){
        return obj.currentStyle.height;
    }else{
        return getComputedStyle(obj).height;
    }
}
export const DomWidthGet = (vue_obj:any) => {
    let obj = vue_obj
    if(obj.currentStyle){
        return obj.currentStyle.width;
    }else{
        return getComputedStyle(obj).width;
    }
}
export const DomWidthNumGet = (vue_obj:any) => {
    let obj = vue_obj
    if(obj.currentStyle){
        return parseFloat(obj.currentStyle.width.replace('px',''));
    }else{
        return parseFloat(getComputedStyle(obj).width.replace('px',''));
    }
}
export const DomHeightNumGet = (vue_obj:any) => {
    let obj = vue_obj
    if(obj.currentStyle){
        return parseFloat(obj.currentStyle.height.replace('px',''));
    }else{
        return parseFloat(getComputedStyle(obj).height.replace('px',''));
    }
}