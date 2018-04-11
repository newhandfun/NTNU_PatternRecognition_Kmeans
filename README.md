## 臺師大電機系圖像辨識第一次作業  image clusting 

機電107 40373037h 蔡良方

---

### 環境

ubuntu 16.04
code::blocks 16.01

### 說明

此程式目標為全自動的kmeans（Unsupervised learning）
理想情況下會自己將同類的圖片分類
最後印出相關結果

### 流程

1. 讀取圖片(程式目錄底下要有pattern_list與圖片資料夾ObjectDataset)
2. 從檔名取tag
1. 按下任意鍵開始訓練 
3. 取特徵(histogram)
4. 產生中心點，此中心點的feature將與前一個分類的中心點有一定的差距
5. 開始分類
6. 輸出結果並從第三步開始執行

###  缺陷

1. 僅計算accuary,無Precision rate及Recall rate
2. accuary太低
