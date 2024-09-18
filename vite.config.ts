import path from 'path'
import type { PluginOption } from 'vite'
import { defineConfig, loadEnv } from 'vite'
import vue from '@vitejs/plugin-vue'

import AutoImport from 'unplugin-auto-import/vite'
import Components from 'unplugin-vue-components/vite'

import { NaiveUiResolver } from 'unplugin-vue-components/resolvers'

export default defineConfig(({ mode, command }) => {
  const env = loadEnv(mode, process.cwd(), '')
  const { VITE_API_URL } = env;
  return {
    define: {
    },
    resolve: {
      alias: {
        '@': path.resolve(process.cwd(), 'src'),
        '@components': path.resolve(process.cwd(), 'src/components'),
        '@st': path.resolve(process.cwd(), 'src/store'),
        '@assets':path.resolve(process.cwd(),'src/assets'),
        '@router':path.resolve(process.cwd(),'src/router'),
        '@libs':path.resolve(process.cwd(),'src/plugins/libs'),
        '@views':path.resolve(process.cwd(),'src/views'),
      },
    },
    plugins:[
      vue({reactivityTransform: true,}),
      AutoImport({
        imports: [
          'vue',
          {
            'naive-ui': [
              'useDialog',
              'useMessage',
              'useNotification',
              'useLoadingBar'
            ]
          }
        ]
      }),
      Components({
        resolvers: [NaiveUiResolver()],
      }),
    ],
    server: {
      host: '0.0.0.0',
      port: 8080,
      open: false,
    },
    build: {
      reportCompressedSize: false,
      sourcemap: false,
      commonjsOptions: {
        ignoreTryCatch: false,
      },
    },
  }
})
