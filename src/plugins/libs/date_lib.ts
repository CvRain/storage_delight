//npm install dayjs
import { App } from 'vue'
import dayjs, { Dayjs } from 'dayjs'

export const GetDate = ()=>{
    return dayjs().format('YYYY-MM-DD HH:mm:ss');
};
export const GetDateUnix = ()=>{
    return Date.parse(new Date().toString());
};
export const GetDateUnixDayjs = ()=>{
    return dayjs().unix()
};
export const TimestampToDate = (unix:number | string) => {
    if(typeof unix !='number'){
        unix = parseInt(unix)
    }
    let date:Date = new Date(unix*1000);
    return `${date.getUTCFullYear()}-${date.getUTCMonth()+1}-${date.getUTCDate()} ${date.getUTCHours()}:${date.getUTCMinutes()}:${date.getUTCSeconds()}`;
}
export const TimestampToDate_13 = (unix:number | string) => {
    if(typeof unix !='number'){
        unix = parseInt(unix)
    }
    let date:Date = new Date(unix);
    return `${date.getUTCFullYear()}-${date.getUTCMonth()+1}-${date.getUTCDate()} ${date.getUTCHours()}:${date.getUTCMinutes()}:${date.getUTCSeconds()}`;
}
export const TimestampToDateYMD = (unix:number | string) => {
    if(typeof unix !='number'){
        unix = parseInt(unix)
    }
    let date:Date = new Date(unix*1000);
    return `${date.getUTCFullYear()}-${date.getUTCMonth()+1}-${date.getUTCDate()}`;
}
export const TimestampToDateYMD_13 = (unix:number | string) => {
    if(typeof unix !='number'){
        unix = parseInt(unix)
    }
    let date:Date = new Date(unix);
    return `${date.getUTCFullYear()}-${date.getUTCMonth()+1}-${date.getUTCDate()}`;
}
export const DatelesstimeFormatToText = (date:number | string) => {
    if(typeof date !='number'){
        date = parseInt(date)
    }
    let now:number = new Date().getTime();
    let time:Dayjs = dayjs(now-date)
    return dayjs(time).format('HH:mm:ss')
}

export const TimeFormatToText = (time:number | string) => {
    if(typeof time !='number'){
        time = parseInt(time)
    }

    return dayjs(time*1000).format('HH:mm:ss')
}

export const TimeFormatToText_13 = (time:number | string) => {
    if(typeof time !='number'){
        time = parseInt(time)
    }
    return dayjs(time).format('HH:mm:ss')
}
export const TimeFormatToDate = (time:number | string) => {
    if(typeof time !='number'){
        time = parseInt(time)
    }

    return dayjs(time*1000).format('YYYY-MM-DD')
}

export const TimeFormatToDate_13 = (time:number | string) => {
    if(typeof time !='number'){
        time = parseInt(time)
    }
    return dayjs(time).format('YYYY-MM-DD')
}
export const TimeFormatToDateText = (time:number | string) => {
    if(typeof time !='number'){
        time = parseInt(time)
    }

    return dayjs(time*1000).format('YYYY-MM-DD HH:mm:ss')
}

export const TimeFormatToDateText_13 = (time:number | string) => {
    if(typeof time !='number'){
        time = parseInt(time)
    }

    return dayjs(time).format('YYYY-MM-DD HH:mm:ss')
}
export const DateTimeToUnix = (date_time_str:string):number=>{
    let date:Dayjs = dayjs(date_time_str);
    let unix:number = date.unix();
    return unix
}