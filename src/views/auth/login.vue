<template>
    <div class="login">
        <div class="login-frame">
            <div class="login-card">
                <n-card size="huge">
                    <template #header>
                        <div class="login-card-header">
                            <div class="login-card-header-title">
                                登录
                            </div>
                            <div class="login-card-header-backhome">
                                <div @mouseup.stop="router.push({name:'index'})">返回主页</div>
                            </div>
                        </div>
                    </template>
                    <div class="login-form">
                        <template v-if="loginType===1">
                            <n-form ref="usernameLoginFormRef" :model="loginFormdata" :rules="usernameRules">
                                <n-form-item path="username" label="用户名">
                                    <n-input v-model:value="loginFormdata.username" @keydown.enter.prevent placeholder="用户名/手机" />
                                </n-form-item>
                                <n-form-item path="password" label="密码">
                                    <n-input
                                        v-model:value="loginFormdata.password"
                                        type="password"
                                        @keydown.enter.prevent
                                        placeholder="密码"
                                    />
                                </n-form-item>
                                <div class="login-operation">
                                    <div class="login-operation-login-route" @mouseup.stop="router.push({name:'register'})">
                                        注册账号
                                    </div>
                                    <div class="login-operation-password-forget-text" @mouseup.stop="router.push({name:'forgetpassword'})">
                                        忘记密码
                                    </div>
                                </div>
                            </n-form>
                        </template>

                        <template v-if="loginType===2">
                            <n-form ref="phoneLoginFormRef" :model="loginFormdata" :rules="phoneRules">
                                <n-form-item path="phone" label="手机号">
                                    <n-input v-model:value="loginFormdata.phone" placeholder="手机号" />
                                </n-form-item>
                                <n-form-item path="phoneCode" label="验证码">
                                    <div class="login-phoneCode-frame">
                                        <div class="login-phoneCode-input">
                                            <n-input
                                                v-model:value="loginFormdata.phoneCode"
                                                type="text"
                                                placeholder="验证码"
                                            /> 
                                        </div>
                                       
                                        <template v-if="!loadsend">
                                            <div class="login-phoneCode-btn">
                                                <n-button type="info" block :disabled="!RegIsPhoneNumber(loginFormdata.phone as string)" @mouseup.stop="SendCode(loginFormdata.phone as string)">
                                                    发送短信验证码
                                                </n-button>
                                            </div>
                                        </template>
                                        <template v-if="loadsend">
                                            <div class="login-phoneCode-btn-disable">
                                                <n-button text disabled>
                                                    <div class="login-phoneCode-btn-disable-spin">
                                                        <n-spin size="medium">{{ timecountNum }}</n-spin>
                                                    </div>
                                                    <div class="login-phoneCode-btn-disable-title">
                                                        &nbsp;&nbsp;&nbsp;秒后重新发送
                                                    </div>
                                                </n-button>
                                            </div>
                                        </template>
                                    </div>
                                </n-form-item>
                                <n-form-item path="according" :show-label="false">
                                    <div class="login-according">
                                        <n-checkbox v-model:checked="loginFormdata.according">
                                            阅读并同意<a href="/">《协议》</a>
                                        </n-checkbox>
                                    </div>
                                </n-form-item>
                            </n-form>
                        </template>

                                
                        <n-button type="primary" block @mouseup.stop="Login(loginType)">
                            登录
                            <template v-if="loginType===2">
                                &nbsp;/&nbsp;注册
                            </template>
                        </n-button>
                    </div>

                    <n-divider>
                        <div style="color: #777;font-size: 0.8em;">其他登录方式</div>
                    </n-divider>

                    <div class="login-type">
                        <div class="login-type-item">
                            <div class="login-checklogin">
                                <div class="login-checklogin-user" @mouseup.stop="loginType = 1" v-if="loginType!=1">
                                    <n-button strong secondary circle size="large">
                                        <template #icon>
                                            <n-icon><User /></n-icon>
                                        </template>
                                    </n-button>
                                </div>
                                <div class="login-checklogin-phone" @mouseup.stop="loginType = 2" v-if="loginType!=2">
                                    <n-button strong secondary circle type="success" size="large">
                                        <template #icon>
                                            <n-icon><MdPhonePortrait /></n-icon>
                                        </template>
                                    </n-button>
                                </div>
                            </div>
                        </div>
                    </div>
                </n-card>
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
import { useCookies } from "vue3-cookies";
import { alovaInstance } from '@libs/alova_lib'

