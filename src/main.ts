import { createApp,h,nextTick,onMounted,toRefs,watch,watchEffect } from 'vue'
import mitt from 'mitt'

import App from './App.vue'

import Header from './components/header.vue'
import Footer from './components/footer.vue'

import 'remixicon/fonts/remixicon.css'
import 'animate.css'
import {router} from './router/web.ts'

import {createPinia} from 'pinia'

import piniaPluginPersistedstate from 'pinia-plugin-persistedstate'

import ReportPaper from './components/report/reportPaper.vue'
import BooleanIcon from './components/booleanIcon.vue'

const app = createApp({
    render(){return h(App)},
})


const emitter: any = mitt();

// window.addEventListener('beforeunload', () => {
//     let user = store.getters.getUser;
//     if (user) {
//         localStorage.setItem('user', JSON.stringify(user));
//     }
// });

const pinia = createPinia();
pinia.use(piniaPluginPersistedstate);

app.provide('BUS',emitter)
app.provide('PHONE_CODE_TIME',import.meta.env.VITE_PHONECODE_TIME)//import.meta.env.VITE_APP_API_URL
app.provide('SERVER',import.meta.env.VITE_API_URL)//import.meta.env.VITE_APP_API_URL
//通用插件
app.use(router);
app.use(pinia);
// 库组件
//自定义组件
app.component('Header',Header);
app.component('Footer',Footer);
app.component('BooleanIcon',BooleanIcon);
app.component('ReportPaper',ReportPaper);

app.mount('#app');