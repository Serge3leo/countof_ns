# Comparison of methods for obtaining the number of array elements

## Table of Contents

- [C language standard](#c-language-standard)
- [Extensions of C language](#extensions-of-c-language)
- [Extensions of C++ language](#extensions-of-c-language-1)
- [Terms and definitions](#terms-and-definitions)
  - [Methods](#methods)
  - [Test results legend](#test-results-legend)
  - [Compiler ID](#compiler-id)

## C language standard

| ID        | Version     | array                                | cv array                              | ptr                                  | cv ptr                               | other                                | VLA                                      | ptr VLA                                  |
| --------- | ----------- | ------------------------------------ | ------------------------------------- | ------------------------------------ | ------------------------------------ | ------------------------------------ | ---------------------------------------- | ---------------------------------------- |
| Clang     | 14 - 21     | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof  | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | *countof_ns*_E<br>countof_ns¹<br>countof | *countof_ns*_E<br>countof_ns¹<br>countof |
| GNU       | 10 - 15     | countof_ns<br>countof_ns¹            | countof_ns<br>countof_ns¹             | countof_ns<br>countof_ns¹            | countof_ns<br>countof_ns¹            | countof_ns<br>countof_ns¹            | <br>countof_ns¹                          | <br>countof_ns¹                          |
| IntelLLVM | 2025.3      | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof  | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | <br>countof_ns¹<br>countof               | <br>countof_ns¹<br>countof               |
| MSVC      | 1939 - 1944 | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W       | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W      | Not implemented                          | Not implemented                          |
| NVHPC     | 25.9        | countof_ns<br>~~countof_ns¹~~_W      | **countof_ns_E**<br>~~countof_ns¹~~_W | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W      | Broken implementation                    | Broken implementation                    |
| PGI       |             |                                      |                                       |                                      |                                      |                                      |                                          |                                          |
| SunPro    | 5.15        | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W       | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W      | **countof_ns_E**<br>~~countof_ns¹~~_W    | **countof_ns_E**<br>~~countof_ns¹~~_W    |

## Extensions of C language

| ID        | Version     | ZLA[0][n]                            | ZLA[0][0]                            | ZLA[n][0]                                        | ptr ZLA[0]                                       | other<br>sizeof(0)                   | VLA[0][n]                            | VLA[0][n]                            | VLA[0][n]                                | ptr VLA[0]                               |
| --------- | ----------- | ------------------------------------ | ------------------------------------ | ------------------------------------------------ | ------------------------------------------------ | ------------------------------------ | ------------------------------------ | ------------------------------------ | ---------------------------------------- | ---------------------------------------- |
| Clang     | 14 - 21     | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | **countof_ns_E**<br>**countof¹_ns_E**<br>countof | **countof_ns_E**<br>**countof¹_ns_E**<br>countof | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | *countof_ns*_E<br>countof_ns¹<br>countof | *countof_ns*_E<br>countof_ns¹<br>countof |
| GNU       | 10 - 15     | countof_ns<br>countof_ns¹            | countof_ns<br>countof_ns¹            | **countof_ns_E**<br>**countof¹_ns_E**            | **countof_ns_E**<br>**countof¹_ns_E**            | countof_ns<br>countof_ns¹            | countof_ns<br>countof_ns¹            | countof_ns<br>countof_ns¹            | <br>countof_ns¹                          | <br>countof_ns¹                          |
| IntelLLVM | 2025.3      | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | **countof_ns_E**<br>**countof¹_ns_E**<br>countof | **countof_ns_E**<br>**countof¹_ns_E**<br>countof | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | <br>countof_ns¹<br>countof               | <br>countof_ns¹<br>countof               |
| MSVC      | 1939 - 1944 |                                      |                                      |                                                  |                                                  |                                      |                                      |                                      |                                          |                                          |
| NVHPC     | 25.9        | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W      | **countof_ns_E**<br>~~countof_ns¹~~_W            | **countof_ns_E**<br>~~countof_ns¹~~_W            | countof_ns<br>~~countof_ns¹~~_W      | Broken implementation                | Broken implementation                | Broken implementation                    | Broken implementation                    |
| PGI       |             |                                      |                                      |                                                  |                                                  |                                      |                                      |                                      |                                          |                                          |
| SunPro    | 5.15        |                                      |                                      |                                                  |                                                  |                                      |                                      |                                      |                                          |                                          |

## Extensions of C++ language

| ID        | Version     | ZLA[0][n]                            | ZLA[0][0]                            | ZLA[n][0]                                        | ptr ZLA[0]                                       | other<br>sizeof(0)                   |
| --------- | ----------- | ------------------------------------ | ------------------------------------ | ------------------------------------------------ | ------------------------------------------------ | ------------------------------------ |
| Clang     | 14 - 21     | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | **countof_ns_E**<br>**countof¹_ns_E**<br>countof | **countof_ns_E**<br>**countof¹_ns_E**<br>countof | countof_ns<br>countof_ns¹<br>countof |
| GNU       | 10 - 15     | countof_ns<br>countof_ns¹            | countof_ns<br>countof_ns¹            | **countof_ns_E**<br>**countof¹_ns_E**            | **countof_ns_E**<br>**countof¹_ns_E**            | countof_ns<br>countof_ns¹            |
| IntelLLVM | 2025.3      | countof_ns<br>countof_ns¹<br>countof | countof_ns<br>countof_ns¹<br>countof | **countof_ns_E**<br>**countof¹_ns_E**<br>countof | **countof_ns_E**<br>**countof¹_ns_E**<br>countof | countof_ns<br>countof_ns¹<br>countof |
| MSVC      | 1939 - 1944 |                                      |                                      |                                                  |                                                  |                                      |
| NVHPC     | 25.9        | countof_ns<br>~~countof_ns¹~~_W      | countof_ns<br>~~countof_ns¹~~_W      | **countof_ns_E**<br>~~countof_ns¹~~_W            | **countof_ns_E**<br>~~countof_ns¹~~_W            | countof_ns<br>~~countof_ns¹~~_W      |
| PGI       |             |                                      |                                      |                                                  |                                                  |                                      |
| SunPro    | 5.15        |                                      |                                      |                                                  |                                                  |                                      |

## Terms and definitions

### Methods

| Name        | Language | Description                                                                                        |
| ----------- | -------- | -------------------------------------------------------------------------------------------------- |
| countof_ns  | C/C++    | Макрос `countof_ns()` из "countof_ns.h" по умолчанию                                               |
| countof_ns¹ | C/C++    | Макрос `countof_ns()` из "countof_ns.h" с предварительным определением `_COUNTOF_NS_WANT\_С11_VLA` |
| countof     | С        | Реализация проекта C2y  компиляторами Clang 21 и IntelLLVM 2025.3                                  |
| ARRAY_SIZE  | С        | Реализация Linux Kernel TODO                                                                       |
| ms_countof  | C/C++    | Копия реализации `_countof()` MSVC                                                                 |
| COUNTOF     | С        | Реализация [alx - recommends codidact](https://stackoverflow.com/a/57537491/8585880)               |
| ARRAY_LEN   | C/C++    | Реализация [James Z.M. Gao](https://stackoverflow.com/a/77001872/8585880)                          |
| std::size   | С++      | Функция стандарта C++17                                                                            |

### Test results legend

| Legend                  | Description                                                                                                                  |
| ----------------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| countof_ns              | Успех.                                                                                                                       |
| **countof_ns_E**        | Ошибка сборки для положительного теста, т.е. ложноотрицательный результат.                                                   |
| **countof_ns_F0**       | Некорректный результат 0, для положительного теста.                                                                          |
| **countof_ns_Fany💥**   | Некорректный результат отличный от 0, для положительного теста. Не встречается.                                              |
| **countof_ns_W**        | Предупреждение при сборке положительного теста.                                                                              |
| ~~countof_ns¹~~_W       | Предупреждение при сборке отрицательного теста и его выполнение без ошибок, т.е. ложноотрицательным результат.               |
| ~~method~~_NW           | Сборка и выполнение отрицательного теста без предупреждений и ошибок. Не встречается.                                        |
| **~~ms_countof~~_W💥**  | Предупреждение при сборке отрицательного теста и UB (аварийный отказ) при его исполнении, т.е. ложноотрицательным результат. |
| **~~ms_countof~~_NW💥** | Сборка отрицательного теста без предупреждений и UB (аварийный отказ) при его исполнении. Не встречается.                    |

### Compiler ID

| ID        | Description                                                                                               |
| --------- | --------------------------------------------------------------------------------------------------------- |
| Clang     | [LLVM Clang C/C++](https://clang.llvm.org/)                                                               |
| GNU       | [GNU C/C++](https://gcc.gnu.org/)                                                                         |
| IntelLLVM | [Intel LLVM-Based Compiler](https://www.intel.com/content/www/us/en/developer/tools/oneapi/overview.html) |
| MSVC      | [Microsoft Visual Studio](https://visualstudio.microsoft.com/)                                            |
| NVHPC     | [NVIDIA HPC Compiler](https://developer.nvidia.com/hpc-compilers)                                         |
| PGI       | The Portland Group                                                                                        |
| SunPro    | Oracle Developer Studio                                                                                   |