import {
  FormInst,
  FormItemInst,
  FormItemRule,
  useMessage,
  FormRules
} from 'naive-ui'

import {
    MdPhonePortrait,
} from '@vicons/ionicons4'

import {
    User,
} from '@vicons/fa'

import { 
    CaptchaBuild,
    RegHaveUppercase,
    RegHaveLowercase,
    RegHaveSymbol,
    RegHaveNumber,
    RegHaveChinese,
    RegHaveLetter,
    RegHaveLetterAndNumber,
    RegIsLetterAndNumber,
    RegIsPhoneNumber,
    RegIsLetter,
    RegIsNumber,
} from '@/plugins/libs/captcha_lib';

interface LoginFormData {
    username: string | null
    password: string | null
    phone: string | null
    phoneCode: string | null
    according:boolean | null
}
interface VaildateData {
    phoneCodeCheck:string
}
const {user,userInfo} = useStore()
const router:Router = useRouter()
const server:string | undefined = inject('SERVER');

const { cookies } = useCookies();
const message = useMessage()
const phoneCodeTime:string | undefined = inject('PHONE_CODE_TIME');

const usernameLoginFormRef = ref<FormInst | null>(null)
const phoneLoginFormRef = ref<FormInst | null>(null)
const loginType = ref<number>(1)
const loadsend = ref<boolean>(false)
const timecountNum = ref<number>(parseInt(phoneCodeTime as string))
    
const loginFormdata = ref<LoginFormData>({
    username: null,
    password: null,
    phone: null,
    phoneCode: null,
    according:false,
})
    
const loginFormdataVaildate = ref<VaildateData>({
    phoneCodeCheck:'',
})

const SendCodeTimeCount = async ():Promise<void>=>{
    loadsend.value = true
    let timedown = setInterval(()=>{
        if(user.isLogin){
            clearInterval(timedown)
        }
        if(timecountNum.value<=0){
            clearInterval(timedown)
            loadsend.value = false
            loginFormdataVaildate.value.phoneCodeCheck = ''
            message.error('验证码已过期，请重新获取',
            {
                duration:5000,
                keepAliveOnHover: true
            })
            timecountNum.value=parseInt(phoneCodeTime as string) 
        }else{
            timecountNum.value-=1
        }
    },1000)
}
const ValidateAccountPassword =  (rule: FormItemRule, value: string): boolean | Error => {
    if (!value) {
        return new Error('请输入[密码]')
    }

    return true
}
const ValidateAccountUsername = (rule: FormItemRule, value: string):boolean | Error=>{
    if (!value) {
        return new Error('请输入[用户名/手机]')
    }

    return true
}
const ValidateAccountPhone = (rule: FormItemRule, value: string):boolean | Error=>{

    if (!value) {
        return new Error('请输入[手机号]')
    }

    if(!RegIsPhoneNumber(value)){
        return new Error('[手机号]格式错误')
    }

    return true
}
const ValidateAccountPhoneCode =  (rule: FormItemRule, value: string): boolean | Error => {
    if (!value) {
        return new Error('请输入[验证码]')
    }
    if (value.length!=6) {
        return new Error('[验证码]长度为6位')
    }
    if (!RegIsNumber(value)) {
        return new Error('[验证码]只能为数字')
    }
    if (loginFormdataVaildate.value.phoneCodeCheck!=value) {
        return new Error('[验证码]错误')
    }
    return true
}
const ValidateAccountAccording = (rule: FormItemRule,value: boolean): boolean | Error => {
    if (!value) {
        return new Error('请阅读[协议]并同意')
    }
    return true
}

