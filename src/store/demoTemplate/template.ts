import { createSlice, PayloadAction } from '@reduxjs/toolkit';

// 定义全局状态的类型
interface GlobalState {
    headerShow?: boolean;
}

// 定义初始状态
const initialState: GlobalState = {
    headerShow: false,
};

// 创建 slice
export const globalStorage = createSlice({
    name: 'globalStorage',
    initialState, // 使用 initialState
    reducers: {
        // 使用 PayloadAction 来接受新的全局属性
        changeGlobalProp: (state, action: PayloadAction<GlobalState>) => {
            // 更新全局属性
            state.headerShow = action.payload.headerShow; // 直接更新 headerShow
        },
    },
});

// 导出 actions
export const { changeGlobalProp } = globalStorage.actions;

// 导出 reducer
export default globalStorage.reducer;
