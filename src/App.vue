<template>
  <n-notification-provider>
    <n-message-provider>
      <n-dialog-provider>
        <n-loading-bar-provider>
          <n-layout>
            <template v-if="headerShow">
              <n-layout-header
                bordered>
                <Header></Header>
              </n-layout-header>
            </template>
            <n-layout-content>
              <router-view/>
            </n-layout-content>
            <template v-if="footerShow">
              <n-layout-footer>
                <Footer></Footer>
              </n-layout-footer>
            </template>
          </n-layout>
        </n-loading-bar-provider>
      </n-dialog-provider>
    </n-message-provider>
  </n-notification-provider>
  </template>
  
  <script setup lang="ts" name="App">
  import { onMounted,onUpdated, onBeforeMount,onBeforeUnmount,onUnmounted,onServerPrefetch, Ref} from 'vue';
  import { ref,reactive,readonly,ComputedRef} from 'vue';
  import { computed,watch,watchEffect,inject,provide} from 'vue';
  import {getCurrentInstance,defineComponent} from 'vue';
  import useStore from '@st/store';
  import {useRoute, RouteLocationNormalizedLoaded} from 'vue-router';
  // 导入 dayjs 库
  import dayjs from 'dayjs'
  // 导入 dayjs 的时区插件
  import utc from 'dayjs/plugin/utc'
  // 导入 dayjs 的时区解析插件
  import timezone from 'dayjs/plugin/timezone'
  import 'dayjs/locale/zh-cn' // 导入本地化语言
  
  // 引入时区数据
  import 'dayjs/locale/zh-cn'
  import 'dayjs/locale/en'
  
  const route:RouteLocationNormalizedLoaded = useRoute()
  const window_width:Ref<number> = ref(0);
  const window_height:Ref<number> = ref(0);
  let headerShow = ref<boolean>(false)
  let footerShow = ref<boolean>(false)

  dayjs.locale('zh-cn') // 使用本地化语言
  
  const getWindowSize = () =>{
    window_width.value = window.innerWidth;
    window_height.value = window.innerHeight;
  }
  onBeforeMount(()=>{
    dayjs.extend(utc)
    dayjs.extend(timezone)
  
    // 设置默认语言为中文
    dayjs.locale('zh-cn')
  
    // 将时区设置为 "Asia/Shanghai"
    dayjs.tz.setDefault('Asia/Shanghai')
  })
  onMounted(() => {
    getWindowSize();
    window.addEventListener('resize', getWindowSize);
  });
  
  onBeforeUnmount(() => {
    window.removeEventListener('resize', getWindowSize);
  });
  
  watchEffect(() => {
    if (route.path === '/login'||route.path === '/register'||route.path === '/forgetpassword'||window_width.value<600) {
      headerShow.value = false;
    }else{
      headerShow.value = true;
    }
  });
  
  
  
  </script>
  
  <style lang="scss">
  @import "./src/assets/scss/main.scss";
  
    #app{
      height: 100vh;
    }
  </style>