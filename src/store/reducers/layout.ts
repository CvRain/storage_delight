import { createSlice, PayloadAction } from '@reduxjs/toolkit';

// 定义全局状态的类型
interface LayoutState {
    headerShow: boolean;
    siderShow: boolean;
    footerShow: boolean;
}

// 定义初始状态
const initialState: LayoutState = {
    headerShow: true,
    siderShow: true,
    footerShow: true,
};

// 创建 slice
export const layoutStorage = createSlice({
    name: 'layoutStorage',
    initialState, // 使用 initialState
    reducers: {
        // 使用 PayloadAction 来接受新的全局属性
        changeGlobalProp: (state, action: PayloadAction<LayoutState>) => {
            // 更新全局属性
            state = action.payload;
        },
        changeHeaderShow: (state, action: PayloadAction<boolean>) => {
            // 更新全局属性
            state.headerShow = action.payload;
        },
        changeSiderShow: (state, action: PayloadAction<boolean>) => {
            // 更新全局属性
            state.siderShow = action.payload;
        },
        changeFooterShow: (state, action: PayloadAction<boolean>) => {
            // 更新全局属性
            state.footerShow = action.payload;
        },
    },
});

// 导出 actions
export const { changeGlobalProp,changeHeaderShow,changeSiderShow,changeFooterShow } = layoutStorage.actions;

// 导出 reducer
export default layoutStorage.reducer;
