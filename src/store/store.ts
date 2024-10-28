import { configureStore } from '@reduxjs/toolkit';
import rootReducer from './reducers/root';

// 创建 Redux store
const store = configureStore({
  reducer: rootReducer, // 直接使用 rootReducer
});

export default store; // 导出 store 实例

// Infer the type of store
export type AppStore = typeof store;

// Infer the `RootState` and `AppDispatch` types from the store itself
export type RootState = ReturnType<AppStore['getState']>;
export type AppDispatch = AppStore['dispatch'];
