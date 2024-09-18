import { createRouter,createWebHashHistory} from "vue-router";
import index from "../../views/index.vue";
export const indexRoutes = {
  index:{
    path: "/index",
    name: "index",
    component:index,
  },
  root:{
    path: "/",
    name: "root",
    component:index,
  }
}

