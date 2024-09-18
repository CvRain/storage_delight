import { App } from 'vue';

export const SetCookie = (cname:string,cvalue:any,exdays:number = 1) =>{
    let d = new Date();
    d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
    let expires = "expires=" + d.toUTCString();
    document.cookie = cname + "=" + cvalue + "; " + expires;
}
export const GetCookie = (cname:string) =>{
    let name = cname + "=";
    let ca = document.cookie.split(';');
    for (let i = 0; i < ca.length; i++) {
        let c = ca[i];
        while (c.charAt(0) == ' ') c = c.substring(1);
        if (c.indexOf(name) != -1) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}
export const ClearCookie = (cname:string) =>{
    let d = new Date();
    d.setTime(-1);
    let expires = "expires=" + d.toUTCString();
    document.cookie = cname + "=''; " + expires;
}
