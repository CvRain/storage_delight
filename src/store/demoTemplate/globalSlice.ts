import { createSlice, PayloadAction } from '@reduxjs/toolkit';

// 定义用户信息的类型
interface globalProp {
    headerShow?: boolean;
}

const globalState: globalProp = {
    headerShow: false,
};

export const globalStorage = createSlice({
    name: 'globalStorage',
    globalState,
    reducers: {
        // 使用 PayloadAction 来接受新的用户信息
        changeGlobalProp: (state, action: PayloadAction<globalProp>) => {
            state.info = action.payload; // 更新用户信息
        },
    },
});

// 导出 actions
export const { changeGlobalProp } = globalStorage.actions;

// 导出 reducer
export default globalStorage.reducer;
