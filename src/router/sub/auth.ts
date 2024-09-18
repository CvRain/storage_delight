import { createRouter,createWebHashHistory} from "vue-router";
import login from '@views/auth/login.vue';
import register from '@views/auth/register.vue';
import forgetpassword from '@views/auth/forgepassword.vue';
export const authRoutes = {
  login:{
    path: "/login",
    name: "login",
    component:login,
  },
  register:{
    path: "/register",
    name: "register",
    component:register,
  },
  forgetpassword:{
    path: "/forgetpassword",
    name: "forgetpassword",
    component:forgetpassword,
  }
}

