import { createSlice, PayloadAction } from '@reduxjs/toolkit';

// 定义用户信息的类型
interface UserInfo {
    id?: string;
    name?: string;
    token?: string;
    email?: string;
    role?: string;
    // 可以根据需要添加其他字段
}

interface UserStorageState {
    info: UserInfo;
}

const initialState: UserStorageState = {
    info: {},
};

export const userSlice = createSlice({
    name: 'user',
    initialState,
    reducers: {
        // 使用 PayloadAction 来接受新的用户信息
        changeUserInfo: (state, action: PayloadAction<UserInfo>) => {
            state.info = action.payload; // 更新用户信息
        },
    },
});

// 导出 actions
export const { changeUserInfo } = userSlice.actions;

// 导出 reducer
export default userSlice.reducer;
