<template>
    <div class="index">
        <div class="index-frame">
            <div class="index-banner">

            </div>
        </div>
    </div>
</template>

<script lang="ts" setup>
import { createAlova, useRequest } from 'alova'
import dayjs from 'dayjs';
import { onMounted,onUpdated, onBeforeMount,onBeforeUnmount,onUnmounted,onServerPrefetch} from 'vue';
import { ref,reactive,readonly,defineComponent,h} from 'vue';
import { computed,watch,watchEffect} from 'vue';
import {getCurrentInstance,inject,provide} from 'vue';
import { Ref,ComputedRef,Component} from 'vue';
import {useRouter,Router} from 'vue-router';
import useStore from '@st/store';
import {storeToRefs} from 'pinia'
import * as echarts from 'echarts';
import { alovaInstance } from '@libs/alova_lib'
import { useCookies } from "vue3-cookies";
import {
  FormInst,
  FormItemInst,
  FormItemRule,
  useMessage,
  FormRules
} from 'naive-ui'
// const {} = useStore()
const router:Router = useRouter()
const server:string | undefined = inject('SERVER');
const {user,userInfo} = useStore()
const message = useMessage()
const { cookies } = useCookies();

onBeforeMount(()=>{
    if(!userInfo.userInfo?.id&&user.user.id){
        UserInfoCheckAndSet(user.user.id)
    }
})


onBeforeMount(()=>{
    if(!user.user.token){
        user.IsLoginAction(false)
        if(cookies.isKey("userToken")){
            UserTokenLogin()
        }
    }
    
})
const UserTokenLogin = async ():Promise<void>=>{
    let token = cookies.get('userToken')

    const LoginReq = (loginToken:string) => {
        return alovaInstance.Post('/login/LoginToken',{loginToken:loginToken},{
            headers: {
                'Content-Type': 'application/json;charset=UTF-8'
            },
            params: {
                //Get请求用的，在url后用?加的参数
            },
            timeout: 10000,//请求超时时间
        });
    }
    const {
        send:LoginSend,
        onSuccess,
        onError,
        onComplete
    } = useRequest(LoginReq, {
        // 默认发送请求
        immediate:false,
        // 请求响应前，data的初始值
        initialData: []
    });

    onError((req:any) => {
        message.error('发生未知错误',
        {
            keepAliveOnHover: true
        })
    });
    onSuccess((req:any) => {
        let requestData = req.data.data
        if(requestData.code==20020){
            user.IsLoginAction(true)
            userInfo.UserInfoAction(requestData.data.loginUserInfoResult)
            user.UserAction(requestData.data.loginUserResult)
        }else{
            message.error(requestData.msg)
        }
    });

    LoginSend(token)
}
const UserInfoCheckAndSet = async (userId:number):Promise<void>=>{
         //send需要设置不自动发送，不然会发送两次请求触发两次onsuccess，onerror
        const UserInfoCheckAndSetReq = (userId:number) => {
            return alovaInstance.Put('/register/InsertUserInfo',{userId:userId},{
                headers: {
                'Content-Type': 'application/json;charset=UTF-8'
                },
                params: {
                    //Get请求用的，在url后用?加的参数
                },
                timeout: 10000,//请求超时时间
            });
        }
        const {
            send:UserInfoCheckAndSetReqSend,
            onSuccess,
            onError,
            onComplete
        } = useRequest(UserInfoCheckAndSetReq, {
            // 默认发送请求
            immediate:false,
            // 请求响应前，data的初始值
            initialData: {}
        });
        onError(err => {
            message.error('发生未知错误',
            {
                keepAliveOnHover: true
            })
        });
        onSuccess((req:any) => {
            let requestData = req.data.data
        })

        UserInfoCheckAndSetReqSend(userId)        
}
</script>

<style lang="scss" scoped>

</style>