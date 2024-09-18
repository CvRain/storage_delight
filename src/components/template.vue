<template>
    
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
import { alovaInstance } from '@libs/alova_lib'
// const {} = useStore()
const router:Router = useRouter()
const server:string | undefined = inject('SERVER');
</script>

<style lang="scss" scoped>
</style>