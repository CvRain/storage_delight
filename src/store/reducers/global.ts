import { createSlice, PayloadAction } from '@reduxjs/toolkit';

// 定义全局状态的类型
interface WebMain {
    webIcon?:string;
}

// 定义初始状态
const initialState: WebMain = {
    webIcon: process.env.WEB_ICON_URL,
};

// 创建 slice
export const globalStorage = createSlice({
    name: 'globalStorage',
    initialState, // 使用 initialState
    reducers: {
        // 使用 PayloadAction 来接受新的全局属性
        changeMain: (state, action: PayloadAction<WebMain>) => {
            // 更新全局属性
            state = action.payload;
        },
        changeWebIcon: (state, action: PayloadAction<string>) => {
            // 更新全局属性
            state.webIcon = action.payload;
        },
    },
});

// 导出 actions
export const { changeMain,changeWebIcon } = globalStorage.actions;

// 导出 reducer
export default globalStorage.reducer;
