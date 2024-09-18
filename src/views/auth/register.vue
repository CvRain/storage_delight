<template>
    <div class="register">
        <div class="register-frame">
            <div class="register-card">
                <n-card title="" size="huge">
                    <n-form ref="registerFormRef" :model="registerFormdata" :rules="registerRules">
                        <n-form-item path="username" label="用户名">
                            <n-input 
                                v-model:value="registerFormdata.username" 
                                type="text"
                                @keydown.enter.prevent 
                                placeholder="用户名" 
                            />
                        </n-form-item>
                        <n-form-item path="password" label="密码">
                            <n-popover 
                                :show="registerFormdataVaildate.passwordPopoverShow"
                                trigger="manual"
                                placement="right"
                            >
                                <template #trigger>
                                    <n-input
                                        v-model:value="registerFormdata.password"
                                        type="password"
                                        @keydown.enter.prevent
                                        placeholder="密码"
                                        @focus.stop="registerFormdataVaildate.passwordPopoverShow = true"
                                        @blur.stop="registerFormdataVaildate.passwordPopoverShow = false"
                                    />
                                </template>
                                    <div class="register-card-password-diffcult">
                                        <div class="register-card-password-diffcult-title-frame">
                                            <div class="register-card-password-diffcult-title">强度：</div>
                                            <n-rate readonly :default-value="0" :color="registerFormdataVaildate.passwordDiffcultColor" :value="registerFormdataVaildate.passwordDiffcult" :count="3">
                                                <n-icon size="20">
                                                    <Square />
                                                </n-icon>
                                            </n-rate>

                                            <div class="register-card-password-diffcult-title">
                                                长度大于6小于20
                                                <n-rate readonly :default-value="0" color="green" :value="(registerFormdata.password.length>6&&registerFormdata.password.length<20)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                            
                                            <div class="register-card-password-diffcult-title">密码格式&nbsp;(需包含其中三种)</div>
                                            <div class="register-card-password-diffcult-title">
                                                大写字母&nbsp;[A-Z]
                                                <n-rate readonly :default-value="0" color="green" :value="RegHaveUppercase(registerFormdata.password)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                            <div class="register-card-password-diffcult-title">
                                                特殊字符&nbsp;[.@!%*#_~?&$]
                                                <n-rate readonly :default-value="0" color="green" :value="RegHaveSymbol(registerFormdata.password)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                            <div class="register-card-password-diffcult-title">
                                                小写字母&nbsp;[a-z]
                                                <n-rate readonly :default-value="0" color="green" :value="RegHaveLowercase(registerFormdata.password)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                            <div class="register-card-password-diffcult-title">
                                                数字&nbsp;[0-9]
                                                <n-rate readonly :default-value="0" color="green" :value="RegHaveNumber(registerFormdata.password)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                        </div>
                                    </div>
                            </n-popover>
                        </n-form-item>
                        <n-form-item path="passwordCheck" label="确认密码">
                            <n-input
                                v-model:value="registerFormdata.passwordCheck"
                                type="password"
                                @keydown.enter.prevent
                                placeholder="确认密码"
                            />
                        </n-form-item>
                        <n-form-item path="phone" label="手机号">
                            <n-input
                                v-model:value="registerFormdata.phone"
                                type="text"
                                @keydown.enter.prevent
                                placeholder="手机号"
                            />
                        </n-form-item>
                        <n-form-item path="phoneCode" label="验证码">
                            <div class="register-phoneCode-frame">
                                <div class="register-phoneCode-input">
                                    <n-input
                                        v-model:value="registerFormdata.phoneCode"
                                        type="text"
                                        placeholder="验证码"
                                    /> 
                                </div>
                                <template v-if="!loadsend">
                                    <div class="register-phoneCode-btn">
                                        <n-button type="info" block :disabled="!RegIsPhoneNumber(registerFormdata.phone)" @mouseup.stop="SendCode(registerFormdata.phone)">
                                            发送短信验证码
                                        </n-button>
                                    </div>
                                </template>
                                <template v-if="loadsend">
                                    <div class="register-phoneCode-btn-disable">
                                        <n-button text disabled>
                                            <div class="register-phoneCode-btn-disable-spin">
                                                <n-spin size="medium">{{ timecountNum }}</n-spin>
                                            </div>
                                            <div class="register-phoneCode-btn-disable-title">
                                                &nbsp;&nbsp;&nbsp;秒后重新发送
                                            </div>
                                        </n-button>
                                    </div>
                                </template>
                            </div>
                        </n-form-item>
                        <n-form-item path="according" :show-label="false">
                            <n-checkbox v-model:checked="registerFormdata.according">
                                阅读并同意<a href="/">《协议》</a>
                            </n-checkbox>
                        </n-form-item>
                        <n-form-item path="regbtn" :show-label="false">
                            <n-button type="primary" block @mouseup.stop="Register">
                                注册
                            </n-button>
                        </n-form-item>
                    </n-form>
                    
                    <n-button type="primary" block @mouseup.stop="router.push({name:'login'})" text>
                            回到登录
                    </n-button>
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
import {Square,CheckmarkCircleSharp,InformationCircle} from '@vicons/ionicons5'

import {
  FormInst,
  FormItemInst,
  FormItemRule,
  useMessage,
  FormRules
} from 'naive-ui'

import { useCookies } from "vue3-cookies";
import { alovaInstance } from '@libs/alova_lib'
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

interface RegisterFormData {
    username: string
    password: string
    passwordCheck: string 
    phone: string 
    phoneCode: string 
    token: string 
    according: boolean
}
interface VaildateData{
    passwordDiffcult:number
    passwordDiffcultColor:string
    passwordPopoverShow:boolean
    phoneCodeCheck:string
}

const { cookies } = useCookies();
const message = useMessage()
const {user,userInfo} = useStore()
const router:Router = useRouter()
const phoneCodeTime:string | undefined = inject('PHONE_CODE_TIME');
const loadsend = ref<boolean>(false)
const timecountNum = ref<number>(parseInt(phoneCodeTime as string))

const registerFormRef = ref<FormInst | null>(null)
const registerFormdataVaildate = ref<VaildateData>({
    passwordPopoverShow:false,
    passwordDiffcult:0,
    passwordDiffcultColor:'red',
    phoneCodeCheck:  '',
})
const registerFormdata = ref<RegisterFormData>({
    username:  '',
    password:  '',
    passwordCheck:  '',
    phone:  '',
    phoneCode:  '',
    token:  '',
    according: false,
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
            registerFormdataVaildate.value.phoneCodeCheck = ''
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

const ValidateAccountUsername = (rule: FormItemRule, value: string):boolean | Error=>{
    if (!value) {
        return new Error('请输入[用户名]')
    }
    if(RegHaveChinese(value)){
        return new Error('[用户名]不能包含中文')
    }
    if(!RegIsLetterAndNumber(value)){
        return new Error('[用户名]只能包含英文和数字')
    }
    if(value.length<6){
        return new Error('[用户名]长度最小为6')
    }

    if(value.length>20){
        return new Error('[用户名]长度最大为20')
    }

    return true
}

const ValidateAccountPassword =  (rule: FormItemRule, value: string): boolean | Error => {
    
    let regResult = [RegHaveUppercase(value),RegHaveSymbol(value),RegHaveLetterAndNumber(value)]

    registerFormdataVaildate.value.passwordDiffcult=0

    regResult.map(res=>{
        if(res){
            registerFormdataVaildate.value.passwordDiffcult++ 
        }
    })

    switch(registerFormdataVaildate.value.passwordDiffcult){
        case 1:registerFormdataVaildate.value.passwordDiffcultColor='red';break;
        case 2:registerFormdataVaildate.value.passwordDiffcultColor='yellow';break;
        case 3:registerFormdataVaildate.value.passwordDiffcultColor='green';break;
    }

    if (!value) {
        registerFormdataVaildate.value.passwordDiffcult=0
        return new Error('请输入[密码]')
    }

    if(value.length<6){
        return new Error('[密码]长度最小为6')
    }

    if(value.length>20){
        return new Error('[密码]长度最大为20')
    }

    if(registerFormdataVaildate.value.passwordDiffcult<2){
        return new Error('[密码]过于简单')
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
    if (registerFormdataVaildate.value.phoneCodeCheck!=value) {
        return new Error('[验证码]错误')
    }
    return true
}
const ValidateAccountAccording = (rule: FormItemRule, value: boolean): boolean | Error => {
    if (!value) {
        return new Error('请阅读[协议]并同意')
    }
    return true
}
const ValidatePasswordCheck = (rule: FormItemRule, value: string): boolean | Error => {
    if (!value) {
        return new Error('[密码验证]不能为空')
    }
    if (value!=registerFormdata.value.password) {
        return new Error('两次密码输入不一致')
    }
    return true
}

const registerRules: FormRules = {
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
        },
    ],
    phone:[
        {
            validator: ValidateAccountPhone,
            trigger: ['input', 'blur']
        }
    ],
    phoneCode:[
        {
            validator: ValidateAccountPhoneCode,
            trigger: ['input', 'blur']
        }
    ],
    passwordCheck: [
        {
            validator: ValidatePasswordCheck,
            trigger: ['blur','input', 'password-input']
        }
    ],
    according:[
        {
            validator: ValidateAccountAccording,
            trigger: ['change','blur']
        }
    ]
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

    registerFormdataVaildate.value.phoneCodeCheck = CaptchaBuild(6)

    CodeSend(phone,registerFormdataVaildate.value.phoneCodeCheck)
}
const VaildateDataRegisterForm = async (successFunc:Function):Promise<void> => {
    const vaildateDataRegisterFormMsg = message.loading('验证信息中', {
        duration: 0
    })

    registerFormRef.value?.validate((errors) => {
        if (!errors) {
            successFunc()
        }else{
            message.error('验证信息失败，请修改信息')
        }
        vaildateDataRegisterFormMsg.destroy()
    })
}
const Register = async ():Promise<void>=>{
    VaildateDataRegisterForm(()=>{
         //send需要设置不自动发送，不然会发送两次请求触发两次onsuccess，onerror
        const RegisterReq = (registerFdt:RegisterFormData) => {
            return alovaInstance.Put('/register/InsertUser',{registerData:registerFdt},{
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
            send:RegisterReqSend,
            onSuccess,
            onError,
            onComplete
        } = useRequest(RegisterReq, {
            // 默认发送请求
            immediate:false,
            // 请求响应前，data的初始值
            initialData: {}
        });
        onError(err => {
            message.error('发生未知错误,请稍后再试',
            {
                keepAliveOnHover: true
            })
        });
        onSuccess((req:any) => {
            let requestData = req.data.data
            if(requestData.code==20001){
                user.IsLoginAction(true)
                user.UserAction(requestData.data.registerUserResult)
                userInfo.UserInfoAction(requestData.data.registerUserInfoResult)
                cookies.set('userToken',requestData.data.registerUserInfoResult.token,'7d')
                message.success('注册成功，自动跳转至主页')
                router.push({name:'index'})
            }else{
                message.error(requestData.msg)
            }
        });

        RegisterReqSend(registerFormdata.value)
    })
}
</script>

<style lang="scss" scoped>
.register-frame{
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
}

.register-card{
    min-width: 30vw;
}
.register-card-password-default{
    display: flex;
    align-items: center;
}
.register-card-password-diffcult-title{
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 2px 0px
}
.register-phoneCode-frame{
    display:grid;
    grid-template-columns: 2fr 1fr;
    column-gap:20px;
    align-items: center;
}
.register-phoneCode-btn-disable{
    width: 100%;
    justify-self: end;
}
</style>