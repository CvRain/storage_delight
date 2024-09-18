import { defineStore } from "pinia";
import { ComputedRef, Ref, computed } from "vue";
import { ref,inject } from "vue";
import { DeepClone } from "@/plugins/libs/common_lib";


export default defineStore("userInfoOption",()=>{
    const server:string | undefined = inject('SERVER');
   
    let userInfo = ref<UserInfo>({
        createdAt: '',
        email: '',
        expressAddress: '',
        headerImg: undefined,
        id: 0,
        jindong: '',
        nickname: '',
        pinduoduo: '',
        qq: '',
        taobao: '',
        updatedAt: '',
        userId: 0,
        wechat: '',
        xianyu: '',
    });

    const UserInfoAction = async (usrIf:UserInfo):Promise<void>=>{
        userInfo.value = usrIf
    }
    return {
        userInfo,
        UserInfoAction,
    }
}, {
    persist: true,
})