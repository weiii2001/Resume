
# **OurScheme Interpreter**

## 📌 **Project 1：OurScheme Interpreter (Basic)**

---

### **Overview**
Project 1 的目標是實作一個簡單的 OurScheme 解譯器，主要功能為讀取 S-expression、解析並以格式化方式輸出。

---

### **Feature Description**

1. **掃描器（Scanner Layer）**  
   - 將用戶輸入依據 Scheme 語法進行分詞（Tokenize）。  
   - 支援的 Token 包括：`LEFT-PAREN`、`RIGHT-PAREN`、`INT`、`FLOAT`、`STRING`、`DOT`、`NIL`、`T`、`QUOTE` 和 `SYMBOL`。

2. **解析器（Parser Layer）**  
   - 驗證 Token 是否符合 S-表達式的語法規則。  
   - 建立對應的樹狀資料結構以表示合法的 S-表達式。

3. **格式化輸出（Pretty Printer）**  
   - 將內部資料結構轉換為格式化的輸出結果。

4. **錯誤處理（Error Handling）**  
   - 顯示錯誤訊息並指出錯誤所在位置（行數與欄位）。  
   - 偵測括號不平衡、無效的 Token 與不完整的表達式。

5. **互動式系統（Interactive System）**  
   - 提供 `> ` 提示符號等待用戶輸入。  
   - 持續運行直到用戶輸入 `(exit)` 或檔案結尾（EOF）。  
   - 結束時顯示感謝訊息。

---

### **Execution Flow**
```text
Welcome to OurScheme!

> <user inputs S-expression>

<pretty-printed result or error message>

> 

(repeat until (exit) or EOF)

Thanks for using OurScheme!
```

---

### **Error Messages**
- ERROR (unexpected token) : atom or '(' expected when token at Line X Column Y is >>...<<
- ERROR (no closing quote) : END-OF-LINE encountered at Line X Column Y
- ERROR (no more input) : END-OF-FILE encountered

---

### **Example Usage**
```text
> (cons 3 4)
( 3
  .
  4
)

> '(1 2 3)
( 1
  2
  3
)

> (1 2 . 3 4)
ERROR (unexpected token) : ')' expected when token at Line 1 Column 10 is >>4<<
```

---

## 📌 **Project 2：OurScheme Interpreter (Extended Evaluation)**

---

### **Overview**
Project 2 在 Project 1 的基礎上擴充功能，新增對基本運算、變數定義、條件判斷與序列運算等的支援，使 OurScheme 解譯器能進行更完整的運算與處理。

---

### **Feature Description**

1. **基本運算（Primitive Operation Evaluation）**  
   - 支援算術運算：`+`、`-`、`*`、`/`  
   - 邏輯運算：`and`、`or`、`not`  
   - 比較運算：`>`、`<`、`=` 等

2. **變數定義（define）**  
   - 可將符號綁定至 S-表達式。  
   - 禁止重新定義系統內建函式（如：`cons`、`car`）。

3. **條件判斷（Conditionals）**  
   - 支援 `if` 與 `cond` 條件式。

4. **序列運算（Sequencing）**  
   - 支援 `begin` 依序執行多個表達式。

5. **錯誤處理（Error Handling）**  
   - 評估過程中若發生錯誤（如：型別錯誤、參數錯誤），會顯示詳細錯誤訊息。

---

### **Execution Flow**
```text
Welcome to OurScheme!

> <user inputs S-expression>

<evaluation result or error message>

> 

(repeat until (exit) or EOF)

Thanks for using OurScheme!
```

---

### **Implemented Primitives**
以下爲已實作的基本功能，其中`（）`內為參數數量需求

1.	**Constructors**
    - cons (2)
    - list (>= 0)
2.	**Bypassing the default evaluation**
	 - quote (1)
    - ' (1)
3.	**The binding of a symbol to an S-expression**
	 - define (2)
4.	**Part accessors**
	 - car (1)
    - cdr (1)

5.	**Primitive predicates**  
下列功能皆只接受一個參數傳入:  
    - atom?
    - pair?
    - list?
    - integer?
    - real?
    - string?
    - boolean?
    - symbol?

6.	**Basic arithmetic, logical and string operations**
	 - \+ (>= 2)
    - \- (>= 2)
    - \* (>= 2)
    - / (>= 2)   
    - not (1)
    - and (>= 2)
    - or (>= 2)
    - \> (>= 2)
    - \>= (>= 2)
    - < (>= 2)
    - <= (>= 2)
    - = (>= 2)
    - string-append (>= 2)
    - string>? (>= 2)
    - string<? (>= 2)
    - string=? (>= 2)

7.	**Eqivalence tester**
	 - eqv? (2)
    - equal? (2)

8.	**Sequencing and functional composition**
	 - begin (>= 1)

9.	**Conditionals**  
	 - if (2 or 3)
    - cond (>= 1)

10.	**Clean environment**  
此指令會將user的definitions清空，一切重新開始
	- clean-environment (0)
---

### **Error Messages**

- ERROR (unbound symbol) : \<symbol>
- ERROR (incorrect number of arguments) : \<function>
- ERROR (attempt to apply non-function) : \<object>
- ERROR (division by zero) : /

---

### **Example Usage**
```text
> (define a 5)
a defined

> a
5

> (+ a 3)
8

> (if (> a 3) 'yes 'no)
yes

> (cons a '(1 2 3))
( 5
  1
  2
  3
)

> (clean-environment)
environment cleaned
```

---

**Thanks for using OurScheme!**
****
