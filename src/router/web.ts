import { createRouter, createWebHashHistory } from 'vue-router';
import { routers } from './routers.ts';
import useStore from '@/store/store.ts';

export const router = createRouter({
  history: createWebHashHistory(),
  routes: routers,
});


router.beforeEach((to, from, next) => {//里面必须加next();不然不会跳转
  const requiresAuth = to.matched.some(record => record.meta.requiresAuth);
  const {auth} = useStore();
  
  // 使用 `as` 关键字将 `_store` 变量的类型注释为 Store<{ auth_option: { is_login: boolean } }>
  if (requiresAuth && !auth.user?.isLogin) {
    next('/login'); // 跳转到登录页面
  } else {
    next(); // 正常跳转
  }
  next();
});

