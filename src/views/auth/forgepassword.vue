<template>
    <div class="forgetpassword">
        <div class="forgetpassword-frame">
            <div class="forgetpassword-card">
                <n-card size="huge">
                    <template #header>
                        <div class="forgetpassword-card-title">
                            <div class="forgetpassword-card-title-text">
                                忘记密码
                            </div>
                            <template v-if="forgetpasswordStep>0">
                                <div class="forgetpassword-card-title-backhome">
                                    <n-button text type="info" @mouseup.stop="router.push({name:'login'})">
                                        返回登录
                                    </n-button>
                                </div>
                            </template>
                        </div>
                    </template>

                    <template v-if="forgetpasswordStep==1">
                        <n-form-item path="username" label="用户名" :rule="usernameRule.username">
                            <n-input 
                                v-model:value="forgetpasswordFormdata.username" 
                                type="text"
                                @keydown.enter.prevent 
                                placeholder="用户名" 
                            />
                        </n-form-item>

                        <n-button type="primary" @mouseup.stop="userExistsCheck(forgetpasswordFormdata.username)" block>
                            下一步
                        </n-button>
                    </template>

                    <template v-if="forgetpasswordStep==2">

                        <div class="" style="padding-bottom: 10px;">{{ `${forgetpasswordUser?.phone.slice(0,3)}*****${forgetpasswordUser?.phone.slice(-3)}` }}</div>

                        <n-form-item path="phone" label="手机号" :rule="phoneRule.phone">
                            <n-input
                                v-model:value="forgetpasswordFormdata.phone"
                                type="text"
                                @keydown.enter.prevent
                                placeholder="手机号"
                            />
                        </n-form-item>
                        <n-form-item path="phoneCode" label="验证码" :rule="phoneRule.phoneCode">
                            <div class="forgetpassword-phoneCode-frame">
                                <div class="forgetpassword-phoneCode-input">
                                    <n-input
                                        v-model:value="forgetpasswordFormdata.phoneCode"
                                        type="text"
                                        placeholder="验证码"
                                    /> 
                                </div>
                                <template v-if="!loadsend">
                                    <div class="forgetpassword-phoneCode-btn">
                                        <n-button type="info" block :disabled="!RegIsPhoneNumber(forgetpasswordFormdata.phone)" @mouseup.stop="SendCode(forgetpasswordFormdata.phone)">
                                            发送短信验证码
                                        </n-button>
                                    </div>
                                </template>
                                <template v-if="loadsend">
                                    <div class="forgetpassword-phoneCode-btn-disable">
                                        <n-button text disabled>
                                            <div class="forgetpassword-phoneCode-btn-disable-spin">
                                                <n-spin size="medium">{{ timecountNum }}</n-spin>
                                            </div>
                                            <div class="forgetpassword-phoneCode-btn-disable-title">
                                                &nbsp;&nbsp;&nbsp;秒后重新发送
                                            </div>
                                        </n-button>
                                    </div>
                                </template>
                            </div>
                        </n-form-item>


                        <div style="display: grid;grid-template-columns: 1fr 3fr;column-gap: 20px;">
                            <n-button @mouseup.stop="forgetpasswordStep = 1" block>
                                上一步
                            </n-button>
                            <n-button type="primary" block @mouseup.stop="phoneCodeCheck">
                                下一步
                            </n-button>
                        </div>
                    </template>
                    
                    <template v-if="forgetpasswordStep==3">
                        <n-form-item path="password" label="密码" :rule="passwordRule.password">
                            <n-popover 
                                :show="forgetpasswordFormdataVaildate.passwordPopoverShow"
                                trigger="manual"
                                placement="top"
                            >
                                <template #trigger>
                                    <n-input
                                        v-model:value="forgetpasswordFormdata.password"
                                        type="password"
                                        @keydown.enter.prevent
                                        placeholder="密码"
                                        @focus.stop="forgetpasswordFormdataVaildate.passwordPopoverShow = true"
                                        @blur.stop="forgetpasswordFormdataVaildate.passwordPopoverShow = false"
                                    />
                                </template>
                                    <div class="forgetpassword-card-password-diffcult">
                                        <div class="forgetpassword-card-password-diffcult-title-frame">
                                            <div class="forgetpassword-card-password-diffcult-title">强度：</div>
                                            <n-rate readonly :default-value="0" :color="forgetpasswordFormdataVaildate.passwordDiffcultColor" :value="forgetpasswordFormdataVaildate.passwordDiffcult" :count="3">
                                                <n-icon size="20">
                                                    <Square />
                                                </n-icon>
                                            </n-rate>

                                            <div class="forgetpassword-card-password-diffcult-title">
                                                长度大于6小于20
                                                <n-rate readonly :default-value="0" color="green" :value="(forgetpasswordFormdata.password.length>6&&forgetpasswordFormdata.password.length<20)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                            
                                            <div class="forgetpassword-card-password-diffcult-title">密码格式&nbsp;(需包含其中三种)</div>
                                            <div class="forgetpassword-card-password-diffcult-title">
                                                大写字母&nbsp;[A-Z]
                                                <n-rate readonly :default-value="0" color="green" :value="RegHaveUppercase(forgetpasswordFormdata.password)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                            <div class="forgetpassword-card-password-diffcult-title">
                                                特殊字符&nbsp;[.@!%*#_~?&$]
                                                <n-rate readonly :default-value="0" color="green" :value="RegHaveSymbol(forgetpasswordFormdata.password)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                            <div class="forgetpassword-card-password-diffcult-title">
                                                小写字母&nbsp;[a-z]
                                                <n-rate readonly :default-value="0" color="green" :value="RegHaveLowercase(forgetpasswordFormdata.password)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                            <div class="forgetpassword-card-password-diffcult-title">
                                                数字&nbsp;[0-9]
                                                <n-rate readonly :default-value="0" color="green" :value="RegHaveNumber(forgetpasswordFormdata.password)?1:0" :count="1">
                                                    <n-icon size="20">
                                                        <CheckmarkCircleSharp />
                                                    </n-icon>
                                                </n-rate>
                                            </div>
                                        </div>
                                    </div>
                            </n-popover>
                        </n-form-item>
                        <n-form-item path="passwordCheck" label="确认密码" :rule="passwordRule.passwordCheck">
                            <n-input
                                v-model:value="forgetpasswordFormdata.passwordCheck"
                                type="password"
                                @keydown.enter.prevent
                                placeholder="确认密码"
                            />
                        </n-form-item>

                        <div style="display: grid;grid-template-columns: 1fr 3fr;column-gap: 20px;">
                            <n-button @mouseup.stop="forgetpasswordStep = 2" block>
                                上一步
                            </n-button>
                            <n-button type="primary" block @mouseup.stop="forgetpassword">
                                修改
                            </n-button>
                        </div>
                    </template>

                    <template v-if="forgetpasswordStep==4">
                        <div style="padding: 30px 0px;">
                            <n-result status="success" title="成功" description="修改密码成功">
                                <template #footer>
                                    <n-button @mouseup.stop="router.push({name:'index'})">返回主页</n-button>
                                </template>
                            </n-result>
                        </div>
                    </template>

                    <template #footer>
                        <template v-if="forgetpasswordStep>0">
                            <div>
                                <n-steps :current="forgetpasswordStep" :status="forgetpasswordStepStatus">
                                    <n-step
                                        title="用户名"
                                    />
                                    <n-step
                                        title="手机验证"
                                    />
                                    <n-step
                                        title="密码修改"
                                    />
                                    <n-step
                                        title="完成"
                                    />
                                </n-steps>
                            </div>
                        </template>
                    </template>
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
import {Square,CheckmarkCircleSharp,InformationCircle} from '@vicons/ionicons5'

import {
  FormInst,
  FormItemInst,
  FormItemRule,
  useMessage,
  FormRules,
  StepsProps,
} from 'naive-ui'

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

interface forgetpasswordFormData {
    username: string
    password: string
    passwordCheck: string 
    phone: string 
    phoneCode: string 
    token: string 
}
interface VaildateData{
    passwordDiffcult:number
    passwordDiffcultColor:string
    passwordPopoverShow:boolean
    phoneCodeCheck:string
}

const message = useMessage()
const {user,userInfo} = useStore()
const router:Router = useRouter()
const phoneCodeTime:string | undefined = inject('PHONE_CODE_TIME');
const loadsend = ref<boolean>(false)
const timecountNum = ref<number>(parseInt(phoneCodeTime as string))
const { cookies } = useCookies();

const forgetpasswordStepStatus = ref<StepsProps['status']>('process');
const forgetpasswordStep = ref<number>(1)
const forgetpasswordUser = ref<User>()
const forgetpasswordFormdataVaildate = ref<VaildateData>({
    passwordPopoverShow:false,
    passwordDiffcult:0,
    passwordDiffcultColor:'red',
    phoneCodeCheck:  '',
})
const forgetpasswordFormdata = ref<forgetpasswordFormData>({
    username:  '',
    password:  '',
    passwordCheck:  '',
    phone:  '',
    phoneCode:  '',
    token:  '',
})

const SendCodeTimeCount = async ():Promise<void>=>{
    loadsend.value = true
    let timedown = setInterval(()=>{
        if(timecountNum.value<=0){
            clearInterval(timedown)
            loadsend.value = false
            forgetpasswordFormdataVaildate.value.phoneCodeCheck = ''
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

const ValidateAccountUsername = (rule: FormItemRule, val: string):boolean | Error=>{
    let value = forgetpasswordFormdata.value.username
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

const ValidateAccountPassword =  (rule: FormItemRule, val: string): boolean | Error => {
    let value = forgetpasswordFormdata.value.password
    let regResult = [RegHaveUppercase(value),RegHaveSymbol(value),RegHaveLetterAndNumber(value)]

    forgetpasswordFormdataVaildate.value.passwordDiffcult=0

    regResult.map(res=>{
        if(res){
            forgetpasswordFormdataVaildate.value.passwordDiffcult++ 
        }
    })

    switch(forgetpasswordFormdataVaildate.value.passwordDiffcult){
        case 1:forgetpasswordFormdataVaildate.value.passwordDiffcultColor='red';break;
        case 2:forgetpasswordFormdataVaildate.value.passwordDiffcultColor='yellow';break;
        case 3:forgetpasswordFormdataVaildate.value.passwordDiffcultColor='green';break;
    }

    if (!value) {
        forgetpasswordFormdataVaildate.value.passwordDiffcult=0
        return new Error('请输入[密码]')
    }

    if(value.length<6){
        return new Error('[密码]长度最小为6')
    }

    if(value.length>20){
        return new Error('[密码]长度最大为20')
    }

    if(forgetpasswordFormdataVaildate.value.passwordDiffcult<2){
        return new Error('[密码]过于简单')
    }

    return true
}
const ValidateAccountPhone = (rule: FormItemRule, val: string):boolean | Error=>{
    let value = forgetpasswordFormdata.value.phone
    if (!value) {
        return new Error('请输入[手机号]')
    }
    if(!RegIsPhoneNumber(value)){
        return new Error('[手机号]格式错误')
    }
    if(forgetpasswordUser.value?.phone != value){
        return new Error('输入[手机号]错误')
    }
    return true
}
const ValidateAccountPhoneCode =  (rule: FormItemRule, val: string): boolean | Error => {
    let value = forgetpasswordFormdata.value.phoneCode
    if (!value) {
        return new Error('请输入[验证码]')
    }
    if (value.length!=6) {
        return new Error('[验证码]长度为6位')
    }
    if (!RegIsNumber(value)) {
        return new Error('[验证码]只能为数字')
    }
    if (forgetpasswordFormdataVaildate.value.phoneCodeCheck!=value&&value!=''&&forgetpasswordFormdataVaildate.value.phoneCodeCheck!='') {
        return new Error('[验证码]错误')
    }
    return true
}
const ValidatePasswordCheck = (rule: FormItemRule, val: boolean): boolean | Error => {
    let value = forgetpasswordFormdata.value.passwordCheck
    if (!value) {
        return new Error('[密码验证]不能为空')
    }
    return true
}
const usernameRule: FormRules = {
    username: [
        {
          validator: ValidateAccountUsername,
          trigger: ['input', 'blur']
        }
    ],
}
const passwordRule: FormRules = {
    password: [
        {
            validator: ValidateAccountPassword,
            trigger: ['input', 'blur']
        },
    ],
    passwordCheck: [
        {
            message: '请再次输入密码',
            trigger: ['input', 'blur']
        },
        {
            message: '两次密码输入不一致',
            trigger: 'input'
        },
        {
            validator: ValidatePasswordCheck,
            trigger: ['blur','input', 'password-input']
        }
    ],
}
const phoneRule: FormRules = {
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
}
const phoneCodeCheck = async ():Promise<void>=>{
    if(forgetpasswordFormdataVaildate.value.phoneCodeCheck&&forgetpasswordFormdata.value.phoneCode&&forgetpasswordFormdata.value.phoneCode == forgetpasswordFormdataVaildate.value.phoneCodeCheck){
        forgetpasswordStep.value = 3
    }
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
    onSuccess(req => {
        if(req){
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

    forgetpasswordFormdataVaildate.value.phoneCodeCheck = CaptchaBuild(6)

    CodeSend(phone,forgetpasswordFormdataVaildate.value.phoneCodeCheck)
}
const userExistsCheck = async (username:string):Promise<void>=>{
    const forgetpasswordUserExistsCheckReq = (username:string) => {
        return alovaInstance.Post('/forgetpassword/UserExists',{forgetpasswordUsername:username},{
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
        send:forgetpasswordUserExistsReqSend,
        onSuccess,
        onError,
        onComplete
    } = useRequest(forgetpasswordUserExistsCheckReq, {
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
        if(requestData.code==10001){
            forgetpasswordUser.value = requestData.data.forgetpasswordUserResult
            forgetpasswordStep.value = 2
        }else{
            message.error(requestData.msg)
        }
    });

    forgetpasswordUserExistsReqSend(username)
}
const forgetpassword = async ():Promise<void>=>{
        //send需要设置不自动发送，不然会发送两次请求触发两次onsuccess，onerror
        const forgetpasswordReq = (forgetpasswordFdt:forgetpasswordFormData) => {
            return alovaInstance.Patch('/forgetpassword/PasswordPatch',{forgetpasswordData:forgetpasswordFdt},{
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
            send:forgetpasswordReqSend,
            onSuccess,
            onError,
            onComplete
        } = useRequest(forgetpasswordReq, {
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
            if(requestData.code==20004){
                forgetpasswordStep.value = 4
                user.IsLoginAction(false);
                user.UserInit();
                cookies.remove('userToken')
            }else{
                message.error(requestData.msg)
            }
        });

        forgetpasswordReqSend(forgetpasswordFormdata.value)
}
</script>

<style lang="scss" scoped>
.forgetpassword-frame{
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
}

.forgetpassword-card{
    min-width: 30vw;
    padding: 30px 0px;
}
.forgetpassword-card-title{
    display: flex;
    justify-content: space-between;
    align-items: center;
}
.forgetpassword-card-password-default{
    display: flex;
    align-items: center;
}
.forgetpassword-card-password-diffcult-title{
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 2px 0px
}
.forgetpassword-phoneCode-frame{
    width:100%;
    display:grid;
    grid-template-columns: 2fr 1fr;
    column-gap:20px;
    align-items: center;
}
.forgetpassword-phoneCode-btn-disable{
    width: 100%;
    justify-self: end;
}
</style>