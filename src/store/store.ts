import userOption from '@/store/modules/user'
import userInfoOption from '@/store/modules/userInfo'

import { createPinia,Pinia } from 'pinia'
import piniaPluginPersistedstate,{createPersistedState} from 'pinia-plugin-persistedstate'


const useStore = ()=>({
    user:userOption(),
    userInfo:userInfoOption(),
})


export default useStore;