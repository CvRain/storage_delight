import { defineStore } from "pinia";
import { ComputedRef, Ref, computed } from "vue";
import { ref,inject } from "vue";
import { DeepClone } from "@/plugins/libs/common_lib";


export default defineStore("userOption",()=>{
    const server:string | undefined = inject('SERVER');
   
    let isLogin = ref<boolean>(false)
    let userInit = ref<User>({
        id:0,
        password:'',
        phone:'',
        role:'',
        token:'',
        username:'',
        deleteUser:false,
    });
    let user = ref<User>({
        id:0,
        password:'',
        phone:'',
        role:'',
        token:'',
        username:'',
        deleteUser:false,
    });

    const UserAction = async (usr:User):Promise<void>=>{
        user.value = usr
    }
    const UserInitAction = async (usr:User):Promise<void>=>{
        userInit.value = usr
    }
    const UserInit = async ():Promise<void>=>{
        user.value = userInit.value
    }
    const IsLoginAction = async (isl:boolean):Promise<void>=>{
        isLogin.value = isl
    }
    return {
        user,
        userInit,
        isLogin,
        UserAction,
        UserInitAction,
        IsLoginAction,

        UserInit,
     }
}, {
    persist: true,
})