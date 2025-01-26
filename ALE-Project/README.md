# Assembly Language and Embedded System Final Project
> 第 52 組  
> 10827221 許俊偉
> 10827223 呂紹至
> 10827226 李承謙 

## Introduction
使用 **GCC、GAS、GDB** 與 **Code::Blocks**，撰寫一個 **ARM 組合語言程式** 之project，並在模擬 ARM 處理器的環境中執行與驗證，並且實現 **Julia Set 分形圖形** 的視覺化，所有程式均需以 ARM Assembly Language 完成。該專案包含四個主要功能：  
1. **Name 函數** – 印出組別與組員姓名。  
2. **ID 函數** – 允許使用者輸入三組學號，顯示並計算學號總和。  
3. **drawJuliaSet 函數** – 計算並繪製 Julia Set 圖形，將結果顯示於畫面上。  
4. **Main 函數** – 整合所有功能並執行。

## Feature Description
1. Name 函數:  
  (1) 首先，先規劃四個記憶體區塊，分別存放組別及組員姓名(英文)  
  (2) 撰寫程式，把四個記憶體區塊的內容印出來

2. ID 函數:  
  (1) 規劃四個記憶體區塊，之後輸入學號時存放用  
  (2) 請使用者輸入三個學號，並將三個學號存入先前規劃的記憶體  
	(3) 將學號作加總，並將結果存入第四個記憶體區塊  
	(4) 請使用者輸入指令，若指令為 p，就分行印出完整的組員學號與學號總和，若指令不為p 就結束

3. drawJuliaSet 函數:  
	(1) 規劃兩個記憶體區塊，分別存放之後拿來比較使用的數字(1500,4000000)  
	(2) r4-r10的暫存器分別拿來當作cX, cY, width, height, i, x, y使用，因為main裡呼叫 drawJuliaSet 時帶有參數，分別為cX, cY, width, height 分別存在r0-r3，因此，先把r0-r3的值存放到r4-r7  
	(3) 參考所給的 drawJuliaSet.c 範例程式，以 ARM 組合語言重新設計 drawJuliaSet 函數

4. Main 函數:  
  (1) 在 Main 函數中呼叫 Name, ID 與 drawJuliaSet 函數，分別達成前三支函數的分項功能  
  (2) 應用 NAME 與 ID 函數所記錄的資料，輸出完整的組別、組員學號、組員名字、學號加總的計算結果以及指定的字串  


## Execution Flow
1. 執行 Name 函數的原始功能:印出組別及組員名字
![image](picture or gif url)
2. 開始執行 ID 函數  
  (1) 輸入學生1的學號(ID1)，如下圖  
  (2) 輸入學生2的學號(ID2)，如下圖  
  (3) 輸入學生3的學號(ID3)，如下圖