const usernameRules: FormRules = {
    username: [
        {
          validator: ValidateAccountUsername,
          trigger: ['input', 'blur']
        }
    ],
    password: [
    {
        validator: ValidateAccountPassword,
        trigger: ['input', 'blur']
    }
    ],
}

const phoneRules: FormRules = {
    phone: [
        {
          validator: ValidateAccountPhone,
          trigger: ['input', 'blur']
        }
    ],
    phoneCode: [
        {
            validator: ValidateAccountPhoneCode,
            trigger: ['input', 'blur']
        },
    ],
    according:[
        {
            validator: ValidateAccountAccording,
            trigger: ['blur','change']
        },
    ]
}

const VaildateDataLoginForm = async (loginTypeNum:number,successFunc:Function):Promise<void> => {
    if(loginTypeNum===1){
        usernameLoginFormRef.value?.validate((errors) => {
            if (!errors) {
                successFunc()
            }else{
                message.error('登录信息有误')
            }
        })
    }else if(loginTypeNum===2){
        phoneLoginFormRef.value?.validate((errors) => {
            if (!errors) {
                successFunc()
            }else{
                message.error('登录信息有误')
            }
        })
    }
}

const Login = async (loginTypeNum:number):Promise<void>=>{
    VaildateDataLoginForm(loginTypeNum,()=>{
        const LoginReq = (loginTypeNum:number,loginFormdata:LoginFormData) => {
            let loginUrl = ''
            switch(loginTypeNum){
                case 1:loginUrl = '/login/LoginUsername';break;
                case 2:loginUrl = '/login/LoginPhone';break;
                default:message.error('服务器错误,请稍后重试');break;
            }
            return alovaInstance.Post(loginUrl,{loginData:loginFormdata},{
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
                cookies.set('userToken',requestData.data.loginUserResult.token,'7d')
                router.push({name:'index'})
            }else{
                message.error(requestData.msg)
            }
        });

        LoginSend(loginTypeNum,loginFormdata.value)
    })
}


const SendCode = async (phone:string):Promise<void>=>{
    //send需要设置不自动发送，不然会发送两次请求触发两次onsuccess，onerror
    const SendCodeReq = (phone:string,codenum:string) => {
        return alovaInstance.Post('/captcha/SendCode',{phone:phone,codenum:codenum},{
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
        send:CodeSend,
        onSuccess,
        onError,
        onComplete
    } = useRequest(SendCodeReq, {
        // 默认发送请求
        immediate:false,
        // 请求响应前，data的初始值
        initialData: []
    });
    onError(err => {
        message.error('发生未知错误',
        {
            keepAliveOnHover: true
        })
    });
    onSuccess((req:any) => {
        let requestData = req.data.data

        if(requestData.status){
            message.success('已发送短信，请收到短信后输入验证码',
            {
                keepAliveOnHover: true
            })
        }else{
            message.error('发送短信失败，请过一段时间后重试',
            {
                keepAliveOnHover: true
            })
        }
    });

    SendCodeTimeCount()

    loginFormdataVaildate.value.phoneCodeCheck = CaptchaBuild(6)

    CodeSend(phone,loginFormdataVaildate.value.phoneCodeCheck)
}

</script>

<style lang="scss" scoped>
.login-frame{
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    padding: 0px 30px;
}
.login-card{
    min-width: 30vw;
}
.login-card-header{
    display: flex;
    justify-content: space-between;
    align-items: end;
}
.login-card-header-backhome{
    cursor: pointer;
    color:#777;
    font-size: 0.7em;
}
.login-phoneCode-frame{
    display:grid;
    grid-template-columns: 2fr 1fr;
    column-gap:20px;
    align-items: center;
}
.login-phoneCode-btn-disable{
    width: 100%;
    justify-self: end;
}
.login-password-forget{
    display: grid;
    justify-items: end;
}
.login-operation{
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding-bottom: 10px;
    &>*{
        cursor: pointer;
        color:#777;
        font-size: 0.8em;
    }
}
.login-type{
    display: flex;
    flex-wrap: wrap;
    align-items: center;
    justify-content: center;
    &>*{
        margin-right: 10px;
    }
}
</style>