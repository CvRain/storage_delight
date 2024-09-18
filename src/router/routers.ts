import {indexRoutes} from './sub/index'
import {authRoutes} from './sub/auth'
import {RouteRecordRaw} from 'vue-router'

export const routers:Array<RouteRecordRaw> = [
    indexRoutes.index,
    indexRoutes.root,
    authRoutes.login,
    authRoutes.register,
    authRoutes.forgetpassword,
    reportRoutes.report,
]